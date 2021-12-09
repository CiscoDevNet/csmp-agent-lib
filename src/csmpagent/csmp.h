/*
 * Cisco CSMP Reference Implementation Source Code License Agreement v1.0
 *
 * These terms govern your use of Cisco Systems, Inc.’s CSMP Reference
 * Implementation Source Code and associated documentation (“Licensed
 * Materials”). By accessing, downloading, copying, distributing, calling, or
 * otherwise making any use of the Licensed Materials, you accept and agree to
 * be bound by the following terms and conditions (the “License”). If you are
 * accepting the License on behalf of an entity, you represent that you have the
 * authority to do so (either you or the entity, “you”). All use of the Licensed
 * Materials must be in accordance with this License and all rights not
 * expressly granted herein are reserved.

 * 1.	LICENSE GRANT.
 * Subject to Your compliance with the terms of this License, Cisco grants you
 * a limited, non-exclusive, perpetual, revocable, non-sublicensable,
 * royalty-free, worldwide, and non-transferable license to: (a) use the
 * Licensed Materials for the purpose of internal development, testing,
 * maintenance and support of your products to interoperate with Cisco 802.15.4
 * border routers -- including without limitation the  Cisco 1000 Series
 * Connected Grid Routers – and Cisco IoT Field Network Director and any
 * successor products (collectively the “Cisco Products”), and for no other
 * purpose; (b) use and reproduce a reasonable number of copies of the Licensed
 * Materials solely for the internal development, testing, back-up, maintenance
 * and support of your products as permitted under this License; (c) make
 * corrections and bug fixes to the Licensed Materials only if such corrections
 * and bug fixes do not interfere with or inhibit the interoperability of your
 * products with the Cisco Products; and (d) distribute the CSMP Reference
 * Implementation Source Code solely in compiled, binary format with your
 * products to your customers solely to enable your products to interoperate
 * with the Cisco Products.

 * 2.	CONDITIONS.
 * You shall not use the Licensed Materials to: (a) substantially replicate and
 * compete with the Cisco Products; (b) collect, store, or transmit confidential
 * , sensitive, or protected data or information without authorization; (c)
 * disable, disrupt, circumvent, interfere with, or otherwise violate the
 * security of any Cisco product, service, or user;  (d) access any application,
 * system, service, computer, data, account, or network without authorization;
 * (e) develop features or functions in stand-alone applications that do not
 * interoperate with the Cisco Products;  or (f) sell or offer to sell the
 * Licensed Products on a standalone basis. The Cisco Products are licensed
 * under their own separate terms and you shall not use the Licensed Materials
 * in any way that violates or is inconsistent with those terms (for more
 * information, please visit: www.cisco.com/go/terms). Failure to comply with
 * any part of this section or section 2 constitutes a material breach of this
 * License, terminates the license granted in Section 1 above, and may result in
 * further repercussions and actions against you, at Cisco’s sole discretion.

 * 3.	OWNERSHIP.
 * The Licensed Materials, in whole or in part, in all forms, and any
 * corrections or bug fixes thereto that you make are the sole and exclusive
 * property of Cisco.   You irrevocably transfer, convey, and assign to Cisco
 * all of your right, title, and interest in any corrections or bug fixes you
 * make to the Licensed Materials.  You shall send the corrections or bug fixes
 * to ask-crmesh-dev@cisco.com.  This License does not grant you permission to
 * use any trade names, trademarks, service marks, or product names of Cisco.
 * If you provide any feedback to Cisco regarding the Licensed Materials, you
 * agree that Cisco, its suppliers, and its customers shall be free to use and
 * incorporate such feedback into the Licensed Materials and Cisco products and
 * services without restriction, payment, or additional consideration of any
 * kind. If you initiate or participate in any litigation against Cisco, its
 * partners, or its customers (including cross-claims and counter-claims)
 * alleging that the Licensed Materials or its use infringe any patent,
 * copyright, or other intellectual property right, then all rights granted to
 * you under this License shall terminate immediately.

 * 4.	CONFIDENTIAL INFORMATION.
 * The Licensed Materials, whether in whole or in part, contain confidential
 * information and proprietary property of Cisco.  Licensee acknowledges that
 * the Licensed Materials contain trade secrets and other confidential
 * information which are the property of Cisco.  You shall keep the Licensed
 * Materials in strict confidence and take all reasonable steps to prevent: (a)
 * unauthorized persons from having access to the Licensed Materials; (b)
 * disclosure of the Licensed Materials to any third party; and (c) unauthorized
 * copying of the Licensed Materials.

 * 5.	PROPRIETARY NOTICES.
 * You agree to maintain and reproduce all copyright, trademarks and other
 * proprietary notices on all copies of the Licensed Materials, in any form, in
 * the same form and manner that such copyright, trademarks, and other
 * proprietary notices are included on the Licensed Materials.

 * 6.	NO SUPPORT.
 * Nothing in this License shall require Cisco to provide any support, services,
 * updates, upgrades, bug fixes or any other modifications to the Licensed
 * Materials.  You shall be solely responsible for providing support to your
 * customers for any problems or issues that they encounter by using the CSMP
 * Reference Implementation Source Code in binary form that you distributed.
 * Cisco may, at its own sole discretion, update, extend, enhance, or otherwise
 * modify the Licensed Materials without notice.  If Cisco provides updates to
 * you and no separate agreement accompanies such updates, then such updates are
 * subject to this License.

 * 7.	LIMITATION OF LIABILITY.
 * CISCO SHALL HAVE NO LIABILITY IN CONNECTION WITH OR RELATING TO THIS LICENSE
 * OR USE OF THE LICENSED MATERIALS, FOR DAMAGES OF ANY KIND, INCLUDING BUT NOT
 * LIMITED TO DIRECT, INCIDENTAL, AND CONSEQUENTIAL DAMAGES, OR FOR ANY LOSS OF
 * USE, DATA, INFORMATION, PROFITS, BUSINESS, OR GOODWILL, HOWEVER CAUSED, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

 * 8.	DISCLAIMER OF WARRANTY.
 * CISCO PROVIDES THE LICENSED MATERIALS “AS IS” WITH ALL FAULTS, WITHOUT
 * WARRANTY OF ANY KIND. TO THE MAXIMUM EXTENT PERMITTED BY LAW, ALL EXPRESS AND
 * IMPLIED CONDITIONS, REPRESENTATIONS, AND WARRANTIES INCLUDING, WITHOUT
 * LIMITATION, ANY IMPLIED WARRANTY OR CONDITION OF MERCHANTABILITY, FITNESS FOR
 * A PARTICULAR PURPOSE, NON-INFRINGEMENT, SATISFACTORY QUALITY,
 * NON-INTERFERENCE, AND ACCURACY, ARE HEREBY EXCLUDED AND EXPRESSLY DISCLAIMED
 * BY CISCO. CISCO DOES NOT WARRANT THAT THE LICENSED MATERIALS ARE SUITABLE FOR
 * YOUR USE, WILL OPERATE PROPERLY WITH YOUR APPLICATION, ARE ACCURATE OR
 * COMPLETE, OR ARE WITHOUT ERROR OR DEFECT.

 * 9.	GENERAL.
 * This License shall be governed by and interpreted in accordance with the laws
 * of the State of California, excluding its conflict of law’s provisions. You
 * agree to comply with all applicable United States export laws, rules, and
 * regulations. If any provision of this License is judged illegal, invalid, or
 * otherwise unenforceable, that provision shall be severed, and the rest of the
 * License shall remain in full force and effect. No failure by Cisco to enforce
 * any of its rights related to the Licensed Materials or to a breach of this
 * License in a particular situation will act as a waiver of such rights. In the
 * event of any inconsistencies with any other terms that may apply to the
 * Licensed Materials, this License shall take precedence.
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
