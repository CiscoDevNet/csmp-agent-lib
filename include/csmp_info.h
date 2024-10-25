/*
 *  Copyright 2021-2024 Cisco Systems, Inc.
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
#include "iana_pen.h"

/** maximum CSMP cert length */
#define MAX_SIGNATURE_CERT_LENGTH 512

/* Max vendor data length */
#define VENDOR_MAX_DATA_LEN 32

/** \brief Max number of vendor subtypes */
#define VENDOR_MAX_SUBTYPES 5

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
typedef struct _Signature_Settings Signature_Settings; /**< data related to TLV 79 */
typedef struct _Vendor_Tlv Vendor_Tlv;                 /**< data related to TLV 127 */
typedef struct _Transfer_Request Transfer_Request;       /**< data related to TLV 65 */
typedef struct _Image_Block Image_Block;                 /**< data related to TLV 67 */
typedef struct _Load_Request Load_Request;               /**< data related to TLV 68 */
typedef struct _Cancel_Load_Request Cancel_Load_Request; /**< data related to TLV 69 */
typedef struct _Set_Backup_Request Set_Backup_Request;   /**< data related to TLV 70 */
typedef struct _Firmware_Image_Info Firmware_Image_Info; /**< data related to TLV 75 */

typedef struct _Csmp_Slothdr Csmp_Slothdr;               /**< firmware image slot */

// CSMP OP RETURN CODES
#define CSMP_OP_TLV_RD_EMPTY   0
#define CSMP_OP_TLV_RD_ERROR  -1
#define CSMP_OP_TLV_WR_ERROR  -2
#define CSMP_OP_UNSUPPORTED   -3
#define CSMP_OP_FAILED        -4

// CSMP RESPONSE CODES
enum {
  RESPONSE_OK = 0,
  RESPONSE_INCOMPATIBLE_HW = 1,
  RESPONSE_INCOMPLETE = 2,
  RESPONSE_UNKNOWN_HASH = 3,
  RESPONSE_FILE_SIZE_TOO_BIG = 4,
  RESPONSE_SIGNATURE_FAILED = 5,
  RESPONSE_INVALID_REQ = 6,
  RESPONSE_INVALID_BLOCK_SIZE = 7,
  RESPONSE_PENDING_REBOOT = 8,
  RESPONSE_IMAGE_RUNNING = 9,
  RESPONSE_NO_VOLUME = 10,
  RESPONSE_DUP_XFER = 11,
  RESPONSE_MATCH_RUN_XFER = 12,
  RESPONSE_MATCH_BAK_XFER = 13
};

// SIZES
#define SHA1_HASH_SIZE        20
#define SHA256_HASH_SIZE      32
#define FILE_NAME_SIZE        128
#define VERSION_SIZE          32
#define BITMAP_SIZE           32
#define HWID_SIZE             32
#define BLOCK_SIZE            1024

// IMAGE SLOT INFO
#define CSMP_FWMGMT_ACTIVE_SLOTS      3          // 0-RUN, 1-UPLOAD, 2-BACKUP
#define CSMP_FWMGMT_SLOTIMG_SIZE      (30*1024)  // ~30 Kb
#define CSMP_FWMGMT_BLKMAP_CNT        (32)

// IMAGE SLOT ID
enum {
  RUN_IMAGE = 0,
  UPLOAD_IMAGE = 1,
  BACKUP_IMAGE = 2,
  BL_IMAGE = 3,
  LMAC_IMAGE = 4,
  BBU_IMAGE = 5,
  PHY_IMAGE = 6,
  PATCH_IMAGE = 7,
  THIRDPARTY_IMAGE = 8,
  NUMSLOTS = 9
};

// FIRMWARE DOWNLOAD STATUS
enum {
  FWHDR_STATUS_COMPLETE = 0,
  FWHDR_STATUS_DOWNLOAD = 0xFFFFFFF0UL,
  FWHDR_STATUS_BADIMAGE = 0xFFFFFF00UL,
  FWHDR_STATUS_BADHASH  = 0xFFFFF000UL,
  FWHDR_STATUS_UNKNOWN  = 0xFFFFFFFFUL,
};

// TRANSFER-REQUEST REPORT INTERVAL DEFAULTS
// Atleast minimum interval of MIN_REPORT_MIN
// Atleast maximum interval of MAX_REPORT_MIN
enum {
  MIN_REPORT_MIN = 600 * 1000,
  MAX_REPORT_MIN = 1800 * 1000
};

enum {
  MIN_HASH_COMPARE_LEN = 4,
  MIN_LOAD_DELAY = 1000  // 1 sec
};

// HARDWARE_DESC
enum {
  FUNCTION_METER = 1,
  FUNCTION_RE = 2,
  FUNCTION_DAG = 3,
  FUNCTION_IOT = 4,
  FUNCTION_UNKNOWN = 99
};

typedef enum {
  CLASS_OTHER = 1,
  CLASS_UNKNOWN = 2,
  CLASS_CHASSIS = 3,
  CLASS_BACKPLANE = 4,
  CLASS_CONTAINER = 5,
  CLASS_POWRERSUPPLY = 6,
  CLASS_FAN = 7,
  CLASS_SENSOR = 8,
  CLASS_MODULE = 9,
  CLASS_PORT = 10,
  CLASS_STACK = 11,
  CLASS_CPU = 12
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

// SIGNATURE_SETTINGS
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

// TLV 127 VENDOR_TLVID
struct _Vendor_Tlv
{
  bool has_subtype; /* 'Y' */
  uint32_t subtype; /* 'Y' */
  bool has_value; /* 'Y' */
  struct {
    size_t len;
    uint8_t data[VENDOR_MAX_DATA_LEN];
  } value; /* 'Y' */
};
#define VENDOR_TLV_INIT \
   { 0,0, 0,{0,{0}} }

// TLV 65 TRANSFER_REQUEST_TLVID
// TLV 71 TRANSFER_RESPONSE_TLVID
struct _Transfer_Request
{
  bool has_hwinfo;/* 'Y' */
  struct {
    bool has_hwid;
    char hwid[HWID_SIZE];
    bool has_vendorhwid;/* 'Y' */
    char vendorhwid[HWID_SIZE];/* 'Y' */
  } hwinfo;/* 'Y' */
  bool has_filehash;/* 'Y' */
  struct {
    size_t len;
    uint8_t data[SHA256_HASH_SIZE];
  } filehash;/* 'Y' */
  bool has_filename;/* 'Y' */
  char filename[FILE_NAME_SIZE];/* 'Y' */
  bool has_version;/* 'Y' */
  char version[VERSION_SIZE];/* 'Y' */
  bool has_filesize;/* 'Y' */
  uint32_t filesize;/* 'Y' */
  bool has_blocksize;
  uint32_t blocksize;
  bool has_report_int_min;/* 'Y' */
  uint32_t report_int_min;/* 'Y' */
  bool has_report_int_max;/* 'Y' */
  uint32_t report_int_max;/* 'Y' */
  bool has_status;/* 'Y' */
  uint32_t status;/* 'Y' */
  bool has_response;/* 'Y' */
  uint32_t response;/* 'Y' */
};
#define TRANSFER_REQUEST_INIT \
  { 0, {0, {0}, 0, {0}}, 0, {0, {0}}, 0, {0}, 0, {0}, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0 }

// TLV 67 IMAGE_BLOCK_TLVID
struct _Image_Block
{
  bool has_filehash;/* Y */
  struct {
    size_t len;
    uint8_t data[SHA256_HASH_SIZE];
  } filehash;/* Y */
  bool retval;
  bool has_blocknum;/* Y */
  uint32_t blocknum;/* Y */
  bool has_blockdata;/* Y */
  struct {
    size_t len;
    uint8_t data[BLOCK_SIZE];
  } blockdata;/* Y */
};
#define IMAGE_BLOCK_INIT \
  { 0, {0, {0}}, 0, 0, 0, 0, {0, {0}} }


// TLV 68 LOAD_REQUEST_TLVID
// TLV 72 LOAD_RESPONSE_TLVID
struct _Load_Request
{
  bool has_filehash;/* Y */
  struct {
    size_t len;
    uint8_t data[SHA256_HASH_SIZE];
  } filehash;/* 'Y' */
  bool has_loadtime;/* Y */
  uint32_t loadtime;/* Y */
  bool has_response;/* 'Y' */
  uint32_t response;/* 'Y' */
};
#define LOAD_REQUEST_INIT \
  { 0, {0, {0}}, 0, 0, 0, 0 }


// TLV 69 CANCEL_LOAD_REQUEST_TLVID
// TLV 73 CANCEL_LOAD_RESPONSE_TLVID
struct _Cancel_Load_Request
{
  bool has_filehash;/* Y */
  struct {
    size_t len;
    uint8_t data[SHA256_HASH_SIZE];
  } filehash;/* 'Y' */
  bool has_loadtime;/* 'Y' */
  uint32_t loadtime;/* 'Y' */
  bool has_response;/* 'Y' */
  uint32_t response;/* 'Y' */
};
#define CANCEL_LOAD_REQUEST_INIT \
  { 0, {0, {0}}, 0, 0, 0, 0 }


// TLV 70 SET_BACKUP_REQUEST_TLVID
// TLV 74 SET_BACKUP_RESPONSE_TLVID
struct _Set_Backup_Request
{
  bool has_filehash;/* 'Y' */
  struct {
    size_t len;
    uint8_t data[SHA256_HASH_SIZE];
  } filehash;/* 'Y' */
  bool has_loadtime;/* 'Y' */
  uint32_t loadtime;/* 'Y' */
  bool has_response;/* 'Y' */
  uint32_t response;/* 'Y' */
};
#define SET_BACKUP_REQUEST_INIT \
  { 0, {0, {0}}, 0, 0, 0, 0 }


// TLV 75 FIRMWARE_IMAGE_INFO_TLVID
struct _Firmware_Image_Info
{
  bool has_index;/* 'Y' */
  uint32_t index;/* 'Y' */
  bool has_filehash;/* 'Y' */
  struct {
    size_t len;
    uint8_t data[SHA256_HASH_SIZE];
  } filehash;/* 'Y' */
  bool has_filename;/* 'Y' */
  char filename[FILE_NAME_SIZE];/* 'Y' */
  bool has_version;/* 'Y' */
  char version[VERSION_SIZE];/* 'Y' */
  bool has_filesize;/* 'Y' */
  uint32_t filesize;/* 'Y' */
  bool has_blocksize;
  uint32_t blocksize;
  uint32_t blockcnt;
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
    char hwid[HWID_SIZE];
    bool has_vendorhwid;/* 'Y' */
    char vendorhwid[HWID_SIZE];/* 'Y' */
  } hwinfo;/* 'Y' */
  bool has_kernelversion;
  char kernelversion[VERSION_SIZE];
  bool has_subkernelversion;
  char subkernelversion[VERSION_SIZE];
  bool has_loaderrorcode;
  uint32_t loaderrorcode;
  bool has_subloaderrorcode;
  uint32_t subloaderrorcode;
  uint32_t status;
};
#define FIRMWARE_IMAGE_INFO_INIT \
  { 0,0, 0,{0,{0}}, 0,{0}, 0,{0}, 0,0, 0,0, 0, 0,{0,{0}}, 0,0, 0,0, 0,0, 0,{0,{0}, \
  0,{0}}, 0,{0}, 0,{0}, 0,0, 0,0, 0 }
// Image app header
typedef struct {
  uint32_t hdr_version;
  uint32_t hdr_len;
  uint32_t app_rev_major;
  uint32_t app_rev_minor;
  uint32_t app_build;
  uint32_t app_len;  // Includes header, app and checksum
  char app_name[FILE_NAME_SIZE];
  char app_git_branch[32];
  char app_git_commit[8];
  uint32_t app_git_flag;
  char app_build_date[16];
  char hwid[HWID_SIZE];
} _Apphdr;

// Image slot header
struct _Csmp_Slothdr
{
  uint8_t filehash[SHA256_HASH_SIZE];
  char filename[FILE_NAME_SIZE];
  char version[VERSION_SIZE];
  char hwid[HWID_SIZE];
  uint32_t filesize;
  uint32_t filesizelastblk;
  uint32_t blockcnt;
  uint32_t blocksize;
  uint32_t reportintervalmin;
  uint32_t reportintervalmax;
  uint32_t status; // Boolean zero if image is complete
  uint32_t nblkmap[CSMP_FWMGMT_BLKMAP_CNT]; // Inverted block completion map
  uint32_t magicU;
  uint32_t magicL;
  // Image
  uint8_t image[CSMP_FWMGMT_SLOTIMG_SIZE];
};

#define APPHDR_INIT \
{0, 0, 0, 0, 0, 0, {0}, {0}, {0}, 0, {0}, {0}}

#define CSMP_SLOTHDR_INIT \
  {{0}, {0}, {0}, {0}, 0, 0, 0, 0, 0, 0, 0, {0}, \
    0, 0, {0}}

#endif // CSMP_INFO_H
