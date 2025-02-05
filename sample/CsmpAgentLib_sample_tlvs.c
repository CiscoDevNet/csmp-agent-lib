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

#include <inttypes.h>
#include "CsmpAgentLib_sample_tlvs.h"


/**
 * @brief csmp get TLV request
 *
 * @param tlvid the tlvid to handle
 * @param num returned amount of instances
 * @return void* pointer to the global variable containing the return data
 */
void* csmptlvs_get(tlvid_t tlvid, uint32_t *num) {
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
    case TRANSFER_REQUEST_ID:
      return transferRequest_get(tlvid, num);
    case LOAD_REQUEST_ID:
      return loadRequest_get(tlvid, num);
    case FIRMWARE_IMAGE_INFO_ID:
      return firmwareImageInfo_get(tlvid, num);
    case SIGNATURE_SETTINGS_ID:
      return signature_settings_get(num);
    case VENDOR_TLV_ID:
      return vendorTlv_get(tlvid, num);

    default:
      printf("sample_get: GET un-supported for TLV: %" PRIuLEAST32 ".%" PRIuLEAST32 "\n", tlvid.vendor, tlvid.type);
      break;
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
    switch(tlvid.type) {
    case CURRENT_TIME_ID:
      currenttime_post((Current_Time*)tlv);
      break;
    case TRANSFER_REQUEST_ID:
      transferRequest_post(tlvid, (Transfer_Request*)tlv);
      break;
    case IMAGE_BLOCK_ID:
      imageBlock_post(tlvid, (Image_Block*)tlv);
      break;
    case LOAD_REQUEST_ID:
      loadRequest_post(tlvid, (Load_Request*)tlv);
      break;
    case CANCEL_LOAD_REQUEST_ID:
      cancelLoadRequest_post(tlvid, (Cancel_Load_Request*)tlv);
      break;
    case SET_BACKUP_REQUEST_ID:
      setBackupRequest_post(tlvid, (Set_Backup_Request*)tlv);
      break;
    case SIGNATURE_SETTINGS_ID:
      signature_settings_post((Signature_Settings*)tlv);
      break;
    case VENDOR_TLV_ID:
      vendorTlv_post(tlvid, (Vendor_Tlv*)tlv);
      break;
    case REBOOT_REQUEST_ID:
      rebootRequest_post(tlvid, (Reboot_Request*)tlv);
      break;

    default:
      printf("sample_post: POST un-supported for TLV: %" PRIuLEAST32 ".%" PRIuLEAST32 "\n", tlvid.vendor, tlvid.type);
      break;
    }
}
