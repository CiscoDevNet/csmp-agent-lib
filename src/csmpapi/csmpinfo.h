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
