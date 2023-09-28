/*
 *  Copyright 2021 Cisco Systems, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef _CSMP_H
#define _CSMP_H

/*! \file
 *
 * CSMP
 *
 */

#include <stdint.h>
#include "coap.h"

/* CSMP return codes */
#define CSMP_OP_TLV_RD_EMPTY   0
#define CSMP_OP_TLV_RD_ERROR   -1
#define CSMP_OP_TLV_WR_ERROR   -2
#define CSMP_OP_UNSUPPORTED    -3
#define CSMP_OP_FAILED         -4

/* return codes */
#define SUCCESS 1
#define FAILURE 0
#define ERROR -1

/**
 * @brief vendor info
 *
 */
typedef struct _tlvid_t {
  uint32_t vendor;  /**< vendor identifier */
  uint32_t type;    /**< type */
} tlvid_t;

/**
 * @brief event identifier
 *
 */
typedef struct _eventid_t {
  uint32_t vendor; /**< vendor identifier */
  uint32_t code;  /**< code */
} eventid_t;

#ifndef MAX_SUBSCRIBE_LIST_CNT
/** maximum subscribers */
#define MAX_SUBSCRIBE_LIST_CNT (15)
#endif

#ifndef MAX_SIGNATURE_CERT_LENGTH
/** maximum CSMP cert length */
#define MAX_SIGNATURE_CERT_LENGTH 512
#endif

#ifndef MAX_NOTIFY_TLVID_CNT
/** maximum error TLV*/
#define MAX_NOTIFY_TLVID_CNT 20
#endif

/**
 * @brief subscription list
 *
 */
typedef struct {
  uint32_t period;  /**< period */
  uint32_t cnt;  /**< counter for the subscriber */
  tlvid_t list[MAX_SUBSCRIBE_LIST_CNT];  /**< list of subscribers */
} csmp_subscription_list_t;

/**
 * @brief event list
 *
 */
typedef struct {
  tlvid_t id;    /**< identifier */
  int32_t index;  /**< index */
} csmp_event_tlvlist_t;

/**
 * @brief event statistics
 *
 */
typedef struct {
  uint16_t trigger_low; /**< trigger low */
  uint16_t trigger_med; /**< triggger medium */
  uint16_t trigger_high; /**< trigger high */
  uint16_t sent; /**< trigger processed */
  uint16_t bad_priority; /**< bad priority */
} csmp_event_stats_t;

/**
 * @brief public key
 *
 */
typedef struct {
  uint8_t *key;  /**< key */
  uint32_t keylen; /**< length of key */
} csmp_sigkey_t;

/**
 * @brief priority enum
 *
 */
typedef enum {
  CSMP_EVENT_PRIORITY_INVALID = 0,  /**< invalid */
  CSMP_EVENT_PRIORITY_LOW = 1,      /**< low priority */
  CSMP_EVENT_PRIORITY_MEDIUM = 2,   /**< medium priority */
  CSMP_EVENT_PRIORITY_HIGH = 3      /**< high priority*/
} csmp_event_priority_t;

enum {
  CSMP_LEN_SKIP = 2
};

enum {
  CSMP_DEFAULT_PORT = 61628U
};

enum {
  CSMP_TYPE_VENDOR = 127U
};

enum {
  CSMP_GROUP_TYPE_CONF   = 1,
  CSMP_GROUP_TYPE_FW     = 2,
  CSMP_GROUP_NUM_TYPES   = 3
};

enum {
  CSMP_PHYSICAL_FUNCTION_METER = 1,
  CSMP_PHYSICAL_FUNCTION_RE = 2,
  CSMP_PHYSICAL_FUNCTION_DAG = 3,
  CSMP_PHYSICAL_FUNCTION_IOT = 4,
  CSMP_PHYSICAL_FUNCTION_UNKNOWN = 99
};

enum {
  CSMP_CGMS_SUC_PROCESS = 0,
  CSMP_CGMS_ERR_COAP = 1,
  CSMP_CGMS_ERR_SIGNATURE = 2,
  CSMP_CGMS_ERR_PROCESS = 3
};

// Follows entPhysicalClass OID 1.3.6.1.2.1.47.1.1.1.1.5
enum {
  CSMP_PHYSICAL_CLASS_OTHER = 1,
  CSMP_PHYSICAL_CLASS_UNKNOWN = 2,
  CSMP_PHYSICAL_CLASS_CHASSIS = 3,
  CSMP_PHYSICAL_CLASS_BACKPLANE = 4,
  CSMP_PHYSICAL_CLASS_CONTAINER = 5,
  CSMP_PHYSICAL_CLASS_POWERSUPPLY = 6,
  CSMP_PHYSICAL_CLASS_FAN = 7,
  CSMP_PHYSICAL_CLASS_SENSOR = 8,
  CSMP_PHYSICAL_CLASS_MODULE = 9,
  CSMP_PHYSICAL_CLASS_PORT = 10,
  CSMP_PHYSICAL_CLASS_STACK = 11,
  CSMP_PHYSICAL_CLASS_CPU = 12,
};

enum {
  TLV_INDEX_TLVID = 1,
  DEVICE_ID_TLVID = 2,
  AGENT_URL_TLVID = 3,
  NMSREDIRECT_REQUEST_TLVID = 6,
  SESSION_ID_TLVID = 7,
  DESCRIPTION_REQUEST_TLVID = 8,
  HARDWARE_DESC_TLVID = 11,
  INTERFACE_DESC_TLVID = 12,
  REPORT_SUBSCRIBE_TLVID = 13,
  IPADDRESS_TLVID = 16,
  IPROUTE_TLVID = 17,
  CURRENT_TIME_TLVID = 18,
  RPLSETTINGS_TLVID = 21,
  UPTIME_TLVID = 22,
  INTERFACE_METRICS_TLVID = 23,
  IPROUTE_RPLMETRICS_TLVID = 25,
  PING_REQUEST_TLVID = 30,
  PING_RESPONSE_TLVID = 31,
  REBOOT_REQUEST_TLVID = 32,
  IEEE8021X_STATUS_TLVID = 33,
  IEEE80211I_STATUS_TLVID = 34,
  WPANSTATUS_TLVID = 35,
  DHCP6_CLIENT_STATUS_TLVID = 36,
  CGMSSETTINGS_TLVID = 42,
  CGMSSTATUS_TLVID = 43,
  CGMSNOTIFICATION_TLVID = 44,
  CGMSSTATS_TLVID = 45,
  IEEE8021X_SETTINGS_TLVID = 47,
  IEEE802154_BEACON_STATS_TLVID = 48,
  RPLINSTANCE_TLVID = 53,
  GROUP_ASSIGN_TLVID = 55,
  GROUP_EVICT_TLVID = 56,
  GROUP_MATCH_TLVID = 57,
  GROUP_INFO_TLVID = 58,
  LOWPAN_MAC_STATS_TLVID = 62,
  LOWPAN_PHY_SETTINGS_TLVID = 63,
  TRANSFER_REQUEST_TLVID = 65,
  IMAGE_BLOCK_TLVID = 67,
  LOAD_REQUEST_TLVID = 68,
  CANCEL_LOAD_REQUEST_TLVID = 69,
  SET_BACKUP_REQUEST_TLVID = 70,
  TRANSFER_RESPONSE_TLVID = 71,
  LOAD_RESPONSE_TLVID = 72,
  CANCEL_LOAD_RESPONSE_TLVID = 73,
  SET_BACKUP_RESPONSE_TLVID = 74,
  FIRMWARE_IMAGE_INFO_TLVID = 75,
  SIGNATURE_VALIDITY_TLVID = 76,
  SIGNATURE_TLVID = 77,
  SIGNATURE_CERT_TLVID = 78,
  SIGNATURE_SETTINGS_TLVID = 79,
  IEEE8021X_AAASEC_TLVID = 80,
  IEEE8021X_CLIENT_SEC_TLVID = 81,
  NET_STAT_TLVID = 124,
  NETWORK_ROLE_TLVID = 141,
  MPL_STATS_TLVID = 241,
  MPL_RESET_TLVID = 242,
  RPL_STATS_TLVID = 313,
  DHCP6_STATS_TLVID = 314,
  EVENT_REPORT_TLVID = 500,
};

#define TLV_INDEX_ID_STRING "1"
#define DEVICE_ID_ID_STRING "2"
#define AGENT_URL_ID_STRING "3"
#define NMSREDIRECT_REQUEST_ID_STRING "6"
#define SESSION_ID_ID_STRING "7"
#define DESCRIPTION_REQUEST_ID_STRING "8"
#define HARDWARE_DESC_ID_STRING "11"
#define INTERFACE_DESC_ID_STRING "12"
#define REPORT_SUBSCRIBE_ID_STRING "13"
#define IPADDRESS_ID_STRING "16"
#define IPROUTE_ID_STRING "17"
#define CURRENT_TIME_ID_STRING "18"
#define RPLSETTINGS_ID_STRING "21"
#define UPTIME_ID_STRING "22"
#define INTERFACE_METRICS_ID_STRING "23"
#define IPROUTE_RPLMETRICS_ID_STRING "25"
#define PING_REQUEST_ID_STRING "30"
#define PING_RESPONSE_ID_STRING "31"
#define REBOOT_REQUEST_ID_STRING "32"
#define IEEE8021X_STATUS_ID_STRING "33"
#define IEEE80211I_STATUS_ID_STRING "34"
#define WPANSTATUS_ID_STRING "35"
#define DHCP6_CLIENT_STATUS_ID_STRING "36"
#define CGMSSETTINGS_ID_STRING "42"
#define CGMSSTATUS_ID_STRING "43"
#define IEEE8021X_SETTINGS_ID_STRING "47"
#define IEEE802154_BEACON_STATS_ID_STRING "48"
#define RPLINSTANCE_ID_STRING "53"
#define GROUP_ASSIGN_ID_STRING "55"
#define GROUP_EVICT_ID_STRING "56"
#define GROUP_MATCH_ID_STRING "57"
#define GROUP_INFO_ID_STRING "58"
#define LOWPAN_MAC_STATS_ID_STRING "62"
#define LOWPAN_PHY_SETTINGS_ID_STRING "63"
#define TRANSFER_REQUEST_ID_STRING "65"
#define IMAGE_BLOCK_ID_STRING "67"
#define LOAD_REQUEST_ID_STRING "68"
#define CANCEL_LOAD_REQUEST_ID_STRING "69"
#define SET_BACKUP_REQUEST_ID_STRING "70"
#define TRANSFER_RESPONSE_ID_STRING "71"
#define LOAD_RESPONSE_ID_STRING "72"
#define CANCEL_LOAD_RESPONSE_ID_STRING "73"
#define SET_BACKUP_RESPONSE_ID_STRING "74"
#define FIRMWARE_IMAGE_INFO_ID_STRING "75"
#define SIGNATURE_VALIDITY_ID_STRING "76"
#define SIGNATURE_ID_STRING "77"
#define SIGNATURE_CERT_ID_STRING "78"
#define SIGNATURE_SETTINGS_ID_STRING "79"
#define IEEE8021X_AAASEC_ID_STRING "80"
#define IEEE8021X_CLIENT_SEC_ID_STRING "81"
#define NET_STAT_ID_STRING "124"
#define NETWORK_ROLE_ID_STRING "141"
#define MPL_STATS_ID_STRING "241"
#define MPL_RESET_STRING "242"
#define RPL_STATS_TLVID_STRING "313"
#define DHCP6_STATS_TLVID_STRING "314"
#define EVENT_REPORT_ID_STRING "500"

#endif
