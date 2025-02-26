/*
 *  Copyright 2021-2025 Cisco Systems, Inc.
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

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "CsmpAgentLib_sample_tlvs.h"
#include "CsmpAgentLib_sample.h"
#include "csmp_service.h"
#include "csmp_info.h"
#include "signature_verify.h"
#include "osal.h"


#define nexthop_IP "fe80::a00:27ff:fe3b:2ab1"

/** \brief the hardware information */
Hardware_Desc g_hardwareDesc = HARDWARE_DESC_INIT;

/** \brief the interface information */
Interface_Desc g_interfaceDesc[interface_max_num] = {INTERFACE_DESC_INIT};

/** \brief the ipaddress information */
IP_Address g_ipAddress[ipaddress_max_num] = {IPADDRESS_INIT};

/** \brief the ip route information */
IP_Route g_ipRoute = IPROUTE_INIT;

/** \brief the current information */
Current_Time g_currentTime = CURRENT_TIME_INIT;

/** \brief the up time information */
Up_Time g_upTime = UPTIME_INIT;

/** \brief the interface metrics data */
Interface_Metrics g_interfaceMetrics[interface_max_num] = {INTERFACE_METRICS_INIT};

/** \brief the ip route rpl metrics data */
IPRoute_RPLMetrics g_iprouteRplmetrics = IPROUTE_RPLMETRICS_INIT;

/** \brief the wpan status data */
WPAN_Status g_wpanStatus = WPANSTATUS_INIT;

/** \brief the rpl data */
RPL_Instance g_rplInstance = RPLINSTANCE_INIT;

/** \brief the transfer requst data */
Transfer_Request g_transferRequest = TRANSFER_REQUEST_INIT;

/** \brief the image block data */
Image_Block g_imageBlock = IMAGE_BLOCK_INIT;

/** \brief the load request data */
Load_Request g_loadRequest = LOAD_REQUEST_INIT;

/** \brief the cancel load request data */
Cancel_Load_Request g_cancelLoadRequest = CANCEL_LOAD_REQUEST_INIT;

/** \brief the set backup request data */
Set_Backup_Request setBackupRequest = SET_BACKUP_REQUEST_INIT;

/** \brief the firmware info data */
Firmware_Image_Info g_firmwareImageInfo[OSAL_CSMP_FWMGMT_ACTIVE_SLOTS] = {FIRMWARE_IMAGE_INFO_INIT};

/** \brief the signature settings data */
Signature_Settings g_SignatureSettings = SIGNATURE_SETTINGS_INIT;

/** \brief Vendor tlv data */
Vendor_Tlv g_vendorTlv[VENDOR_MAX_SUBTYPES] = {VENDOR_TLV_INIT};

/** \brief eui64, should come from the HW*/
uint8_t g_eui64[8] = {0x0a, 0x00, 0x27, 0xff, 0xfe, 0x3b, 0x2a, 0xb2};

/** \brief Agent IPv6 address*/
char g_ipv6[46]="1:1:1::202";

char *SSID = "CISCO";
char vendorhwid[32] = "OPENCSMP";
uint32_t g_init_time;
bool g_reboot_request = false; //Flag to track ongoing reboot request
uint8_t neighbor_eui64[2][8] = {{0x0a, 0x00, 0x27, 0xff, 0xfe, 0x3b, 0x2a, 0xb1},
                             {0x0a, 0x00, 0x27, 0xff, 0xfe, 0x3b, 0x2a, 0xb0}};
dev_config_t g_devconfig;
csmp_handle_t g_csmp_handle;

// Firmware upgrade globals
bool g_downloadbusy       = false;  // Track ongoing download
bool g_initxfer           = false;  // Track transfer request
bool g_initload           = false;  // Track load request
uint32_t g_curloadtime    = 0;      // Track current loadtime
uint32_t g_curloadslot    = 0xFFU;  // Track current load slot
uint32_t g_curbackupslot  = 0xFFU;  // Track current backup slot

// Firmware image slots (Slot-id: 0-RUN, 1-UPLOAD, 2-BACKUP)
osal_csmp_slothdr_t g_slothdr[OSAL_CSMP_FWMGMT_ACTIVE_SLOTS] = {0};
FILE *upload_slot = NULL;
/* public key */
//new key
static const char pubkey[PUBLIC_KEY_LEN] = {
  0x04, 0x23, 0xD2, 0x83, 0x45, 0xE8, 0xD5, 0xDF, 0x86, 0x9D,
  0x6E, 0xE7, 0x58, 0x0D, 0xC1, 0x8F, 0x35, 0x9D, 0x57, 0xB1,
  0x3D, 0x50, 0x4A, 0x16, 0x01, 0x15, 0xC4, 0x81, 0x19, 0xB0,
  0xE6, 0x60, 0xB8, 0x64, 0x14, 0x01, 0x5D, 0x56, 0x83, 0xBE,
  0xE1, 0x85, 0x98, 0xCB, 0x90, 0xE1, 0xF7, 0x9B, 0xF4, 0x33,
  0x5A, 0x4B, 0x29, 0xAD, 0x35, 0x69, 0x9B, 0x4F, 0xDC, 0x42,
  0x7F, 0xEB, 0xC2, 0x99, 0xA5
};

/**
 * @brief pubkey_get
 *
 * @param key pointer to pubkey
 */
void pubkey_get(char *key) {
  strncpy(key, pubkey, PUBLIC_KEY_LEN);
  key[PUBLIC_KEY_LEN] = '\0';
}

/**
 * @brief hardware description function
 *
 * @param num amount of instances (array index) of hardware Descriptions
 * @return void* pointer to global variable g_hardwareDesc
 */
void* hardware_desc_get(uint32_t *num) {
  *num = 1;
  g_hardwareDesc.has_entphysicalindex = true;
  g_hardwareDesc.has_entphysicaldescr = true;
  g_hardwareDesc.has_entphysicalclass = true;
  g_hardwareDesc.has_entphysicalname = true;
  g_hardwareDesc.has_entphysicalhardwarerev = true;
  g_hardwareDesc.has_entphysicalfirmwarerev = true;
  g_hardwareDesc.has_entphysicalserialnum = true;
  g_hardwareDesc.has_entphysicalmfgname = true;
  g_hardwareDesc.has_entphysicalmodelname = true;
  g_hardwareDesc.has_entphysicalfunction = true;

  switch (DEVICE_TYPE) {
    case OPENCSMP:
          DPRINTF("sample_hw_desc: DEVICE_TYPE = OPENCSMP\n");
    g_hardwareDesc.entphysicalindex = 1;
    sprintf(g_hardwareDesc.entphysicaldescr,"CSMP-Agent Library");
    g_hardwareDesc.entphysicalclass = CLASS_MODULE;
    sprintf(g_hardwareDesc.entphysicalname,"lowpan");
    sprintf(g_hardwareDesc.entphysicalhardwarerev,"1.0");
    memcpy(g_hardwareDesc.entphysicalfirmwarerev, g_slothdr[RUN_IMAGE].version, sizeof(g_hardwareDesc.entphysicalfirmwarerev));
    snprintf(g_hardwareDesc.entphysicalserialnum,sizeof(g_hardwareDesc.entphysicalserialnum),
             "%02X%02X%02X%02X%02X%02X%02X%02X",
             g_eui64[0],g_eui64[1],g_eui64[2],g_eui64[3],
             g_eui64[4],g_eui64[5],g_eui64[6],g_eui64[7]);
    sprintf(g_hardwareDesc.entphysicalmfgname,"Cisco IIoT");
    sprintf(g_hardwareDesc.entphysicalmodelname,"OPENCSMP");
    g_hardwareDesc.entphysicalfunction = FUNCTION_METER; // Meter
    break;

    case CISCO_IR510:
    DPRINTF("sample_hw_desc: DEVICE_TYPE = CISCO_IR510\n");
    g_hardwareDesc.entphysicalindex = 1;
    sprintf(g_hardwareDesc.entphysicaldescr,"Cisco IR510 Gateway");
    g_hardwareDesc.entphysicalclass = CLASS_MODULE;
    sprintf(g_hardwareDesc.entphysicalname,"lowpan");
    sprintf(g_hardwareDesc.entphysicalhardwarerev,"1.0");
    memcpy(g_hardwareDesc.entphysicalfirmwarerev, g_slothdr[RUN_IMAGE].version, sizeof(g_hardwareDesc.entphysicalfirmwarerev));
    snprintf(g_hardwareDesc.entphysicalserialnum,sizeof(g_hardwareDesc.entphysicalserialnum),
             "%02X%02X%02X%02X%02X%02X%02X%02X",
             g_eui64[0],g_eui64[1],g_eui64[2],g_eui64[3],
             g_eui64[4],g_eui64[5],g_eui64[6],g_eui64[7]);
        sprintf(g_hardwareDesc.entphysicalmfgname,"Cisco IIoT");
    sprintf(g_hardwareDesc.entphysicalmodelname,"CISCO_IR510");
    g_hardwareDesc.entphysicalfunction = FUNCTION_DAG; // Gateway
    break;

    case ITRON_METER:
    DPRINTF("sample_hw_desc: DEVICE_TYPE = ITRON_METER\n");
    g_hardwareDesc.entphysicalindex = 1;
    sprintf(g_hardwareDesc.entphysicaldescr,"Itron OWCM");
    g_hardwareDesc.entphysicalclass = CLASS_MODULE;
    sprintf(g_hardwareDesc.entphysicalname,"lowpan");
    sprintf(g_hardwareDesc.entphysicalhardwarerev,"3.1");
    memcpy(g_hardwareDesc.entphysicalfirmwarerev, g_slothdr[RUN_IMAGE].version, sizeof(g_hardwareDesc.entphysicalfirmwarerev));
    snprintf(g_hardwareDesc.entphysicalserialnum,sizeof(g_hardwareDesc.entphysicalserialnum),
             "%02X%02X%02X%02X%02X%02X%02X%02X",
             g_eui64[0],g_eui64[1],g_eui64[2],g_eui64[3],
             g_eui64[4],g_eui64[5],g_eui64[6],g_eui64[7]);
    sprintf(g_hardwareDesc.entphysicalmfgname,"Itron");
    sprintf(g_hardwareDesc.entphysicalmodelname,"ITRON_METER");
    g_hardwareDesc.entphysicalfunction = FUNCTION_METER; // Meter
    break;

    default: // GENERIC_HW: 3rd party generic hardware
    DPRINTF("sample_hw_desc: DEVICE_TYPE = GENERIC_HW\n");
  }

  return &g_hardwareDesc;
}

/**
 * @brief interface description function
 *
 * @param num  amount of instances (array index) of interface Descriptions
 * @return void* pointer to global variable g_interfaceDesc
 */
void* interface_desc_get(uint32_t *num) {
  *num = 2;
  g_interfaceDesc[0].has_ifindex = true;
  g_interfaceDesc[0].has_ifname = true;
  g_interfaceDesc[0].has_ifdescr = true;
  g_interfaceDesc[0].has_iftype = true;

  g_interfaceDesc[0].ifindex = 1;
  sprintf(g_interfaceDesc[0].ifname,"lo");
  sprintf(g_interfaceDesc[0].ifdescr,"Loopback");
  g_interfaceDesc[0].iftype = 24;

  g_interfaceDesc[1].has_ifindex = true;
  g_interfaceDesc[1].has_ifname = true;
  g_interfaceDesc[1].has_ifdescr = true;
  g_interfaceDesc[1].has_iftype = true;
  g_interfaceDesc[1].has_ifphysaddress = true;

  g_interfaceDesc[1].ifindex = 2;
  sprintf(g_interfaceDesc[1].ifname,"lowpan");
  sprintf(g_interfaceDesc[1].ifdescr,"Ieee154");
  g_interfaceDesc[1].iftype = 259;
  g_interfaceDesc[1].ifphysaddress.len = 8;
  memcpy(g_interfaceDesc[1].ifphysaddress.data, g_eui64, sizeof(g_eui64));

  return &g_interfaceDesc;
}

/**
 * @brief function to set the ip Addresses.
 *
 * @param num amount of instances (array index) of ipAddresses
 * @return void* pointer to global variable g_ipAddress
 */
void* ipaddress_get(uint32_t *num) {
  int i;

  *num = 3;
  for(i=0;i<3;i++) {
    g_ipAddress[i].has_ipaddressindex = true;
    g_ipAddress[i].has_ipaddressaddrtype = true;
    g_ipAddress[i].has_ipaddressaddr = true;
    g_ipAddress[i].has_ipaddressifindex = true;
    g_ipAddress[i].has_ipaddresstype = true;
    g_ipAddress[i].has_ipaddressorigin = true;
    g_ipAddress[i].has_ipaddressstatus = true;
    g_ipAddress[i].has_ipaddresspfxlen = true;

    g_ipAddress[i].ipaddressindex = i+1;
  }

  g_ipAddress[0].ipaddressaddrtype = IPV6;
  g_ipAddress[0].ipaddressaddr.len = 16;
  osal_inet_pton(AF_INET6, "0::1", &g_ipAddress[0].ipaddressaddr.data);
  g_ipAddress[0].ipaddressifindex = 1;
  g_ipAddress[0].ipaddresstype = UNICAST;
  g_ipAddress[0].ipaddressorigin = DHCP;
  g_ipAddress[0].ipaddressstatus = true;
  g_ipAddress[0].ipaddresspfxlen = 128;

  g_ipAddress[1].ipaddressaddrtype = IPV6;
  g_ipAddress[1].ipaddressaddr.len = 16;
  osal_inet_pton(AF_INET6, "fe80::207:8109:dc:c8d", &g_ipAddress[1].ipaddressaddr.data);
  g_ipAddress[1].ipaddressifindex = 2;
  g_ipAddress[1].ipaddresstype = UNICAST;
  g_ipAddress[1].ipaddressorigin = LINKLAYER;
  g_ipAddress[1].ipaddressstatus = true;
  g_ipAddress[1].ipaddresspfxlen = 128;

  g_ipAddress[2].ipaddressaddrtype = IPV6;
  g_ipAddress[2].ipaddressaddr.len = 16;
  osal_inet_pton(AF_INET6, g_ipv6, &g_ipAddress[2].ipaddressaddr.data);
  g_ipAddress[2].ipaddressifindex = 2;
  g_ipAddress[2].ipaddresstype = UNICAST;
  g_ipAddress[2].ipaddressorigin = DHCP;
  g_ipAddress[2].ipaddressstatus = true;
  g_ipAddress[2].ipaddresspfxlen = 64;

  return &g_ipAddress;
}

/**
 * @brief set up ip Route information
 *
 * @param num amount of instances of g_ipRoute
 * @return void* pointer to g_ipRoute
 */
void* iproute_get(uint32_t *num) {
  *num = 1;
  g_ipRoute.has_inetcidrrouteindex = true;
  g_ipRoute.has_inetcidrroutedesttype = true;
  g_ipRoute.has_inetcidrroutedest = true;
  g_ipRoute.has_inetcidrroutepfxlen = true;
  g_ipRoute.has_inetcidrroutenexthoptype = true;
  g_ipRoute.has_inetcidrroutenexthop = true;
  g_ipRoute.has_inetcidrrouteifindex = true;

  g_ipRoute.inetcidrrouteindex = 1;
  g_ipRoute.inetcidrroutedesttype = IPV6;
  g_ipRoute.inetcidrroutedest.len = 16;
  osal_inet_pton(AF_INET6, "0::0", &g_ipRoute.inetcidrroutedest.data);
  g_ipRoute.inetcidrroutepfxlen = 0;
  g_ipRoute.inetcidrroutenexthoptype = IPV6Z;
  g_ipRoute.inetcidrroutenexthop.len = 16;
  osal_inet_pton(AF_INET6, nexthop_IP, &g_ipRoute.inetcidrroutenexthop.data);
  g_ipRoute.inetcidrrouteifindex = 2;
  return &g_ipRoute;
}

/**
 * @brief set up the current time
 *
 * @param num amount of instances
 * @return void* pointer to g_currentTime
 */
void* currenttime_get(uint32_t *num) {
  struct timeval tv = {0};

  memset(&g_currentTime, 0, sizeof(g_currentTime));

  *num = 1;
  osal_gettime(&tv, NULL);
  g_currentTime.has_posix = true;
  g_currentTime.posix = tv.tv_sec;
  return &g_currentTime;
}

/**
 * @brief set the current time
 *
 * @param tlv
 */
void currenttime_post(Current_Time *tlv) {
  struct timeval tv = {0};
  if(tlv->has_posix) {
    tv.tv_sec = tlv->posix;
    osal_settime(&tv, NULL);
  }
}

/**
 * @brief Get the uptime
 *
 * @param num amount of instances in g_uptime
 * @return void* point to g_uptime
 */
void* uptime_get(uint32_t *num) {
  struct timeval tv = {0};

  memset(&g_upTime, 0, sizeof(g_upTime));

  *num = 1;
  osal_gettime(&tv, NULL);
  g_upTime.has_sysuptime = true;
  g_upTime.sysuptime = tv.tv_sec - g_init_time;
  return &g_upTime;
}

/**
 * @brief interface metrics information setup
 *
 * @param num num mount of instances of g_interfaceMetrics
 * @return void* pointer to global g_interfaceMetrics
 */
void* interface_metrics_get(uint32_t *num) {
  static int outnum = 0;
  static int innum = 0;
  int i;

  outnum += 1320;
  innum += 610;

  *num = 2;
  for(i=0;i<2;i++) {
    g_interfaceMetrics[i].has_ifindex = true;
    g_interfaceMetrics[i].has_ifadminstatus = true;
    g_interfaceMetrics[i].has_ifoperstatus = true;
    g_interfaceMetrics[i].has_iflastchange = true;
    g_interfaceMetrics[i].has_ifinoctets = true;
    g_interfaceMetrics[i].has_ifoutoctets = true;
    g_interfaceMetrics[i].has_ifindiscards = true;
    g_interfaceMetrics[i].has_ifinerrors = true;
    g_interfaceMetrics[i].has_ifoutdiscards = true;
    g_interfaceMetrics[i].has_ifouterrors = true;

    g_interfaceMetrics[i].ifindex = i+1;
  }
  g_interfaceMetrics[0].ifadminstatus = IF_ADMIN_STATUS_UP;
  g_interfaceMetrics[0].ifoperstatus = IF_OPER_STATUS_UP;
  g_interfaceMetrics[0].iflastchange = 0;
  g_interfaceMetrics[0].ifinoctets = 0;
  g_interfaceMetrics[0].ifoutoctets = 0;
  g_interfaceMetrics[0].ifindiscards = 0;
  g_interfaceMetrics[0].ifinerrors = 0;
  g_interfaceMetrics[0].ifoutdiscards = 0;
  g_interfaceMetrics[0].ifouterrors = 0;

  g_interfaceMetrics[1].ifadminstatus = IF_ADMIN_STATUS_UP;
  g_interfaceMetrics[1].ifoperstatus = IF_OPER_STATUS_UP_INSECURE;
  g_interfaceMetrics[1].iflastchange = 10;
  g_interfaceMetrics[1].ifinoctets = outnum;
  g_interfaceMetrics[1].ifoutoctets = innum;
  g_interfaceMetrics[1].ifindiscards = 23;
  g_interfaceMetrics[1].ifinerrors = 0;
  g_interfaceMetrics[1].ifoutdiscards = 0;
  g_interfaceMetrics[1].ifouterrors = 0;
  return &g_interfaceMetrics;
}

/**
 * @brief iproute RPL information setup
 *
 * @param num num mount of instances of g_iprouteRplmetrics
 * @return void* pointer to global g_iprouteRplmetrics
 */
void* iproute_rplmetrics_get(uint32_t *num) {
  *num = 1;
  g_iprouteRplmetrics.has_inetcidrrouteindex = true;
  g_iprouteRplmetrics.has_instanceindex = true;
  g_iprouteRplmetrics.has_rank = true;
  g_iprouteRplmetrics.has_hops = true;
  g_iprouteRplmetrics.has_pathetx = true;
  g_iprouteRplmetrics.has_linketx = true;
  g_iprouteRplmetrics.has_rssiforward = true;
  g_iprouteRplmetrics.has_rssireverse = true;
  g_iprouteRplmetrics.has_lqiforward = true;
  g_iprouteRplmetrics.has_lqireverse = true;
  g_iprouteRplmetrics.has_dagsize = true;

  g_iprouteRplmetrics.inetcidrrouteindex = 1;
  g_iprouteRplmetrics.instanceindex = 1;
  g_iprouteRplmetrics.rank = 256;
  g_iprouteRplmetrics.hops = 1;
  g_iprouteRplmetrics.pathetx = 2;
  g_iprouteRplmetrics.linketx = 2;
  g_iprouteRplmetrics.rssiforward = -69;
  g_iprouteRplmetrics.rssireverse = -59;
  g_iprouteRplmetrics.lqiforward = 60;
  g_iprouteRplmetrics.lqireverse = 8;
  g_iprouteRplmetrics.dagsize = 4;

  return &g_iprouteRplmetrics;
}

/**
 * @brief wpan information setup
 *
 * @param num num mount of instances of g_wpanStatus
 * @return void* pointer to global g_wpanStatus
 */
void* wpanstatus_get(uint32_t *num) {
  *num = 1;
  g_wpanStatus.has_ifindex = true;
  g_wpanStatus.has_ssid = true;
  g_wpanStatus.has_panid = true;
  g_wpanStatus.has_master = true;
  g_wpanStatus.has_dot1xenabled = true;
  g_wpanStatus.has_securitylevel = true;
  g_wpanStatus.has_rank = true;
  g_wpanStatus.has_beaconvalid = true;
  g_wpanStatus.has_beaconversion = true;
  g_wpanStatus.has_beaconage = true;
  g_wpanStatus.has_txpower = true;
  g_wpanStatus.has_dagsize = true;
  g_wpanStatus.has_metric = true;
  g_wpanStatus.has_lastchanged = true;
  g_wpanStatus.has_lastchangedreason = true;

  g_wpanStatus.ifindex = 2;
  g_wpanStatus.ssid.len = strlen(SSID);
  memcpy(g_wpanStatus.ssid.data, SSID, strlen(SSID));
  g_wpanStatus.panid = 1234;
  g_wpanStatus.master = false;
  g_wpanStatus.dot1xenabled = false;
  g_wpanStatus.securitylevel = IEEE154_SEC_MIC_32;
  g_wpanStatus.rank = 256;
  g_wpanStatus.beaconvalid = true;
  g_wpanStatus.beaconversion = 32695;
  g_wpanStatus.beaconage = 253;
  g_wpanStatus.txpower = 28;
  g_wpanStatus.dagsize = 4;
  g_wpanStatus.metric = 1;
  g_wpanStatus.lastchanged = 10;
  g_wpanStatus.lastchangedreason = IEEE154_PAN_LEAVE_INIT;

  return &g_wpanStatus;
}

/**
 * @brief RPL instance information
 *
 * @param num mount of instances of g_rplInstance
 * @return void* pointer to global g_rplInstance
 */
void* rplinstance_get(uint32_t *num) {
  uint8_t dodagid[16] = {0x20, 0x01, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00,
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  *num = 1;
  g_rplInstance.has_instanceindex = true;
  g_rplInstance.has_instanceid = true;
  g_rplInstance.has_dodagid = true;
  g_rplInstance.has_dodagversionnumber = true;
  g_rplInstance.has_rank = true;
  g_rplInstance.has_parentcount = true;
  g_rplInstance.has_dagsize = true;

  g_rplInstance.instanceindex = 1;
  g_rplInstance.instanceid = 170;
  g_rplInstance.dodagid.len = 16;
  memcpy(g_rplInstance.dodagid.data, dodagid, 16);
  g_rplInstance.dodagversionnumber = 206;
  g_rplInstance.rank = 256;
  g_rplInstance.parentcount = 1;
  g_rplInstance.dagsize = 4;

  return &g_rplInstance;
}

/**
 * @brief   GET TLV127 VENDOR_TLVID
 *
 * @param   tlvid tlvid structure
 * @param   num instances of subtypes or g_vendorTlv
 * @return  void* pointer to global g_vendorTlv
 */
void* vendorTlv_get(tlvid_t tlvid, uint32_t *num) {
  printf("## sample_vendorTlv: GET for TLV:%u.%u\n", tlvid.vendor, tlvid.type);

  // Vendor-ID validation
  // Received Vendor-ID to match device's VENDOR_ID
  if (tlvid.vendor != VENDOR_ID) {
    printf("sample_vendorTlv: csmptlv %d vendor-id mismatch (Expected:%d, Received:%d)\n", tlvid.type, VENDOR_ID, tlvid.vendor);
    return NULL;
  }
  // Max support subtypes by the vendor
  *num = VENDOR_MAX_SUBTYPES;

  printf("## sample_vendorTlv: GET for TLV:%u.%u done\n", tlvid.vendor, tlvid.type);
  return &g_vendorTlv;
}

/**
 * @brief   POST TLV127 VENDOR_TLVID
 *
 * @param   tlvid tlvid structure
 * @param   tlv Vendor_Tlv structure
 * @return  void
 */
void vendorTlv_post(tlvid_t tlvid, Vendor_Tlv *tlv) {
  printf("## sample_vendorTlv: POST for TLV:%u.%u\n", tlvid.vendor, tlvid.type);

  int idx;

  // Vendor-ID validation
  // Received Vendor-ID to match device's VENDOR_ID
  if (tlvid.vendor != VENDOR_ID) {
    printf("sample_vendorTlv: csmptlv %d vendor-id mismatch (Expected:%d, Received:%d)\n", tlvid.type, VENDOR_ID, tlvid.vendor);
    return;
  }
  // Lookup and update subtype
  for (idx = 0; idx < VENDOR_MAX_SUBTYPES; idx++) {
    if (tlv->subtype == g_vendorTlv[idx].subtype) {
      g_vendorTlv[idx].value.len = tlv->value.len;
      memcpy(g_vendorTlv[idx].value.data, tlv->value.data, g_vendorTlv[idx].value.len);
      printf("sample_vendorTlv: Updated vendor subtype:%u\n", g_vendorTlv[idx].subtype);
      printf("## sample_vendorTlv: POST for TLV:%u.%u done\n", tlvid.vendor, tlvid.type);
      return;
    }
  }
  // New subtype will be added, overwrites subtype at g_vendorTlv[0]
  g_vendorTlv[0].subtype = tlv->subtype;
  g_vendorTlv[0].value.len = tlv->value.len;
  memcpy(g_vendorTlv[0].value.data, tlv->value.data, g_vendorTlv[0].value.len);
  printf("sample_vendorTlv: Added vendor subtype:%u\n", g_vendorTlv[0].subtype);

  printf("## sample_vendorTlv: POST for TLV:%u.%u done\n", tlvid.vendor, tlvid.type);
}

/**
 * @brief   GET TLV65 TRANSFER_REQUEST_TLVID
 *
 * @param   tlvid tlvid structure
 * @param   num amount of instances of g_transferRequest
 * @return  void* pointer to global g_transferRequest
 */
void* transferRequest_get(tlvid_t tlvid, uint32_t *num) {
  (void)tlvid;
  (void)num;
  DPRINTF("## sample_firmwaremgmt: GET for TLV %d.\n", tlvid.type);

  // Check upload slot download status
  if (g_slothdr[UPLOAD_IMAGE].status != FWHDR_STATUS_DOWNLOAD) {
    g_transferRequest.status = g_slothdr[UPLOAD_IMAGE].status;
    DPRINTF("sample_firmwaremgmt: Transfer request download status = %u\n",
           g_transferRequest.status);
    return &g_transferRequest;
  }
  // Udpate g_transferRequest fields
  memcpy(g_transferRequest.filehash.data, g_slothdr[UPLOAD_IMAGE].filehash, OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE);
  strncpy(g_transferRequest.filename, g_slothdr[UPLOAD_IMAGE].filename, OSAL_CSMP_SLOTHDR_FILE_NAME_SIZE);
  strncpy(g_transferRequest.version, g_slothdr[UPLOAD_IMAGE].version, OSAL_CSMP_SLOTHDR_VERSION_SIZE);
  strncpy(g_transferRequest.hwinfo.hwid, g_slothdr[UPLOAD_IMAGE].hwid, OSAL_CSMP_SLOTHDR_HWID_SIZE);
  g_transferRequest.filesize = g_slothdr[UPLOAD_IMAGE].filesize;
  g_transferRequest.blocksize = g_slothdr[UPLOAD_IMAGE].blocksize;
  g_transferRequest.report_int_min = g_slothdr[UPLOAD_IMAGE].reportintervalmin;
  g_transferRequest.report_int_max = g_slothdr[UPLOAD_IMAGE].reportintervalmax;
  g_transferRequest.status = g_slothdr[UPLOAD_IMAGE].status;

  DPRINTF("## sample_firmwaremgmt: GET for TLV %d done.\n", tlvid.type);
  return &g_transferRequest;
}

/**
 * @brief   POST TLV65 TRANSFER_REQUEST_TLVID
 *
 * @param   tlvid tlvid structure
 * @param   tlv Transfer_Request structure
 * @return  void
 */
void transferRequest_post(tlvid_t tlvid, Transfer_Request *tlv) {
  (void)tlvid;
  DPRINTF("## sample_firmwaremgmt: POST for TLV %d.\n", tlvid.type);

  if (!tlv) {
    DPRINTF("sample_firmwaremgmt: Transfer request tlv context is NULL\n");
    return;
  }

  uint32_t tmin = tlv->report_int_min * 1000;
  uint32_t tmax = tlv->report_int_max * 1000;

  // Check hardware id
  if (!(tlv->hwinfo.has_hwid)) {
    tlv->response = RESPONSE_INCOMPATIBLE_HW;
    DPRINTF("sample_firmwaremgmt: Invalid hardware id: %s\n", tlv->hwinfo.hwid);
    return;
  }
  // Check filehash len
  if (tlv->filehash.len != OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE) {
    tlv->response = RESPONSE_INVALID_REQ;
    DPRINTF("sample_firmwaremgmt: Invalid filehash size: %lu\n", tlv->filehash.len);
    return;
  }
  // Check filesize
  if (tlv->filesize == 0 ||
      tlv->filesize > OSAL_CSMP_FWMGMT_SLOTIMG_SIZE) {
    tlv->response = RESPONSE_FILE_SIZE_TOO_BIG;
        DPRINTF("sample_firmwaremgmt: Invalid file size: %u\n", tlv->filesize);
    return;
  }
  // blocksize should be smaller than csmp's MTU (1024)
  // blocksize should be larger than filesize/1024 since there is only 1024 bitmaps
  if (tlv->blocksize == 0 ||
      tlv->blocksize > OSAL_CSMP_SLOTHDR_BLOCK_SIZE ||
      tlv->blocksize < tlv->filesize/(OSAL_CSMP_FWMGMT_BLKMAP_CNT * 32)) {
    tlv->response = RESPONSE_INVALID_BLOCK_SIZE;
    DPRINTF("sample_firmwaremgmt: Invalid block size: %u\n", tlv->blocksize);
    return;
  }
  // Check pending reboot
  if (g_initload && (g_curloadslot == UPLOAD_IMAGE)) {
    tlv->response = RESPONSE_PENDING_REBOOT;
    DPRINTF("sample_firmwaremgmt: Pending reboot for upload image\n");
    return;
  }
  // Check duplicate request on Upload slot
  if ((memcmp(tlv->filehash.data, g_slothdr[UPLOAD_IMAGE].filehash, tlv->filehash.len)) == 0) {
    tlv->response = RESPONSE_DUP_XFER;
    DPRINTF("sample_firmwaremgmt: Duplicate transfer request\n");
    return;
  }
  // Check duplicate request on Run slot
  if ((memcmp(tlv->filehash.data, g_slothdr[RUN_IMAGE].filehash, tlv->filehash.len)) == 0) {
    tlv->response = RESPONSE_MATCH_RUN_XFER;
    DPRINTF("sample_firmwaremgmt: Transfer request matches Run image\n");
    return;
  }
  // Check duplicate request on Backup slot
  if ((memcmp(tlv->filehash.data, g_slothdr[BACKUP_IMAGE].filehash, tlv->filehash.len)) == 0) {
    tlv->response = RESPONSE_MATCH_BAK_XFER;
    DPRINTF("sample_firmwaremgmt: Transfer request matches Backup image\n");
    return;
  }

  // Initiliase new transfer - start
  tlv->has_response = true;
  tlv->response = RESPONSE_OK;
  memcpy(&g_transferRequest, tlv, sizeof(g_transferRequest));
  if (!g_initxfer) {
    g_initxfer = true;
  }

  // Init report intervals
  tmin = (tmin >= MIN_REPORT_MIN) ? tmin : MIN_REPORT_MIN;
  tmax = (tmax >= MAX_REPORT_MIN) ? tmax : MAX_REPORT_MIN;
  tmax = (tmax < tmin) ? tmin : tmax;

  // Erase upload slot
  memset(&g_slothdr[UPLOAD_IMAGE], 0xFF, sizeof(g_slothdr[UPLOAD_IMAGE]));

  // Init upload slot from tlv context
  memcpy(g_slothdr[UPLOAD_IMAGE].filehash, tlv->filehash.data, OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE);
  strncpy(g_slothdr[UPLOAD_IMAGE].filename, tlv->filename, sizeof(g_slothdr[UPLOAD_IMAGE].filename));
  strncpy(g_slothdr[UPLOAD_IMAGE].version, tlv->version, sizeof(g_slothdr[UPLOAD_IMAGE].version));
  strncpy(g_slothdr[UPLOAD_IMAGE].hwid, tlv->hwinfo.hwid, sizeof(g_slothdr[UPLOAD_IMAGE].hwid));
  g_slothdr[UPLOAD_IMAGE].filesize = tlv->filesize;
  g_slothdr[UPLOAD_IMAGE].blocksize = tlv->blocksize;
  g_slothdr[UPLOAD_IMAGE].filesizelastblk = g_slothdr[UPLOAD_IMAGE].filesize %
                                            g_slothdr[UPLOAD_IMAGE].blocksize;
  if (g_slothdr[UPLOAD_IMAGE].filesizelastblk == 0) {
      g_slothdr[UPLOAD_IMAGE].filesizelastblk = g_slothdr[UPLOAD_IMAGE].blocksize;
  }
  g_slothdr[UPLOAD_IMAGE].blockcnt = (g_slothdr[UPLOAD_IMAGE].filesize +
                                     (g_slothdr[UPLOAD_IMAGE].blocksize - 1)) /
                                      g_slothdr[UPLOAD_IMAGE].blocksize;
  g_slothdr[UPLOAD_IMAGE].reportintervalmin = tmin;
  g_slothdr[UPLOAD_IMAGE].reportintervalmax = tmax;
  g_slothdr[UPLOAD_IMAGE].status = FWHDR_STATUS_DOWNLOAD;
  // g_slothdr[UPLOAD_IMAGE].magicU = CSMP_FWMGMT_SLOTMAGIC_U;
  // g_slothdr[UPLOAD_IMAGE].magicL = CSMP_FWMGMT_SLOTMAGIC_L;

  // Initiliase new transfer - done
  g_initxfer = false;

  DPRINTF("Erasing 'UPLOAD' storage slot...\n");
  assert(osal_erase_storaqe(UPLOAD_IMAGE, &g_slothdr[UPLOAD_IMAGE]) == OSAL_SUCCESS);

  DPRINTF("## sample_firmwaremgmt: POST for TLV %d done.\n", tlvid.type);
}

/**
 * @brief   POST TLV67 IMAGE_BLOCK_TLVID
 *
 * @param   tlvid tlvid structure
 * @param   tlv Image_Block structure
 * @return  void
 */
void imageBlock_post(tlvid_t tlvid, Image_Block *tlv) {
  (void)tlvid;
  DPRINTF("## sample_firmwaremgmt: POST for TLV %d.\n", tlvid.type);

  if (!tlv) {
    DPRINTF("sample_firmwaremgmt: Image block tlv context is NULL\n");
    return;
  }

  // Update g_imageBlock structure from tlv context
  memcpy(&g_imageBlock, tlv, sizeof(g_imageBlock));

  // Check filehash.len, filehash.data, blockdata.len with upload slot
  if ((g_imageBlock.filehash.len >= MIN_HASH_COMPARE_LEN) &&
      (g_imageBlock.filehash.len <= sizeof(g_slothdr[UPLOAD_IMAGE].filehash)) &&
      (memcmp(g_imageBlock.filehash.data, g_slothdr[UPLOAD_IMAGE].filehash,
              g_imageBlock.filehash.len) == 0) &&
      (g_imageBlock.blockdata.len <= g_slothdr[UPLOAD_IMAGE].blocksize)) {
    tlv->retval = true;
    // Write image block to upload slot
    DPRINTF("sample_firmwaremgmt: Writing image block to upload slot\n");

    uint32_t bytes = 0;
    (void)bytes; // Suppress unused param warning.
    uint32_t word = g_imageBlock.blocknum >> 5;
    uint32_t bit = 31 - (g_imageBlock.blocknum & 31);
    uint32_t mapval = 0xFFFFFFFFUL;
    uint32_t offset = g_imageBlock.blocknum * g_slothdr[UPLOAD_IMAGE].blocksize;
    DPRINTF("sample_firmwaremgmt: Image block blocknum=%u offset=%u [word=%x bit=%x]\n",
           g_imageBlock.blocknum, offset, word, bit);

    // Check Transfer Request intialised
    if (g_initxfer) {
      DPRINTF("sample_firmwaremgmt: Transfer still initializing... (Image block %u)\n",
             g_imageBlock.blocknum);
      g_downloadbusy = false;
      return;
    }
    // Check blocknum exceeds bitmap
    if (word >= OSAL_CSMP_FWMGMT_BLKMAP_CNT) {
      DPRINTF("sample_firmwaremgmt: Image block %u exceeds bitmap length\n",
             g_imageBlock.blocknum);
      g_downloadbusy = false;
      return;
    }

    mapval = g_slothdr[UPLOAD_IMAGE].nblkmap[word];
    DPRINTF("sample_firmwaremgmt: Image block mapval=0x%x\n", mapval);

    if ((mapval & (1 << bit)) == 0) {
      DPRINTF("sample_firmwaremgmt: Image block %u already written\n",
             g_imageBlock.blocknum);
      // Check for transfer completion
      if(g_slothdr[UPLOAD_IMAGE].status == FWHDR_STATUS_COMPLETE)
      {
        DPRINTF("sample_firmwaremgmt: Transfer completed, skipping redundant image block\n");
        g_downloadbusy = false;
        return;
      }
      // Check slot header bitmap for download completion
      uint32_t blk_i = 0, blk_j = 0, shift = 31;
      uint32_t blk_whole_cnt;
      uint32_t last_mapval;

      blk_whole_cnt = g_slothdr[UPLOAD_IMAGE].blockcnt & ~0x1F;
      while ((blk_i < (OSAL_CSMP_FWMGMT_BLKMAP_CNT - 1)) &&
            (blk_j < blk_whole_cnt)) {
        if (g_slothdr[UPLOAD_IMAGE].nblkmap[blk_i] != 0) {
          DPRINTF("sample_firmwaremgmt: Image block transfer still not complete\n");
          g_downloadbusy = false;
          return;
        }
        blk_i++;
        blk_j += 32;
      }
      last_mapval = g_slothdr[UPLOAD_IMAGE].nblkmap[blk_i];
      while (blk_j < g_slothdr[UPLOAD_IMAGE].blockcnt) {
        if (last_mapval & (1 << shift)) {
          DPRINTF("sample_firmwaremgmt: Last image block transfer still not complete\n");
          g_downloadbusy = false;
          return;
        }
        blk_j++;
        shift--;
      }

      // Set slot status as complete else bad image
      DPRINTF("sample_firmwaremgmt: Image block transfer complete, filehash matched!\n");
      g_slothdr[UPLOAD_IMAGE].status = FWHDR_STATUS_COMPLETE;
      fclose(upload_slot);
      upload_slot = NULL;
      g_downloadbusy = false;
      if (osal_write_firmware_slothdr(UPLOAD_IMAGE, &g_slothdr[UPLOAD_IMAGE]) < 0)
      return;
    }
    if(upload_slot == NULL){
      upload_slot = fopen("opencsmp-upload-slot.bin", "wb");
    }
    // Write image block to slot at valid offset
    if (offset < OSAL_CSMP_FWMGMT_SLOTIMG_SIZE &&
       ((offset + g_imageBlock.blockdata.len) < OSAL_CSMP_FWMGMT_SLOTIMG_SIZE)) {
      DPRINTF("sample_firmwaremgmt: Valid image block %u write offset=%u\n",
             g_imageBlock.blocknum, offset);
      if(upload_slot != NULL){
        fseek(upload_slot, offset, SEEK_SET);
        fwrite(g_imageBlock.blockdata.data, 1, g_imageBlock.blockdata.len, upload_slot);
        fflush(upload_slot);
        osal_write_slothdr(UPLOAD_IMAGE, g_slothdr);
      }
      else{
        printf("image block write to file failed!");
      }
      }
      mapval ^= (1 << bit);
      g_slothdr[UPLOAD_IMAGE].nblkmap[word] = mapval;

      DPRINTF("sample_firmwaremgmt: Write image block %u (len=%lu offset=%u) \
             success! [mapval=0x%x word=0x%x bit=0x%x]\n",
             g_imageBlock.blocknum, g_imageBlock.blockdata.len, offset, mapval, word, bit);
    } else {
      // Invalid write offset
      DPRINTF("sample_firmwaremgmt: Invalid image block %u write offset=%u\n",
             g_imageBlock.blocknum, offset);
    }
  } else {
    tlv->retval = false;
    DPRINTF("sample_firmwaremgmt: Image block POST failed!\n");
  }

  g_downloadbusy = false;
  DPRINTF("## sample_firmwaremgmt: POST for TLV %d done.\n", tlvid.type);
}

/**
 * @brief   GET TLV68 LOAD_REQUEST_TLVID
 *
 * @param   tlvid tlvid structure
 * @param   num amount of instances of g_firmwareImageInfo
 * @return  void* pointer to global g_loadRequest
 */
void* loadRequest_get(tlvid_t tlvid, uint32_t *num) {
  (void)tlvid;
  (void)num;
  DPRINTF("## sample_firmwaremgmt: GET for TLV %d.\n", tlvid.type);

  // Check for pending active load requests
  if (!g_initload) {
    DPRINTF("sample_firmwaremgmt: No active load requests pending\n");
    return NULL;
  }

  // Check for valid current load slot
  switch (g_curloadslot) {
  case UPLOAD_IMAGE:
    memcpy(g_loadRequest.filehash.data, g_slothdr[UPLOAD_IMAGE].filehash,
           OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE);
  break;
  case BACKUP_IMAGE:
    memcpy(g_loadRequest.filehash.data, g_slothdr[BACKUP_IMAGE].filehash,
           OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE);
  break;
  default:
    DPRINTF("sample_firmwaremgmt: Current load slot invalid\n");
    return NULL;
  }
  g_loadRequest.filehash.len = OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE;
  g_loadRequest.loadtime = g_curloadtime;

  DPRINTF("## sample_firmwaremgmt: GET for TLV %d done.\n", tlvid.type);
  return &g_loadRequest;
}

void loadreq_timer_fired() {
  DPRINTF("loadreq_timer: Load request timer fired for slot=%d with delay=%u\n",
         g_curloadslot, g_curloadtime);
  DPRINTF("loadreq_timer: Writing Run Slot to disk\n");
  osal_copy_firmware(g_curloadslot, RUN_IMAGE, g_slothdr);
  g_curloadslot=0xFF;
  g_curloadtime=0;
  osal_trickle_timer_stop(lrq_timer);
  g_reboot_request = true;
  sample_data_init(); //Note: This function resets init time and g_slot_hdr to simulate reboot only for linux
  osal_system_reboot(&g_devconfig.NMSaddr);
  g_reboot_request = false;
}

/**
 * @brief   REBOOT REQUEST TIMER HANDLER
 * @return  void
 */
void rebootreq_timer_fired() {
  DPRINTF("rebootreq_timer: Reboot request timer fired\n");
  osal_trickle_timer_stop(async_timer);
  sample_data_init(); //Note: This function resets init time and g_slot_hdr to simulate reboot only for linux
  osal_system_reboot(&g_devconfig.NMSaddr);
  g_reboot_request = false;
}

/**
 * @brief   POST TLV68 LOAD_REQUEST_TLVID
 *
 * @param   tlvid tlvid structure
 * @param   tlv Load_Request structure
 * @return  void
 */
void loadRequest_post(tlvid_t tlvid, Load_Request *tlv) {
  (void)tlvid;
  uint32_t newloadslot;
  uint32_t delay = MIN_LOAD_DELAY; // 1 sec
  DPRINTF("## sample_firmwaremgmt: POST for TLV %d.\n", tlvid.type);

  if (!tlv) {
    DPRINTF("sample_firmwaremgmt: Load request tlv context is NULL\n");
    return;
  }

  // Reference g_loadRequest via tlv context
  g_loadRequest = *tlv;

  // Check if filehash matches UPLOAD image
  if (memcmp(g_loadRequest.filehash.data, g_slothdr[UPLOAD_IMAGE].filehash,
             g_loadRequest.filehash.len) == 0) {
    switch (g_slothdr[UPLOAD_IMAGE].status) {
      case FWHDR_STATUS_COMPLETE:
        DPRINTF("sample_firmwaremgmt: Load request ok for upload slot image\n");
        tlv->response = RESPONSE_OK;
        break;
      case FWHDR_STATUS_BADHASH:
      case FWHDR_STATUS_BADIMAGE:
        DPRINTF("sample_firmwaremgmt: Load request on bad upload slot image\n");
        tlv->response = RESPONSE_SIGNATURE_FAILED;
        break;
      default:
        DPRINTF("sample_firmwaremgmt: Load request incomplete\n");
        tlv->response = RESPONSE_INCOMPLETE;
        break;
    }
    if (tlv->response != RESPONSE_OK) {
      DPRINTF("sample_firmwaremgmt: Load request failed\n");
      return;
    }
    newloadslot = UPLOAD_IMAGE;
  } // Check if filehash matches RUN image
  else if (memcmp(g_loadRequest.filehash.data, g_slothdr[RUN_IMAGE].filehash,
           g_loadRequest.filehash.len) == 0) {
    DPRINTF("sample_firmwaremgmt: Load request on running image\n");
    tlv->response = RESPONSE_IMAGE_RUNNING;
    return;
  } // Check if filehash matches BACKUP image
  else if (memcmp(g_loadRequest.filehash.data, g_slothdr[BACKUP_IMAGE].filehash,
           g_loadRequest.filehash.len) == 0) {
    newloadslot = BACKUP_IMAGE;
  } // Load request for unknown filehash
  else {
    DPRINTF("sample_firmwaremgmt: Load request with unknown filehash\n");
    tlv->response = RESPONSE_UNKNOWN_HASH;
    return;
  }
  // Check for redundant load request
  if (g_initload && (newloadslot == g_curloadslot) &&
      (g_loadRequest.loadtime == g_curloadtime)) {
    DPRINTF("sample_firmwaremgmt: Redundant load request\n");
    return;
  }

  // Cancel any previous/pending load request timer
  osal_trickle_timer_stop(lrq_timer);
  g_curloadslot = newloadslot;
  g_curloadtime = 0;
  g_initload = true;

  if (g_loadRequest.loadtime != 0) {
    // Get current time to calculate timer delay for load request
    struct timeval tv;
    osal_gettime(&tv, NULL);

    if (g_loadRequest.loadtime > tv.tv_sec) {
      delay = (g_loadRequest.loadtime - tv.tv_sec);
    } else {
      DPRINTF("sample_firmwaremgmt: Load request valid, local clock invalid\n");
      tlv->response = RESPONSE_INVALID_REQ;
      g_initload = false;
      return;
    }
  }

  // Upon success update g_loadRequest structure from tlv context
  memcpy(&g_loadRequest, tlv, sizeof(g_loadRequest));

  // Stop previous/pending load timers
  osal_trickle_timer_stop(lrq_timer);
    // Start new timer delay
  g_curloadtime = g_loadRequest.loadtime;
  osal_trickle_timer_start(lrq_timer, delay, delay,
                      (trickle_timer_fired_t)loadreq_timer_fired);
  DPRINTF("sample_firmwaremgmt: Load request timer started for slot=%d with delay=%u at epoch time =%u s\n",
         g_curloadslot, delay, g_curloadtime);

  DPRINTF("## sample_firmwaremgmt: POST for TLV %d done.\n", tlvid.type);
}

/**
 * @brief   POST TLV69 CANCEL_LOAD_REQUEST_TLVID
 *
 * @param   tlvid tlvid structure
 * @param   tlv Cancel_Load_Request structure
 * @return  void
 */
void cancelLoadRequest_post(tlvid_t tlvid, Cancel_Load_Request *tlv) {
  (void)tlvid;
  const uint8_t *filehash = NULL;
  DPRINTF("## sample_firmwaremgmt: POST for TLV %d.\n", tlvid.type);

  if (!tlv) {
    DPRINTF("sample_firmwaremgmt: Cancel load request tlv context is NULL\n");
    return;
  }

  switch (g_curloadslot) {
    case RUN_IMAGE:
    DPRINTF("sample_firmwaremgmt: Received cancel load request for RUN_IMAGE (%u)\n",
           g_curloadslot);
    filehash = g_slothdr[RUN_IMAGE].filehash;
    break;
    case UPLOAD_IMAGE:
    DPRINTF("sample_firmwaremgmt: Received cancel load request for UPLOAD_IMAGE (%u)\n",
           g_curloadslot);
    filehash = g_slothdr[UPLOAD_IMAGE].filehash;
    break;
    case BACKUP_IMAGE:
    DPRINTF("sample_firmwaremgmt: Received cancel load request for BACKUP_IMAGE (%u)\n",
           g_curloadslot);
    filehash = g_slothdr[BACKUP_IMAGE].filehash;
    break;
    default:
    DPRINTF("sample_firmwaremgmt: Received cancel load request for invalid slot (%u)\n",
           g_curloadslot);
    if (g_curloadslot != 0xFFU){
      tlv->response = RESPONSE_INVALID_REQ;
    }
    else{
      DPRINTF("sample_firmwaremgmt: Redundant Cancel Load Request\n");
    }
    return;
  }

  if ((filehash != NULL) &&
    memcmp(tlv->filehash.data, filehash, OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE) == 0) {
    DPRINTF("sample_firmwaremgmt: Cancel load request valid, cancelling current load request\n");

    // Cancel current load request
    osal_trickle_timer_stop(lrq_timer);
    g_initload = false;
    g_curloadtime = 0;
    g_curloadslot = 0xFFU;
  }

  DPRINTF("## sample_firmwaremgmt: POST for TLV %d done.\n", tlvid.type);
}

/**
 * @brief   POST TLV70 SET_BACKUP_REQUEST_TLVID
 *
 * @param   tlvid tlvid structure
 * @param   tlv Set_Backup_Request structure
 * @return  void
 */
void setBackupRequest_post(tlvid_t tlvid, Set_Backup_Request *tlv) {
  (void)tlvid;
  DPRINTF("## sample_firmwaremgmt: POST for TLV %d.\n", tlvid.type);

  if (!tlv) {
    DPRINTF("sample_firmwaremgmt: Cancel load request tlv context is NULL\n");
    return;
  }

  // Check for upload image status
  if (memcmp(tlv->filehash.data, g_slothdr[UPLOAD_IMAGE].filehash,
             tlv->filehash.len) == 0) {
    if (g_slothdr[UPLOAD_IMAGE].status != FWHDR_STATUS_COMPLETE) {
      DPRINTF("sample_firmwaremgmt: Set backup request for imcomplete upload image\n");
      tlv->response = RESPONSE_INCOMPLETE;
      return;
    }
    DPRINTF("sample_firmwaremgmt: Set backup request for upload image\n");
    g_curbackupslot = UPLOAD_IMAGE;
  } // Check for run image status
  else if (memcmp(tlv->filehash.data, g_slothdr[RUN_IMAGE].filehash,
                  tlv->filehash.len) == 0) {
    DPRINTF("sample_firmwaremgmt: Set backup request for run image\n");
    g_curbackupslot = RUN_IMAGE;
  } // Backup request for unknon filehash
  else {
    DPRINTF("sample_firmwaremgmt: Set backup request for unknown filehash\n");
    tlv->response = RESPONSE_UNKNOWN_HASH;
    return;
  }

  // Check for pending load requests on backup image
  if (g_initload && (g_curloadslot = BACKUP_IMAGE)) {
    DPRINTF("sample_firmwaremgmt: Active load request on backup image\n");
    tlv->response = RESPONSE_PENDING_REBOOT;
    return;
  }
  // Erase backup slot
  if (g_curbackupslot == RUN_IMAGE || g_curbackupslot == UPLOAD_IMAGE) {
    memset(&g_slothdr[BACKUP_IMAGE], 0xFF, sizeof(g_slothdr[BACKUP_IMAGE]));
  }
  // Copy target image to backup slot
  switch (g_curbackupslot) {
    case RUN_IMAGE:
      DPRINTF("sample_firmwaremgmt: Backing-up run image to backup slot\n");
      osal_copy_firmware(RUN_IMAGE, BACKUP_IMAGE, g_slothdr);
      g_slothdr[BACKUP_IMAGE].status = FWHDR_STATUS_COMPLETE;
      break;
    case UPLOAD_IMAGE:
      DPRINTF("sample_firmwaremgmt: Backing-up upload image to backup slot\n");
      osal_copy_firmware(UPLOAD_IMAGE, BACKUP_IMAGE, g_slothdr);
      g_slothdr[BACKUP_IMAGE].status = FWHDR_STATUS_COMPLETE;
      break;
    default:
      DPRINTF("sample_firmwaremgmt: Set backup request from invalid backup slot (%u)\n",
             g_curbackupslot);
      tlv->response = RESPONSE_INVALID_REQ;
      return;
  }
  g_curbackupslot = 0xFFU;

  DPRINTF("## sample_firmwaremgmt: POST for TLV %d done.\n", tlvid.type);
}

/**
 * @brief   GET TLV75 FIRMWARE_IMAGE_INFO_TLVID
 *
 * @param   tlvid tlvid structure
 * @param   num number of instances of g_firmwareImageInfo
 * @return  void* pointer to global g_firmwareImageInfo
 */
void* firmwareImageInfo_get(tlvid_t tlvid, uint32_t *num) {
  (void)tlvid;
  *num = 0;
  DPRINTF("## sample_firmwaremgmt: GET for TLV %d.\n", tlvid.type);

  // Enumerate all active slots
  // Active slots: 0-RUN_IMAGE, 1-UPLOAD_IMAGE, 2-BACKUP_IMAGE
  for (uint32_t idx = 0; idx < OSAL_CSMP_FWMGMT_ACTIVE_SLOTS; idx++) {
      // Reset g_firmwareImageInfo[slotid] structure
      memset(&g_firmwareImageInfo[idx], 0, sizeof(g_firmwareImageInfo[idx]));

      // Track number of inuse active slots
      (*num)++;
      DPRINTF("sample_firmwaremgmt: Reading firmware image info for slot id:%d\n", idx);

      // Index
      g_firmwareImageInfo[idx].has_index = true;
      g_firmwareImageInfo[idx].index = idx + 1;
      // Filehash
      g_firmwareImageInfo[idx].has_filehash = true;
      g_firmwareImageInfo[idx].filehash.len = sizeof(g_slothdr[idx].filehash);
      memcpy(g_firmwareImageInfo[idx].filehash.data, g_slothdr[idx].filehash,
             sizeof(g_slothdr[idx].filehash));
      // Filename
      g_firmwareImageInfo[idx].has_filename = true;
      strncpy(g_firmwareImageInfo[idx].filename, g_slothdr[idx].filename,
              OSAL_CSMP_SLOTHDR_FILE_NAME_SIZE);
      // Version
      g_firmwareImageInfo[idx].has_version = true;
      strncpy(g_firmwareImageInfo[idx].version, g_slothdr[idx].version,
              OSAL_CSMP_SLOTHDR_VERSION_SIZE);
      // Filesize
      g_firmwareImageInfo[idx].has_filesize = true;
      g_firmwareImageInfo[idx].filesize = g_slothdr[idx].filesize;
      // Blocksize
      g_firmwareImageInfo[idx].has_blocksize = true;
      g_firmwareImageInfo[idx].blocksize = g_slothdr[idx].blocksize;
      // Blockcount
      g_firmwareImageInfo[idx].blockcnt = g_slothdr[idx].blockcnt;
      // Bitmap
      //Invert the nblkmap and store in bitmap data
      if (g_slothdr[idx].status == FWHDR_STATUS_DOWNLOAD) {
        uint32_t i, j;
        uint32_t blkmapCnt = (g_slothdr[idx].blockcnt + 31) / 32;

        if (blkmapCnt > OSAL_CSMP_FWMGMT_BLKMAP_CNT) {
          blkmapCnt = OSAL_CSMP_FWMGMT_BLKMAP_CNT;
        }
        g_firmwareImageInfo[idx].has_bitmap = true;
        for (i = 0, j = 0; (i < blkmapCnt) &&
            (j < sizeof(g_firmwareImageInfo[idx].bitmap.data)); i++, j += 4) {
          uint32_t val = ~g_slothdr[idx].nblkmap[i];
          g_firmwareImageInfo[idx].bitmap.data[j] = (val >> 24) & 0xFF;
          g_firmwareImageInfo[idx].bitmap.data[j+1] = (val >> 16) & 0xFF;
          g_firmwareImageInfo[idx].bitmap.data[j+2] = (val >> 8) & 0xFF;
          g_firmwareImageInfo[idx].bitmap.data[j+3] = (val) & 0xFF;
        }
        if (j > ((g_slothdr[idx].blockcnt + 7) / 8)) {
          j = (g_slothdr[idx].blockcnt + 7) / 8;
        }
        g_firmwareImageInfo[idx].bitmap.len = j;
      } else {
        g_firmwareImageInfo[idx].has_bitmap = false;
      }
      // Default image?
      g_firmwareImageInfo[idx].has_isdefault = (idx == BACKUP_IMAGE) ? true : false;
      g_firmwareImageInfo[idx].isdefault = g_firmwareImageInfo[idx].has_isdefault;
      // Running image?
      g_firmwareImageInfo[idx].has_isrunning = (idx == RUN_IMAGE) ? true : false;
      g_firmwareImageInfo[idx].isrunning = g_firmwareImageInfo[idx].has_isrunning;
      // Loadtime
      if (g_curloadtime != 0 && g_curloadslot == idx) {
        g_firmwareImageInfo[idx].has_loadtime = true;
        g_firmwareImageInfo[idx].loadtime = g_curloadtime;
      }
      // Hardware Id
      g_firmwareImageInfo[idx].has_hwinfo = true;
      g_firmwareImageInfo[idx].hwinfo.has_hwid = true;
      strncpy(g_firmwareImageInfo[idx].hwinfo.hwid, g_slothdr[idx].hwid, OSAL_CSMP_SLOTHDR_HWID_SIZE);
      // Vendor Hardware Id
      g_firmwareImageInfo[idx].hwinfo.has_vendorhwid = false;
      // Kernel version
      g_firmwareImageInfo[idx].has_kernelversion =  false;
      // Subkernel version
      g_firmwareImageInfo[idx].has_subkernelversion = false;
      // Loader error
      g_firmwareImageInfo[idx].has_loaderrorcode = false;
      // Subloader error
      g_firmwareImageInfo[idx].has_subloaderrorcode = false;

      // Download status
      g_firmwareImageInfo[idx].status = g_slothdr[idx].status;
  }

  DPRINTF("## sample_firmwaremgmt: GET for TLV %d done.\n", tlvid.type);
  return &g_firmwareImageInfo;
}

/**
 * @brief get the signature settings
 *
 * @param num amount of instances
 * @return void* pointer to global g_SignatureSettings
 */
void* signature_settings_get(uint32_t *num) {
  *num = 1;

  return &g_SignatureSettings;
}

/**
 * @brief set the signature settings
 *
 * @param tlv
 */
void signature_settings_post(Signature_Settings *tlv) {
  g_SignatureSettings.has_reqsignedpost = true;
  g_SignatureSettings.reqsignedpost = tlv->reqsignedpost;

  g_SignatureSettings.has_reqvalidcheckpost = true;
  g_SignatureSettings.reqvalidcheckpost = tlv->reqvalidcheckpost;

  g_SignatureSettings.has_reqtimesyncpost = true;
  g_SignatureSettings.reqtimesyncpost = tlv->reqtimesyncpost;

  g_SignatureSettings.has_reqseclocalpost = true;
  g_SignatureSettings.reqseclocalpost = tlv->reqseclocalpost;

  g_SignatureSettings.has_reqsignedresp = true;
  g_SignatureSettings.reqsignedresp = tlv->reqsignedresp;

  g_SignatureSettings.has_reqvalidcheckresp = true;
  g_SignatureSettings.reqvalidcheckresp = tlv->reqvalidcheckresp;

  g_SignatureSettings.has_reqtimesyncresp = true;
  g_SignatureSettings.reqtimesyncresp = tlv->reqtimesyncresp;

  g_SignatureSettings.has_reqseclocalresp = true;
  g_SignatureSettings.reqseclocalresp = tlv->reqseclocalresp;

  g_SignatureSettings.has_cert = true;
  g_SignatureSettings.cert.len = tlv->cert.len;
  memcpy(g_SignatureSettings.cert.data,tlv->cert.data,g_SignatureSettings.cert.len);
}
 * @brief   POST TLV32 REBOOT_REQUEST_TLVID
 * @param   tlvid tlvid structure
 * @param   tlv Vendor_Tlv structure
 * @return  void
void rebootRequest_post(tlvid_t tlvid, Reboot_Request *tlv) {
  printf("## app_reboot_request: POST for TLV:%u.%u\n", tlvid.vendor, tlvid.type);
  switch(tlv->flag){
    case REBOOT:
      DPRINTF("** app_rebootRequest: Reboot timer initializing...\n");
      osal_trickle_timer_start(async_timer, REBOOT_DELAY, REBOOT_DELAY, (trickle_timer_fired_t)rebootreq_timer_fired);
      g_reboot_request = true;
      DPRINTF("** app_rebootRequest: Reboot flag not supported!\n");
