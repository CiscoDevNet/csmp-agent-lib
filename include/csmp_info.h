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

#ifndef _CSMP_INFO_H
#define _CSMP_INFO_H

/*! \file
 *
 * CSMP info
 *
 * This file contains the structures that represents the TLV descriptions.
 * For each TLV a struct is defined.
 * Each entry in the TLV data will contain
 * - the data itself
 * - boolean indicator if the data is actual available for this instance on the device.
 *
 * These structures are used to encode to the TLV in protobuf format, e.g. used to set data to be communicated outside the device
 * Also these structures are used to decode the incoming data in protocol buf format.
 *
 * On application level, the structures are used by global variables.
 * The global variable are set by functions on a GET request by the NMS.
 * Also the global variable are used by a POST request, then the data incoming request data is set in the global variable.
 */

#include <stdbool.h>
#include <stdint.h>

/** maximum CSMP cert length */
#define MAX_SIGNATURE_CERT_LENGTH 512

// structs for each tlv
/*****************************************************************************
  the member marked by 'Y' in each structure is suggested to be filled
  with effective value, otherwise the NMS may not handle the TLV correctly
*************************************************************************/
typedef struct _Hardware_Desc Hardware_Desc;           /**< data related to TLV 11 */
typedef struct _Interface_Desc Interface_Desc;         /**< data related to TLV 12 */
typedef struct _IP_Address IP_Address;                 /**< data related to TLV 16 */
typedef struct _IP_Route IP_Route;                     /**< data related to TLV 17 */
typedef struct _Current_Time Current_Time;             /**< data related to TLV 18 */
typedef struct _Up_time Up_Time;                       /**< data related to TLV 22 */
typedef struct _Interface_Metrics Interface_Metrics;   /**< data related to TLV 23 */
typedef struct _IPRoute_RPLMetrics IPRoute_RPLMetrics; /**< data related to TLV 25 */
typedef struct _WPAN_Status WPAN_Status;               /**< data related to TLV 35 */
typedef struct _RPL_Instance RPL_Instance;             /**< data related to TLV 53 */
typedef struct _Firmware_Image_Info Firmware_Image_Info; /**< data related to TLV 75 */
typedef struct _Signature_Settings Signature_Settings; /**< data related to TLV 79 */
typedef struct _Vendor_Specific Vendor_Specific;       /**< data related to TLV 127 */

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
  bool has_entphysicalindex;/**< 'Y' contains the entphysicalindex */
  int32_t entphysicalindex;/* 'Y' */
  bool has_entphysicaldescr;/**< 'Y' contains the entphysicaldescr */
  char entphysicaldescr[64];/**< 'Y' A textual description of physical entity */
  bool has_entphysicalvendortype;
  struct {
    size_t len;
    uint8_t data[16];
  } entphysicalvendortype;
  bool has_entphysicalcontainedin;
  int32_t entphysicalcontainedin;
  bool has_entphysicalclass;/* 'Y' */
  int32_t entphysicalclass;/* 'Y' */  //ref to physical_class_t
  bool has_entphysicalparentrelpos;
  int32_t entphysicalparentrelpos;
  bool has_entphysicalname;/* 'Y' */
  char entphysicalname[32];/* 'Y' */
  bool has_entphysicalhardwarerev;/* 'Y' */
  char entphysicalhardwarerev[16];/* 'Y' */
  bool has_entphysicalfirmwarerev;/* 'Y' */
  char entphysicalfirmwarerev[16];/* 'Y' */
  bool has_entphysicalsoftwarerev;
  char entphysicalsoftwarerev[16];
  bool has_entphysicalserialnum;/* 'Y' */
  char entphysicalserialnum[18];/* 'Y' */
  bool has_entphysicalmfgname;/* 'Y' */
  char entphysicalmfgname[32];/* 'Y' */
  bool has_entphysicalmodelname;/* 'Y' */
  char entphysicalmodelname[20];/* 'Y' */
  bool has_entphysicalassetid;
  char entphysicalassetid[32];
  bool has_entphysicalmfgdate;
  uint32_t entphysicalmfgdate;
  bool has_entphysicaluris;
  char entphysicaluris[64];
  bool has_entphysicalfunction;/* 'Y' */
  uint32_t entphysicalfunction;/* 'Y' */
  bool has_entphysicaloui;
  char entphysicaloui[8];
};
#define HARDWARE_DESC_INIT \
 { 0,0, 0,{0}, 0,{0,{0}}, 0,0, 0,0, 0,0, 0,{0}, 0,{0}, 0,{0}, 0,{0}, 0,{0}, 0,{0}, 0,{0}, 0,{0}, 0,0, 0,{0}, 0,0, 0,{0} }


// INTERFACE_DESC
struct  _Interface_Desc
{
  bool has_ifindex;/* 'Y' */
  int32_t ifindex;/* 'Y' */
  bool has_ifname;/* 'Y' */
  char ifname[20];/* 'Y' */
  bool has_ifdescr;/* 'Y' */
  char ifdescr[32];/* 'Y' */
  bool has_iftype;/* 'Y' */
  int32_t iftype;/* 'Y' */
  bool has_ifmtu;
  int32_t ifmtu;
  bool has_ifphysaddress;/* 'Y' */
  struct {
    size_t len;
    uint8_t data[8];
  } ifphysaddress;/* 'Y' */
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
  bool has_ipaddressindex;/* 'Y' */
  int32_t ipaddressindex;/* 'Y' */
  bool has_ipaddressaddrtype;/* 'Y' */
  uint32_t ipaddressaddrtype;/* 'Y' */  //ref to ipaddr_type_t
  bool has_ipaddressaddr;/* 'Y' */
  struct {
    size_t len;
    uint8_t data[16];
  } ipaddressaddr;/* 'Y' */
  bool has_ipaddressifindex;/* 'Y' */
  int32_t ipaddressifindex;/* 'Y' */
  bool has_ipaddresstype;/* 'Y' */
  uint32_t ipaddresstype;/* 'Y' */    //ref to ip_type_t
  bool has_ipaddressorigin;/* 'Y' */
  uint32_t ipaddressorigin;/* 'Y' */  //ref to ip_origin_t
  bool has_ipaddressstatus;/* 'Y' */
  uint32_t ipaddressstatus;/* 'Y' */
  bool has_ipaddresscreated;
  uint32_t ipaddresscreated;
  bool has_ipaddresslastchanged;
  uint32_t ipaddresslastchanged;
  bool has_ipaddresspfxlen;/* 'Y' */
  uint32_t ipaddresspfxlen;/* 'Y' */
};
#define IPADDRESS_INIT \
 { 0,0, 0,0, 0,{0,{0}}, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 }


// IPROUTE
struct  _IP_Route
{
  bool has_inetcidrrouteindex;/* 'Y' */
  int32_t inetcidrrouteindex;/* 'Y' */
  bool has_inetcidrroutedesttype;/* 'Y' */
  uint32_t inetcidrroutedesttype;/* 'Y' */
  bool has_inetcidrroutedest;/* 'Y' */
  struct {
    size_t len;
    uint8_t data[16];
  } inetcidrroutedest;/* 'Y' */
  bool has_inetcidrroutepfxlen;/* 'Y' */
  uint32_t inetcidrroutepfxlen;/* 'Y' */
  bool has_inetcidrroutenexthoptype;/* 'Y' */
  uint32_t inetcidrroutenexthoptype;/* 'Y' */
  bool has_inetcidrroutenexthop;/* 'Y' */
  struct {
    size_t len;
    uint8_t data[16];
  } inetcidrroutenexthop;/* 'Y' */
  bool has_inetcidrrouteifindex;/* 'Y' */
  int32_t inetcidrrouteifindex;/* 'Y' */
  bool has_inetcidrroutetype;/* 'Y' */
  uint32_t inetcidrroutetype;/* 'Y' */
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
  bool has_posix;/* 'Y' */
  uint32_t posix;/* 'Y' */
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
  bool has_sysuptime;/* 'Y' */
  uint32_t sysuptime;/* 'Y' */
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
  bool has_ifindex;/* 'Y' */
  int32_t ifindex;/* 'Y' */
  bool has_ifinspeed;
  uint32_t ifinspeed;
  bool has_ifoutspeed;
  uint32_t ifoutspeed;
  bool has_ifadminstatus;/* 'Y' */
  uint32_t ifadminstatus;/* 'Y' */  //ref to admin_status_t
  bool has_ifoperstatus;/* 'Y' */
  uint32_t ifoperstatus;/* 'Y' */   //ref to oper_status_t
  bool has_iflastchange;/* 'Y' */
  uint32_t iflastchange;/* 'Y' */
  bool has_ifinoctets;/* 'Y' */
  uint32_t ifinoctets;/* 'Y' */
  bool has_ifoutoctets;/* 'Y' */
  uint32_t ifoutoctets;/* 'Y' */
  bool has_ifindiscards;/* 'Y' */
  uint32_t ifindiscards;/* 'Y' */
  bool has_ifinerrors;/* 'Y' */
  uint32_t ifinerrors;/* 'Y' */
  bool has_ifoutdiscards;/* 'Y' */
  uint32_t ifoutdiscards;/* 'Y' */
  bool has_ifouterrors;/* 'Y' */
  uint32_t ifouterrors;/* 'Y' */
};
#define INTERFACE_METRICS_INIT \
 { 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 }


// IPROUTE_RPLMETRICS
struct  _IPRoute_RPLMetrics
{
  bool has_inetcidrrouteindex;/* 'Y' */
  int32_t inetcidrrouteindex;/* 'Y' */
  bool has_instanceindex;/* 'Y' */
  int32_t instanceindex;/* 'Y' */
  bool has_rank;/* 'Y' */
  int32_t rank;/* 'Y' */
  bool has_hops;/* 'Y' */
  int32_t hops;/* 'Y' */
  bool has_pathetx;/* 'Y' */
  int32_t pathetx;/* 'Y' */
  bool has_linketx;/* 'Y' */
  int32_t linketx;/* 'Y' */
  bool has_rssiforward;/* 'Y' */
  int32_t rssiforward;/* 'Y' */
  bool has_rssireverse;/* 'Y' */
  int32_t rssireverse;/* 'Y' */
  bool has_lqiforward;/* 'Y' */
  int32_t lqiforward;/* 'Y' */
  bool has_lqireverse;/* 'Y' */
  int32_t lqireverse;/* 'Y' */
  bool has_dagsize;/* 'Y' */
  uint32_t dagsize;/* 'Y' */
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
  bool has_ifindex;/* 'Y' */
  int32_t ifindex;/* 'Y' */
  bool has_ssid;/* 'Y' */
  struct {
    size_t len;
    uint8_t data[32];
  } ssid;/* 'Y' */
  bool has_panid;/* 'Y' */
  uint32_t panid;/* 'Y' */
  bool has_master;/* 'Y' */
  bool master;/* 'Y' */
  bool has_dot1xenabled;/* 'Y' */
  bool dot1xenabled;/* 'Y' */
  bool has_securitylevel;/* 'Y' */
  uint32_t securitylevel;/* 'Y' */       //ref to security_level_t
  bool has_rank;/* 'Y' */
  uint32_t rank;/* 'Y' */
  bool has_beaconvalid;/* 'Y' */
  bool beaconvalid;/* 'Y' */
  bool has_beaconversion;/* 'Y' */
  uint32_t beaconversion;/* 'Y' */
  bool has_beaconage;/* 'Y' */
  uint32_t beaconage;/* 'Y' */
  bool has_txpower;/* 'Y' */
  int32_t txpower;/* 'Y' */
  bool has_dagsize;/* 'Y' */
  uint32_t dagsize;/* 'Y' */
  bool has_metric;/* 'Y' */
  uint32_t metric;/* 'Y' */
  bool has_lastchanged;/* 'Y' */
  uint32_t lastchanged;/* 'Y' */
  bool has_lastchangedreason;/* 'Y' */
  uint32_t lastchangedreason;/* 'Y' */  //ref to change_reason_t
  bool has_demomodeenabled;
  bool demomodeenabled;
};
#define WPANSTATUS_INIT \
 { 0,0, 0,{0,{0}}, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 }


// RPL_INSTANCE
struct  _RPL_Instance
{
  bool has_instanceindex;/* 'Y' */
  int32_t instanceindex;/* 'Y' */
  bool has_instanceid;/* 'Y' */
  int32_t instanceid;/* 'Y' */
  bool has_dodagid;/* 'Y' */
  struct {
    size_t len;
    uint8_t data[16];
  } dodagid;/* 'Y' */
  bool has_dodagversionnumber;/* 'Y' */
  int32_t dodagversionnumber;/* 'Y' */
  bool has_rank;/* 'Y' */
  int32_t rank;/* 'Y' */
  bool has_parentcount;/* 'Y' */
  int32_t parentcount;/* 'Y' */
  bool has_dagsize;/* 'Y' */
  uint32_t dagsize;/* 'Y' */
};
#define RPLINSTANCE_INIT \
 { 0,0, 0,0, 0,{0,{0}}, 0,0, 0,0, 0,0, 0,0 }


struct  _Firmware_Image_Info
{
  bool has_index;/* 'Y' */
  uint32_t index;/* 'Y' */
  bool has_filehash;/* 'Y' */
  struct {
    size_t len;
    uint8_t data[32];
  } filehash;/* 'Y' */
  bool has_filename;/* 'Y' */
  char filename[128];/* 'Y' */
  bool has_version;/* 'Y' */
  char version[32];/* 'Y' */
  bool has_filesize;/* 'Y' */
  uint32_t filesize;/* 'Y' */
  bool has_blocksize;
  uint32_t blocksize;
  bool has_bitmap;
  struct {
    size_t len;
    uint8_t data[128];
  } bitmap;
  bool has_isdefault;/* 'Y' */
  bool isdefault;/* 'Y' */
  bool has_isrunning;/* 'Y' */
  bool isrunning;/* 'Y' */
  bool has_loadtime;
  uint32_t loadtime;
  bool has_hwinfo;/* 'Y' */
  struct {
    bool has_hwid;
    char hwid[32];
    bool has_vendorhwid;/* 'Y' */
    char vendorhwid[32];/* 'Y' */
  } hwinfo;/* 'Y' */
};
#define FIRMWARE_IMAGE_INFO_INIT \
   { 0,0, 0,{0,{0}}, 0,{0}, 0,{0}, 0,0, 0,0, 0,{0,{0}}, 0,0, 0,0, 0,0, 0,{0,{0}, 0,{0}} }

struct _Signature_Settings
{
    bool has_reqsignedpost;
    bool reqsignedpost;
    bool has_reqvalidcheckpost;
    bool reqvalidcheckpost;
    bool has_reqtimesyncpost;
    bool reqtimesyncpost;
    bool has_reqseclocalpost;
    bool reqseclocalpost;
    bool has_reqsignedresp;
    bool reqsignedresp;
    bool has_reqvalidcheckresp;
    bool reqvalidcheckresp;
    bool has_reqtimesyncresp;
    bool reqtimesyncresp;
    bool has_reqseclocalresp;
    bool reqseclocalresp;
    bool has_cert;
    struct {
      size_t len;
      uint8_t data[MAX_SIGNATURE_CERT_LENGTH];
    } cert;
};

#define SIGNATURE_SETTINGS_INIT \
   { 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,{0,{0}} }

// VENDOR
struct  _Vendor_Specific
{
  bool has_data;
  struct {
    size_t len;
    uint8_t data[64];
  } data;
};
#define VENDOR_INIT \
 { 0,{0,{0}} }
#endif
