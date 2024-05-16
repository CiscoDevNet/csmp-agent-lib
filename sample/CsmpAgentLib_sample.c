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

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "csmp_service.h"
#include "csmp_info.h"
#include "CsmpAgentLib_sample.h"
#include "signature_verify.h"
#include "osal.h"

#define nexthop_IP "fe80::a00:27ff:fe3b:2ab1"
#define CSMP_NON_VENDOR_ID 0
#define VENDOR_DATA_LEN 64

char *SSID = "Cisco Systems";
char vendorhwid[32] = "vendor hardware ID";
uint32_t g_init_time;
uint8_t neighbor_eui64[2][8] = {{0x0a, 0x00, 0x27, 0xff, 0xfe, 0x3b, 0x2a, 0xb1},
                             {0x0a, 0x00, 0x27, 0xff, 0xfe, 0x3b, 0x2a, 0xb0}};
dev_config_t g_devconfig;
csmp_handle_t g_csmp_handle;

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

  g_hardwareDesc.entphysicalindex = 1;
  sprintf(g_hardwareDesc.entphysicaldescr,"CSMP Agent Lib test node");
  g_hardwareDesc.entphysicalclass = MODULE;
  sprintf(g_hardwareDesc.entphysicalname,"lowpan");
  sprintf(g_hardwareDesc.entphysicalhardwarerev,"1.0");
  sprintf(g_hardwareDesc.entphysicalfirmwarerev,"1.0.0");
  snprintf(g_hardwareDesc.entphysicalserialnum,sizeof(g_hardwareDesc.entphysicalserialnum),
             "%02X%02X%02X%02X%02X%02X%02X%02X",
             g_eui64[0],g_eui64[1],g_eui64[2],g_eui64[3],
             g_eui64[4],g_eui64[5],g_eui64[6],g_eui64[7]);
  sprintf(g_hardwareDesc.entphysicalmfgname,"IOTG CRDC");
  sprintf(g_hardwareDesc.entphysicalmodelname,"CSMP AGENT LIB");
  g_hardwareDesc.entphysicalfunction = 1;

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
  osal_inet_pton(AF_INET6, "2001:a:b:c::9", &g_ipAddress[2].ipaddressaddr.data);
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
    settimeofday(&tv, NULL);
  }
}

/**
 * @brief get the vendor specific data
 *
 * @param num amount of instances
 * @return void* pointer to g_VendorData
 */
void* vendorspecificdata_get(tlvid_t tlvid, uint32_t *num) {
  if (tlvid.type <= 0 || tlvid.type > 10)
  {
    return NULL;
  }
  *num = 1;
  g_VendorData[tlvid.type - 1].has_data = true;
  return &g_VendorData[tlvid.type - 1];
}

/**
 * @brief set the vendor specific data
 *
 * @param tlv
 */
void vendorspecificdata_post(tlvid_t tlvid, Vendor_Specific *tlv) {
  if (tlvid.type <= 0 || tlvid.type > 10)
  {
    return;
  }
  if (tlv->data.len > VENDOR_DATA_LEN)
  {
    return;
  }
  g_VendorData[tlvid.type - 1].has_data = true;
  g_VendorData[tlvid.type - 1].data.len = tlv->data.len;
  memcpy(g_VendorData[tlvid.type - 1].data.data,tlv->data.data,g_VendorData[tlvid.type - 1].data.len);
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
 * @brief firmware information
 *
 * @param num amount of instances of g_firmwareImageInfo
 * @return void* pointer to global g_firmwareImageInfo
 */
void* firmware_image_info_get(uint32_t *num) {
  *num = 1;

  g_firmwareImageInfo.has_index = true;
  g_firmwareImageInfo.has_filehash = true;
  g_firmwareImageInfo.has_filename = true;
  g_firmwareImageInfo.has_version = true;
  g_firmwareImageInfo.has_filesize = true;
  g_firmwareImageInfo.has_isdefault = true;
  g_firmwareImageInfo.has_isrunning = true;
  g_firmwareImageInfo.has_hwinfo = true;
  g_firmwareImageInfo.hwinfo.has_hwid = true;
  g_firmwareImageInfo.hwinfo.has_vendorhwid = true;

  g_firmwareImageInfo.index = 1;
  g_firmwareImageInfo.filehash.len = 32;
  sprintf((char *)g_firmwareImageInfo.filehash.data, "12345");
  sprintf(g_firmwareImageInfo.filename, "vendor firmware");
  sprintf(g_firmwareImageInfo.version, "1.0.0");
  g_firmwareImageInfo.filesize = 246272;
  g_firmwareImageInfo.isdefault = true;
  g_firmwareImageInfo.isrunning = true;
  memcpy(g_firmwareImageInfo.hwinfo.vendorhwid, vendorhwid, strlen(vendorhwid));

  return &g_firmwareImageInfo;
}

/**
 * @brief get up the signature settings
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

/**
 * @brief csmp get TLV request
 *
 * @param tlvid the tlvid to handle
 * @param num returned amount of instances
 * @return void* pointer to the global variable containing the return data
 */
void* csmptlvs_get(tlvid_t tlvid, uint32_t *num) {
  if (tlvid.vendor == CSMP_NON_VENDOR_ID)
  {
    switch(tlvid.type) {
    case HARDWARE_DESC_ID:
      return hardware_desc_get(num);
    case INTERFACE_DESC_ID:
      return interface_desc_get(num);
    case IPADDRESS_ID:
      return ipaddress_get(num);
    case IPROUTE_ID:
      return iproute_get(num);
    case CURRENT_TIME_ID:
      return currenttime_get(num);
    case UPTIME_ID:
      return uptime_get(num);
    case INTERFACE_METRICS_ID:
      return interface_metrics_get(num);
    case IPROUTE_RPLMETRICS_ID:
      return iproute_rplmetrics_get(num);
    case WPANSTATUS_ID:
      return wpanstatus_get(num);
    case RPLINSTANCE_ID:
      return rplinstance_get(num);
    case FIRMWARE_IMAGE_INFO_ID:
      return firmware_image_info_get(num);
    case SIGNATURE_SETTINGS_ID:
      return signature_settings_get(num);

    default:
      break;
    }
  }
  else
  {
    return vendorspecificdata_get(tlvid, num);
  }
  return NULL;
}

/**
 * @brief csmp post TLV request
 *
 * @param tlvid the tlvid to handle
 * @param tlv the request data
 */
void csmptlvs_post(tlvid_t tlvid, void *tlv) {
  if (tlvid.vendor == CSMP_NON_VENDOR_ID)
  {
    switch(tlvid.type) {
    case CURRENT_TIME_ID:
      currenttime_post((Current_Time*)tlv);
      break;
    case SIGNATURE_SETTINGS_ID:
      signature_settings_post((Signature_Settings*)tlv);
      break;
    default:
      break;
    }
  }
  else
  {
    vendorspecificdata_post(tlvid, (Vendor_Specific*)tlv);
  }
}

int8_t char2hex(char ch) {
  if ((ch >= '0') && (ch <= '9')) {
    return ch - '0';
  } else if ((ch >= 'a') && (ch <= 'f')) {
    return ch - 'a' + 10;
  } else if ((ch >= 'A') && (ch <= 'F')) {
    return ch - 'A' + 10;
  } else {
    return -1;
  }
}

int str2addr(char *str, uint8_t *addr) {
  uint32_t len;
  uint8_t offset = 0;
  int8_t i, ch;

  len = strlen(str);
  if ((len != 16))
    return -1;

  offset = 15;
  for (i = len - 1; i >= 0; i--) {
    ch = char2hex(str[i]);
    if (ch < 0)
      return -1;

    addr[offset / 2] |= (offset % 2)? ch: ch << 4;

    offset--;
  }

  return 0;
}

/**************************************************************
  usage: ./CsmpAgentLib_sample
          [-d NMS_ipv6_address]
          [-min reginterval_min]
          [-max reginterval_max]
          [-eid ieee_eui64]
***************************************************************/
int main(int argc, char **argv)
{
  struct timeval tv = {0};
  csmp_service_status_t status;
  csmp_service_stats_t *stats_ptr;
  char *status_msg[] = {"CSMP service is not started\n",
                       "Failed to start CSMP service\n",
                       "Registration is in progress...\n",
                       "Regist to the NMS successfully\n"};
  int ret, i;
  char *endptr;
  bool sigFlag = false;

  osal_gettime(&tv, NULL);
  g_init_time = tv.tv_sec;

  /**************************************************************
    init the dev_config parameter of csmp_service_start func:
      * NMS address
      * EUI64
      * register interval(min, max)
  ***************************************************************/
  memset(&g_devconfig, 0, sizeof(dev_config_t));
  osal_inet_pton(AF_INET6, NMS_IP, &g_devconfig.NMSaddr.s6_addr);
  memcpy(g_devconfig.ieee_eui64.data, g_eui64, sizeof(g_eui64));
  g_devconfig.reginterval_min = reg_interval_min;
  g_devconfig.reginterval_max = reg_interval_max;

  for (i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-min") == 0) {   // reginterval_min
      if (++i >= argc)
        goto start_error;
      g_devconfig.reginterval_min = strtol(argv[i], &endptr, 0);
      if (*endptr != '\0')
        goto start_error;
    } else if (strcmp(argv[i], "-max") == 0) {   // reginterval_max
      if (++i >= argc)
        goto start_error;
      g_devconfig.reginterval_max = strtol(argv[i], &endptr, 0);
      if (*endptr != '\0')
        goto start_error;
    } else if (strcmp(argv[i], "-eid") == 0) {   // EUI64 address
      if (++i >= argc)
        goto start_error;
      memset(g_devconfig.ieee_eui64.data, 0, sizeof(g_devconfig.ieee_eui64.data));
      if (str2addr(argv[i], g_devconfig.ieee_eui64.data) < 0)
        goto start_error;
      if (*endptr != '\0')
        goto start_error;
    } else if (strcmp(argv[i], "-d") == 0) {  // NMS address
      if (++i >= argc)
        goto start_error;
      if (osal_inet_pton(AF_INET6, argv[i], &g_devconfig.NMSaddr.s6_addr) <= 0) {
        printf("NMS address in presentation format\n");
        goto start_error;
      }
    } else if (strcmp(argv[i], "-sig") == 0) {  // Signature Settings
      if (++i >= argc)
        goto start_error;
      if (strcmp(argv[i], "true") == 0) {
        printf("setting signature settings to TRUE\n");
        sigFlag = true;
      }
    }
  }

  //Setting signature settings according to input in command line
  if (sigFlag)
  {
    //csmp signature settings data true
    g_devconfig.csmp_sig_settings.reqsignedpost = true;
    g_devconfig.csmp_sig_settings.reqvalidcheckpost = true;
    g_devconfig.csmp_sig_settings.reqtimesyncpost = true;
    g_devconfig.csmp_sig_settings.reqseclocalpost = true;
    g_devconfig.csmp_sig_settings.reqsignedresp = true;
    g_devconfig.csmp_sig_settings.reqvalidcheckresp = true;
    g_devconfig.csmp_sig_settings.reqtimesyncresp = true;
    g_devconfig.csmp_sig_settings.reqseclocalresp = true;
  }
  else
  {
    //csmp signature settings data false
    g_devconfig.csmp_sig_settings.reqsignedpost = false;
    g_devconfig.csmp_sig_settings.reqvalidcheckpost = false;
    g_devconfig.csmp_sig_settings.reqtimesyncpost = false;
    g_devconfig.csmp_sig_settings.reqseclocalpost = false;
    g_devconfig.csmp_sig_settings.reqsignedresp = false;
    g_devconfig.csmp_sig_settings.reqvalidcheckresp = false;
    g_devconfig.csmp_sig_settings.reqtimesyncresp = false;
    g_devconfig.csmp_sig_settings.reqseclocalresp = false;
  }

  /* check reginterval_max and reginterval_min */
  if (g_devconfig.reginterval_max < g_devconfig.reginterval_min
      || g_devconfig.reginterval_min == 0
      || g_devconfig.reginterval_max > 36000) {
    printf("reg interval error\n");

    goto start_error;
  }

  /*************************************************************
    init the csmp_handle parameter of csmp_service_start func:
      * callback function for the GET TLV request
      * callback function for the POST TLV request
      * callback function for the signature verification
  **************************************************************/
  g_csmp_handle.csmptlvs_get = (csmptlvs_get_t)csmptlvs_get;
  g_csmp_handle.csmptlvs_post = (csmptlvs_post_t)csmptlvs_post;
  g_csmp_handle.signature_verify = (signature_verify_t)signature_verify;

  // start csmp agent lib service
  ret = csmp_service_start(&g_devconfig, &g_csmp_handle);
  if(ret < 0)
    printf("start csmp agent service: fail!\n");
  else
    printf("start csmp agent service: success!\n");

  // get the regmin and regmax
  printf("min : %d, max = %d\n",g_devconfig.reginterval_min, g_devconfig.reginterval_max);

  while(1) {
    sleep(g_devconfig.reginterval_min);

    // get the service status
    status = csmp_service_status();
    printf("%s\n",status_msg[status]);

    // get the stats of CSMP agent service
    stats_ptr = csmp_service_stats();
    printf("-------------- CSMP service stats --------------\n");
    printf(" reg_succeed: %d\n reg_attempts: %d\n reg_fails: %d\n\
        \n *** reg_fail reason ***\n  error_coap: %d\n  error_signature: %d\n  error_process: %d\n\
        \n metrics_reports: %d\n csmp_get_succeed: %d\n csmp_post_succeed: %d\n\
        \n sig_ok: %d\n sig_no_signature: %d\n sig_bad_auth: %d\n sig_bad_validity: %d\n",\
        stats_ptr->reg_succeed,stats_ptr->reg_attempts,stats_ptr->reg_fails,\
        stats_ptr->reg_fails_stats.error_coap,stats_ptr->reg_fails_stats.error_signature,\
        stats_ptr->reg_fails_stats.error_process,stats_ptr->metrics_reports,\
        stats_ptr->csmp_get_succeed,stats_ptr->csmp_post_succeed,stats_ptr->sig_ok,\
        stats_ptr->sig_no_signature,stats_ptr->sig_bad_auth,stats_ptr->sig_bad_validity);
    printf("---------------------- end --------------------\n");
  }

  //stop csmp agent service
  ret = csmp_service_stop();
  if(ret)
    printf("stop csmp agent service: success!\n");
  else
    printf("stop csmp agent service: fail!\n");

  return 0;

start_error:
  printf("start csmp agent service: fail!\n");
}

