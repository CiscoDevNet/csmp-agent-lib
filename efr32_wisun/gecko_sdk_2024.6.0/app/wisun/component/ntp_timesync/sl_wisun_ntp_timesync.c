/***************************************************************************//**
 * @file
 * @brief Wi-SUN NTP Time Synchronization.
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "sl_wisun_ntp_timesync.h"
#include "sl_wisun_ntp_timesync_config.h"
#include "sl_status.h"
#include "socket.h"
#include "sl_sleeptimer.h"
#include "sl_sleeptimer_config.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

/// Number of times to retry receiving an NTP packet.
#define SL_WISUN_NTP_TIMESYNC_RECV_RETRY_COUNT 100U

/// NTP Port number.
#define SL_WISUN_NTP_TIMESYNC_PORT             123U

/// Timestamp delta between NTP epoch and Unix epoch.
#define SL_WISUN_NTP_TIMESYNC_TIMESTAMP_DELTA  2208988800ULL

/// Change the endianness of a 32-bit integer.
#define __change_endianess(x) \
  (((x & 0x000000FF) << 24) | \
  ((x & 0x0000FF00) << 8) | \
  ((x & 0x00FF0000) >> 8) | \
  ((x & 0xFF000000) >> 24))

/// NTP packet
/// Total: 384 bits or 48 bytes.
typedef struct sl_wisun_ntp_packet {
    /// Eight bits. li, vn, and mode.
    /// li.   Two bits.   Leap indicator.
    /// vn.   Three bits. Version number of the protocol.
    /// mode. Three bits. Client will pick mode 3 for client.
    uint8_t li_vn_mode;      
    /// Eight bits. Stratum level of the local clock.
    uint8_t stratum;
    /// Eight bits. Maximum interval between successive messages.         
    uint8_t poll;
    /// Eight bits. Precision of the local clock.
    uint8_t precision;
    /// 32 bits. Total round trip delay time.
    uint32_t root_delay;
    /// 32 bits. Max error aloud from primary clock source. 
    uint32_t root_dispersion;
    /// 32 bits. Reference clock identifier.
    uint32_t ref_id;
    /// 32 bits. Reference time-stamp seconds.
    uint32_t ref_tm_s;
    /// 32 bits. Reference time-stamp fraction of a second.
    uint32_t ref_tm_f;
    /// 32 bits. Originate time-stamp seconds.
    uint32_t orig_tm_s;
    /// 32 bits. Originate time-stamp fraction of a second.
    uint32_t orig_tm_f;
    /// 32 bits. Received time-stamp seconds.
    uint32_t rx_tm_s;
    /// 32 bits. Received time-stamp fraction of a second.
    uint32_t rx_tm_f;
    /// 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
    uint32_t tx_tm_s;
    /// 32 bits. Transmit time-stamp fraction of a second.
    uint32_t tx_tm_f;
} sl_wisun_ntp_packet_t;

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                                Static Variables
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

#if SL_SLEEPTIMER_WALLCLOCK_CONFIG
sl_status_t sl_wisun_ntp_timesync(void)
{
  static sl_wisun_ntp_packet_t packet = { 0 };
  static sockaddr_in6_t server_addr = { 0 };
  static sockaddr_in6_t tmp_addr = { 0 };
  static char ipv6_str[40] = { 0 };
  int32_t sockfd = 0;
  int32_t ret = 0;
  socklen_t len = sizeof(sockaddr_in6_t);
  sl_sleeptimer_timestamp_t ts = 0;
  static sl_sleeptimer_date_t date = { 0 };
  static char date_str[128] = { 0 };
  bool result = false;

  packet.li_vn_mode = 0x1b;

  sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
  if (sockfd < 0) {
    printf("[NTP Timesync: Error opening socket]\n");
    return SL_STATUS_FAIL;
  }

  server_addr.sin6_family = AF_INET6;
  server_addr.sin6_port = htons(SL_WISUN_NTP_TIMESYNC_PORT);

  if (inet_pton(AF_INET6, SL_WISUN_NTP_TIMESYNC_SERVER_IPV6_ADDRESS, &server_addr.sin6_addr) < 0) {
    printf("[NTP Timesync: Invalid NTP server address]\n");
    return SL_STATUS_FAIL;
  }

  if (connect(sockfd, (const struct sockaddr *)&server_addr, sizeof(sockaddr_in6_t)) < 0) {
    printf("[NTP Timesync: Error connecting to NTP server]\n");
    return SL_STATUS_FAIL;
  }

  if (sendto(sockfd, &packet, sizeof(sl_wisun_ntp_packet_t), 0, 
             (const struct sockaddr *)&server_addr, sizeof(sockaddr_in6_t)) < 0) {
    printf("[NTP Timesync: Error sending NTP request]\n");
    return SL_STATUS_FAIL;
  }

  for (uint32_t i = 0U; i < SL_WISUN_NTP_TIMESYNC_RECV_RETRY_COUNT; i++) {

    ret = recvfrom(sockfd, &packet, sizeof(sl_wisun_ntp_packet_t), 0, 
                   (struct sockaddr *)&tmp_addr, &len);
    if (ret < 0) {
      continue;
    }

    if (inet_ntop(AF_INET6, &tmp_addr.sin6_addr, ipv6_str, sizeof(ipv6_str)) == NULL) {
      printf("[NTP Timesync: Error converting IPv6 address]\n");
      continue;
    }

    if (memcmp(&tmp_addr.sin6_addr, &server_addr.sin6_addr, sizeof(in6_addr_t)) != 0) {
      printf("[NTP Timesync: Received packet from unknown source]\n");
      continue;
    }

    packet.tx_tm_s = __change_endianess(packet.tx_tm_s);
    packet.tx_tm_f = __change_endianess(packet.tx_tm_f);

    ts = (sl_sleeptimer_timestamp_t)((uint64_t)packet.tx_tm_s - SL_WISUN_NTP_TIMESYNC_TIMESTAMP_DELTA);
    
    if (sl_sleeptimer_set_time(ts) != SL_STATUS_OK) {
      printf("[NTP Timesync: Error setting time]\n");
      break;
    }

    (void) sl_sleeptimer_convert_time_to_date(ts, 
                                              SL_WISUN_NTP_TIMESYNC_TIMEZONE_UTC_OFFSET_HOUR * 60 *60,
                                              &date);
    (void) sl_sleeptimer_convert_date_to_str(date_str, sizeof(date_str), (const uint8_t *) "%x - %I:%M:%S %p", &date);
    
    printf("[NTP Timesync: %s]\n", date_str);
    result = true;
    break;
  }
  
  close(sockfd);

  return result ? SL_STATUS_OK : SL_STATUS_FAIL;
}

#else

sl_status_t sl_wisun_ntp_timesync(void)
{
  (void) 0;
  return SL_STATUS_FAIL;
}

#endif

// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------
