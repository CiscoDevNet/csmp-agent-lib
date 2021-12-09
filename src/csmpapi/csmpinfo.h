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

#ifndef CSMP_INFO_H
#define CSMP_INFO_H

#include <stdbool.h>
#include <stdint.h>

/*! \file
 *
 * CSMP info
 *
 * Type definition of payloads as structures that can be used with protobuf.
 * also includes initial values that can be used for initialisation
 */


// structs for each tlv
typedef struct _Hardware_Desc Hardware_Desc;
typedef struct _Interface_Desc Interface_Desc;
typedef struct _IP_Address IP_Address;
typedef struct _IP_Route IP_Route;
typedef struct _Current_Time Current_Time;
typedef struct _Up_time Up_Time;
typedef struct _Interface_Metrics Interface_Metrics;
typedef struct _IPRoute_RPLMetrics IPRoute_RPLMetrics;
typedef struct _WPAN_Status WPAN_Status;
typedef struct _Neighbor_802154G Neighbor_802154G;
typedef struct _RPL_Instance RPL_Instance;
typedef struct _Firmware_Image_Info Firmware_Image_Info;

// HARDWARE_DESC
typedef enum {
  OTHERPHY = 1,
  UNKNOWNPHY = 2,
  CHASSIS= 3,
  BACKPLANE = 4,
  CONTAINER = 5,
  POWRER_SUPPLY = 6,
  FAN = 7,
  SENSOR = 8,
  MODULE = 9,
  PORT = 10,
  STACK = 11,
  CPU = 12
} physical_class_t;

struct  _Hardware_Desc
{
  bool has_entphysicalindex;
  int32_t entphysicalindex;
  bool has_entphysicaldescr;
  char entphysicaldescr[64];
  bool has_entphysicalvendortype;
  struct {
    size_t len;
    uint8_t data[16];
  } entphysicalvendortype;
  bool has_entphysicalcontainedin;
  int32_t entphysicalcontainedin;
  bool has_entphysicalclass;
  int32_t entphysicalclass;  //ref to physical_class_t
  bool has_entphysicalparentrelpos;
  int32_t entphysicalparentrelpos;
  bool has_entphysicalname;
  char entphysicalname[32];
  bool has_entphysicalhardwarerev;
  char entphysicalhardwarerev[16];
  bool has_entphysicalfirmwarerev;
  char entphysicalfirmwarerev[16];
  bool has_entphysicalsoftwarerev;
  char entphysicalsoftwarerev[16];
  bool has_entphysicalserialnum;
  char entphysicalserialnum[18];
  bool has_entphysicalmfgname;
  char entphysicalmfgname[32];
  bool has_entphysicalmodelname;
  char entphysicalmodelname[20];
  bool has_entphysicalassetid;
  char entphysicalassetid[32];
  bool has_entphysicalmfgdate;
  uint32_t entphysicalmfgdate;
  bool has_entphysicaluris;
  char entphysicaluris[64];
  bool has_entphysicalfunction;
  uint32_t entphysicalfunction;
  bool has_entphysicaloui;
  char entphysicaloui[8];
};
#define HARDWARE_DESC_INIT \
 { 0,0, 0,{0}, 0,{0,{0}}, 0,0, 0,0, 0,0, 0,{0}, 0,{0}, 0,{0}, 0,{0}, 0,{0}, 0,{0}, 0,{0}, 0,{0}, 0,0, 0,{0}, 0,0, 0,{0} }


// INTERFACE_DESC
struct  _Interface_Desc
{
  bool has_ifindex;
  int32_t ifindex;
  bool has_ifname;
  char ifname[20];
  bool has_ifdescr;
  char ifdescr[32];
  bool has_iftype;
  int32_t iftype;
  bool has_ifmtu;
  int32_t ifmtu;
  bool has_ifphysaddress;
  struct {
    size_t len;
    uint8_t data[8];
  } ifphysaddress;
};
#define INTERFACE_DESC_INIT \
 { 0,0, 0,{0}, 0,{0}, 0,0, 0,0, 0,{0,{0}} }


// IPADDRESS
typedef enum {
  UNKNOWN = 0,
  IPV4 = 1,
  IPV6 = 2,
  IPV4Z = 3,
  IPV6Z = 4,
  DNS =  16
} ipaddr_type_t;

typedef enum {
  UNICAST = 1,
  ANYCAST = 2,
  BROADCAST = 3,
  MULTICAST = 4
} ip_type_t;

typedef enum {
  OTHER = 1,
  MANUAL = 2,
  DHCP = 4,
  LINKLAYER = 5,
  RANDOM = 6
} ip_origin_t;

struct  _IP_Address
{
  bool has_ipaddressindex;
  int32_t ipaddressindex;
  bool has_ipaddressaddrtype;
  uint32_t ipaddressaddrtype;  //ref to ipaddr_type_t
  bool has_ipaddressaddr;
  struct {
    size_t len;
    uint8_t data[16];
  } ipaddressaddr;
  bool has_ipaddressifindex;
  int32_t ipaddressifindex;
  bool has_ipaddresstype;
  uint32_t ipaddresstype;    //ref to ip_type_t
  bool has_ipaddressorigin;
  uint32_t ipaddressorigin;  //ref to ip_origin_t
  bool has_ipaddressstatus;
  uint32_t ipaddressstatus;
  bool has_ipaddresscreated;
  uint32_t ipaddresscreated;
  bool has_ipaddresslastchanged;
  uint32_t ipaddresslastchanged;
  bool has_ipaddresspfxlen;
  uint32_t ipaddresspfxlen;
};
#define IPADDRESS_INIT \
 { 0,0, 0,0, 0,{0,{0}}, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 }


// IPROUTE
struct  _IP_Route
{
  bool has_inetcidrrouteindex;
  int32_t inetcidrrouteindex;
  bool has_inetcidrroutedesttype;
  uint32_t inetcidrroutedesttype;
  bool has_inetcidrroutedest;
  struct {
    size_t len;
    uint8_t data[16];
  } inetcidrroutedest;
  bool has_inetcidrroutepfxlen;
  uint32_t inetcidrroutepfxlen;
  bool has_inetcidrroutenexthoptype;
  uint32_t inetcidrroutenexthoptype;
  bool has_inetcidrroutenexthop;
  struct {
    size_t len;
    uint8_t data[16];
  } inetcidrroutenexthop;
  bool has_inetcidrrouteifindex;
  int32_t inetcidrrouteifindex;
  bool has_inetcidrroutetype;
  uint32_t inetcidrroutetype;
  bool has_inetcidrrouteproto;
  uint32_t inetcidrrouteproto;
  bool has_inetcidrrouteage;
  uint32_t inetcidrrouteage;
};
#define IPROUTE_INIT \
 { 0,0, 0,0, 0,{0,{0}}, 0,0, 0,0, 0,{0,{0}}, 0,0, 0,0, 0,0, 0,0 }


// CURRENT_TIME
struct  _Current_Time
{
  bool has_posix;
  uint32_t posix;
  bool has_iso8601;
  char iso8601[20];
  bool has_source;
  uint32_t source;
};
#define CURRENT_TIME_INIT \
 {0,0, 0,{0}, 0,0 }


// UPTIME
struct  _Up_time
{
  bool has_sysuptime;
  uint32_t sysuptime;
};
#define UPTIME_INIT \
 { 0,0 }


// INTERFACE_METRICS
typedef enum {
  IF_ADMIN_STATUS_UP = 1,
  IF_ADMIN_STATUS_DOWN = 2,
  IF_ADMIN_STATUS_TESTING = 3
}admin_status_t;

typedef enum {
  IF_OPER_STATUS_UP = 1,
  IF_OPER_STATUS_DOWN = 2,
  IF_OPER_STATUS_TESTING = 3,
  IF_OPER_STATUS_UNKNOWN = 4,
  IF_OPER_STATUS_DORMANT = 5,
  IF_OPER_STATUS_NOT_PRESENT = 6,
  IF_OPER_STATUS_LOWER_LAYER_DOWN = 7,
  IF_OPER_STATUS_UP_SECURE = 8,
  IF_OPER_STATUS_UP_INSECURE = 9
}oper_status_t;

struct  _Interface_Metrics
{
  bool has_ifindex;
  int32_t ifindex;
  bool has_ifinspeed;
  uint32_t ifinspeed;
  bool has_ifoutspeed;
  uint32_t ifoutspeed;
  bool has_ifadminstatus;
  uint32_t ifadminstatus;  //ref to admin_status_t
  bool has_ifoperstatus;
  uint32_t ifoperstatus;   //ref to oper_status_t
  bool has_iflastchange;
  uint32_t iflastchange;
  bool has_ifinoctets;
  uint32_t ifinoctets;
  bool has_ifoutoctets;
  uint32_t ifoutoctets;
  bool has_ifindiscards;
  uint32_t ifindiscards;
  bool has_ifinerrors;
  uint32_t ifinerrors;
  bool has_ifoutdiscards;
  uint32_t ifoutdiscards;
  bool has_ifouterrors;
  uint32_t ifouterrors;
};
#define INTERFACE_METRICS_INIT \
 { 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 }


// IPROUTE_RPLMETRICS
struct  _IPRoute_RPLMetrics
{
  bool has_inetcidrrouteindex;
  int32_t inetcidrrouteindex;
  bool has_instanceindex;
  int32_t instanceindex;
  bool has_rank;
  int32_t rank;
  bool has_hops;
  int32_t hops;
  bool has_pathetx;
  int32_t pathetx;
  bool has_linketx;
  int32_t linketx;
  bool has_rssiforward;
  int32_t rssiforward;
  bool has_rssireverse;
  int32_t rssireverse;
  bool has_lqiforward;
  int32_t lqiforward;
  bool has_lqireverse;
  int32_t lqireverse;
  bool has_dagsize;
  uint32_t dagsize;
};
#define IPROUTE_RPLMETRICS_INIT \
 { 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 }


// WPAN_STATUS
typedef enum {
  IEEE154_SEC_NONE = 0,
  IEEE154_SEC_MIC_32 = 1,
  IEEE154_SEC_MIC_64 = 2,
  IEEE154_SEC_MIC_128 = 3,
  IEEE154_SEC_ENC = 4,
  IEEE154_SEC_ENC_MIC_32 = 5,
  IEEE154_SEC_ENC_MIC_64 = 6,
  IEEE154_SEC_ENC_MIC_128 = 7
}security_level_t;

typedef enum {
  IEEE154_PAN_LEAVE_INIT = 0,
  IEEE154_PAN_LEAVE_SYNC_TIMEOUT = 1,
  IEEE154_PAN_LEAVE_GTK_TIMEOUT = 2,
  IEEE154_PAN_LEAVE_NO_DEF_ROUTE = 3,
  IEEE154_PAN_LEAVE_OPTIMIZE = 4
}change_reason_t;

struct  _WPAN_Status
{
  bool has_ifindex;
  int32_t ifindex;
  bool has_ssid;
  struct {
    size_t len;
    uint8_t data[32];
  } ssid;
  bool has_panid;
  uint32_t panid;
  bool has_master;
  bool master;
  bool has_dot1xenabled;
  bool dot1xenabled;
  bool has_securitylevel;
  uint32_t securitylevel;       //ref to security_level_t
  bool has_rank;
  uint32_t rank;
  bool has_beaconvalid;
  bool beaconvalid;
  bool has_beaconversion;
  uint32_t beaconversion;
  bool has_beaconage;
  uint32_t beaconage;
  bool has_txpower;
  int32_t txpower;
  bool has_dagsize;
  uint32_t dagsize;
  bool has_metric;
  uint32_t metric;
  bool has_lastchanged;
  uint32_t lastchanged;
  bool has_lastchangedreason;
  uint32_t lastchangedreason;  //ref to change_reason_t
  bool has_demomodeenabled;
  bool demomodeenabled;
};
#define WPANSTATUS_INIT \
 { 0,0, 0,{0,{0}}, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 }


// NEIGHBOR
struct  _Neighbor_802154G
{
  bool has_nbrindex;
  int32_t nbrindex;
  bool has_physaddress;
  struct {
    size_t len;
    uint8_t data[8];
  } physaddress;
  bool has_lastchanged;
  uint32_t lastchanged;
  bool has_rssiforward;
  int32_t rssiforward;
  bool has_rssireverse;
  int32_t rssireverse;
  bool has_lqiforward;
  uint32_t lqiforward;
  bool has_lqireverse;
  uint32_t lqireverse;
};
#define NEIGHBOR802154_G_INIT \
 { 0,0, 0,{0,{0}}, 0,0, 0,0, 0,0, 0,0, 0,0 }


// RPL_INSTANCE
struct  _RPL_Instance
{
  bool has_instanceindex;
  int32_t instanceindex;
  bool has_instanceid;
  int32_t instanceid;
  bool has_dodagid;
  struct {
    size_t len;
    uint8_t data[16];
  } dodagid;
  bool has_dodagversionnumber;
  int32_t dodagversionnumber;
  bool has_rank;
  int32_t rank;
  bool has_parentcount;
  int32_t parentcount;
  bool has_dagsize;
  uint32_t dagsize;
};
#define RPLINSTANCE_INIT \
 { 0,0, 0,0, 0,{0,{0}}, 0,0, 0,0, 0,0, 0,0 }


struct  _Firmware_Image_Info
{
  bool has_index;
  uint32_t index;
  bool has_filehash;
  struct {
    size_t len;
    uint8_t data[32];
  } filehash;
  bool has_filename;
  char filename[128];
  bool has_version;
  char version[32];
  bool has_filesize;
  uint32_t filesize;
  bool has_blocksize;
  uint32_t blocksize;
  bool has_bitmap;
  struct {
    size_t len;
    uint8_t data[128];
  } bitmap;
  bool has_isdefault;
  bool isdefault;
  bool has_isrunning;
  bool isrunning;
  bool has_loadtime;
  uint32_t loadtime;
  bool has_hwinfo;
  struct {
    bool has_hwid;
    char hwid[32];
    bool has_vendorhwid;
    char vendorhwid[32];
  } hwinfo;
};
#define FIRMWARE_IMAGE_INFO_INIT \
   { 0,0, 0,{0,{0}}, 0,{0}, 0,{0}, 0,0, 0,0, 0,{0,{0}}, 0,0, 0,0, 0,0, 0,{0,{0}, 0,{0}} }
#endif
