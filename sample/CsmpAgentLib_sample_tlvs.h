/*
 *  Copyright 2024 Cisco Systems, Inc.
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

#ifndef _SAMPLE_TLVS_H_
#define _SAMPLE_TLVS_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "csmp_service.h"
#include "csmp_info.h"

extern dev_config_t g_devconfig;
extern csmp_handle_t g_csmp_handle;
extern uint32_t g_init_time;
extern bool g_reboot_request;

/**
 * @brief pubkey_get (portable)
 *
 * @param key pointer to pubkey
 */
void pubkey_get(char *key);

/**
 * @brief hardware description function (portable)
 *
 * @param num amount of instances (array index) of hardware Descriptions
 * @return void* pointer to global variable g_hardwareDesc
 */
void* hardware_desc_get(uint32_t *num);

/**
 * @brief interface description function (portable)
 *
 * @param num  amount of instances (array index) of interface Descriptions
 * @return void* pointer to global variable g_interfaceDesc
 */
void* interface_desc_get(uint32_t *num);

/**
 * @brief function to set the ip Addresses. (portable)
 *
 * @param num amount of instances (array index) of ipAddresses
 * @return void* pointer to global variable g_ipAddress
 */
void* ipaddress_get(uint32_t *num);

/**
 * @brief set up ip Route information (portable)
 *
 * @param num amount of instances of g_ipRoute
 * @return void* pointer to g_ipRoute
 */
void* iproute_get(uint32_t *num);

/**
 * @brief set up the current time (portable)
 *
 * @param num amount of instances
 * @return void* pointer to g_currentTime
 */
void* currenttime_get(uint32_t *num);


/**
 * @brief set the current time (portable)
 *
 * @param tlv
 */
void currenttime_post(Current_Time *tlv);

/**
 * @brief Get the uptime
 *
 * @param num amount of instances in g_uptime (portable)
 * @return void* point to g_uptime
 */
void* uptime_get(uint32_t *num);

/**
 * @brief interface metrics information setup (portable)
 *
 * @param num num mount of instances of g_interfaceMetrics
 * @return void* pointer to global g_interfaceMetrics
 */
void* interface_metrics_get(uint32_t *num);

/**
 * @brief iproute RPL information setup (portable)
 *
 * @param num num mount of instances of g_iprouteRplmetrics
 * @return void* pointer to global g_iprouteRplmetrics
 */
void* iproute_rplmetrics_get(uint32_t *num);


/**
 * @brief wpan information setup (portable)
 *
 * @param num num mount of instances of g_wpanStatus
 * @return void* pointer to global g_wpanStatus
 */
void* wpanstatus_get(uint32_t *num);


/**
 * @brief RPL instance information (portable)
 *
 * @param num mount of instances of g_rplInstance
 * @return void* pointer to global g_rplInstance
 */
void* rplinstance_get(uint32_t *num);

/**
 * @brief firmware information (portable)
 *
 * @param num amount of instances of g_firmwareImageInfo
 * @return void* pointer to global g_firmwareImageInfo
 */
void* firmwareImageInfo_get(tlvid_t tlvid, uint32_t *num);


/**
 * @brief get the signature settings (portable)
 *
 * @param num amount of instances
 * @return void* pointer to global g_SignatureSettings
 */
void* signature_settings_get(uint32_t *num);

/**
 * @brief set the signature settings (portable)
 *
 * @param tlv
 */
void signature_settings_post(Signature_Settings *tlv);

/**
 * @brief get vendor tlv (portable)
 *
 * @param num amount of instances
 * @return void* pointer to global g_vendorTlv
 */
void* vendorTlv_get(tlvid_t tlvid, uint32_t *num);

/**
 * @brief set vendor tlv (portable)
 *
 * @param num amount of instances
 * @return void* pointer to global g_vendorTlv
 */
void vendorTlv_post(tlvid_t tlvid, Vendor_Tlv *tlv);

/**
 * @brief csmp get TLV request
 *
 * @param tlvid the tlvid to handle
 * @param num returned amount of instances
 * @return void* pointer to the global variable containing the return data
 */
void* csmptlvs_get(tlvid_t tlvid, uint32_t *num);

/**
 * @brief csmp post TLV request
 *
 * @param tlvid the tlvid to handle
 * @param tlv the request data
 */
void csmptlvs_post(tlvid_t tlvid, void *tlv);

/**
 * @brief transfer request get TLV request
 *
 * @param tlvid the tlvid to handle
 * @param tlv the request data
 */
void* transferRequest_get(tlvid_t tlvid, uint32_t *num);

/**
 * @brief transfer request post TLV request
 *
 * @param tlvid the tlvid to handle
 * @param tlv the request data
 */
void transferRequest_post(tlvid_t tlvid, Transfer_Request *tlv);

/**
 * @brief loadRequest get TLV request
 *
 * @param tlvid the tlvid to handle
 * @param tlv the request data
 */
void* loadRequest_get(tlvid_t tlvid, uint32_t *num);

/**
 * @brief loadRequest post TLV request
 *
 * @param tlvid the tlvid to handle
 * @param tlv the request data
 */
void loadRequest_post(tlvid_t tlvid, Load_Request *tlv);

/**
 * @brief cancelLoadRequest post TLV request
 *
 * @param tlvid the tlvid to handle
 * @param tlv the request data
 */
void cancelLoadRequest_post(tlvid_t tlvid, Cancel_Load_Request *tlv);

/**
 * @brief image block post TLV request
 *
 * @param tlvid the tlvid to handle
 * @param tlv the request data
 */
void imageBlock_post(tlvid_t tlvid, Image_Block *tlv);

/**
 * @brief setBackupRequest post TLV request
 *
 * @param tlvid the tlvid to handle
 * @param tlv the request data
 */
void setBackupRequest_post(tlvid_t tlvid, Set_Backup_Request *tlv);

#endif // _SAMPLE_TLVS_H_
