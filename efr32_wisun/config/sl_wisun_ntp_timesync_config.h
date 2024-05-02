/***************************************************************************//**
 * @file
 * @brief Wi-SUN NTP Time Synchronization configuration.
 *
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef __SL_WISUN_NTP_TIMESYNC_CONFIG_H__
#define __SL_WISUN_NTP_TIMESYNC_CONFIG_H__

// <<< Use Configuration Wizard in Context Menu >>>

// <h> NTP Time Synchronization Configuration

// <s SL_WISUN_NTP_TIMESYNC_SERVER_IPV6_ADDRESS> NTP server IPv6 address
#define SL_WISUN_NTP_TIMESYNC_SERVER_IPV6_ADDRESS        "fd12:3456::b6e3:f9ff:fec5:84ff"

// <o SL_WISUN_NTP_TIMESYNC_TIMEZONE_UTC_OFFSET_HOUR> UTC Timezone offset in hours
// <i> Default: 2
#define SL_WISUN_NTP_TIMESYNC_TIMEZONE_UTC_OFFSET_HOUR   2

// </h>

// <<< end of configuration section >>>

#endif