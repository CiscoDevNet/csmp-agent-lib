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

#include "csmp.h"
#include "csmpinfo.h"
#include "csmptlv.h"
#include "csmpagent.h"
#include "csmpfunction.h"
#include "CsmpTlvs.pb-c.h"

int csmp_get_firmwareImageInfo(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  size_t rv = 0;
  uint32_t num;

  (void)tlvindex; // Suppress unused param warning.
  DPRINTF("csmpagent_firmwareImageInfo: start working.\n");

  FirmwareImageInfo FirmwareImageInfoMsg = FIRMWARE_IMAGE_INFO__INIT;
  HardwareInfo HardwareInfoMsg = HARDWARE_INFO__INIT;

  Firmware_Image_Info *firmware_image_info = NULL;
  firmware_image_info = g_csmptlvs_get(tlvid, &num);

  if(firmware_image_info) {
    if(firmware_image_info->has_index) {
      FirmwareImageInfoMsg.index_present_case = FIRMWARE_IMAGE_INFO__INDEX_PRESENT_INDEX;
      FirmwareImageInfoMsg.index = firmware_image_info->index;
    }
    if(firmware_image_info->has_filehash) {
      FirmwareImageInfoMsg.file_hash_present_case = FIRMWARE_IMAGE_INFO__FILE_HASH_PRESENT_FILE_HASH;
      FirmwareImageInfoMsg.filehash.len = firmware_image_info->filehash.len;
      FirmwareImageInfoMsg.filehash.data = firmware_image_info->filehash.data;
    }
   if(firmware_image_info->has_filename) {
      FirmwareImageInfoMsg.file_name_present_case = FIRMWARE_IMAGE_INFO__FILE_NAME_PRESENT_FILE_NAME;
      FirmwareImageInfoMsg.filename = firmware_image_info->filename;
    }
   if(firmware_image_info->has_version) {
      FirmwareImageInfoMsg.version_present_case = FIRMWARE_IMAGE_INFO__VERSION_PRESENT_VERSION;
      FirmwareImageInfoMsg.version = firmware_image_info->version;
    }
   if(firmware_image_info->has_filesize) {
      FirmwareImageInfoMsg.file_size_present_case = FIRMWARE_IMAGE_INFO__FILE_SIZE_PRESENT_FILE_SIZE;
      FirmwareImageInfoMsg.filesize = firmware_image_info->filesize;
    }
    if(firmware_image_info->has_blocksize) {
      FirmwareImageInfoMsg.block_size_present_case = FIRMWARE_IMAGE_INFO__BLOCK_SIZE_PRESENT_BLOCK_SIZE;
      FirmwareImageInfoMsg.blocksize = firmware_image_info->blocksize;
    }
    if(firmware_image_info->has_bitmap) {
      FirmwareImageInfoMsg.bitmap_present_case = FIRMWARE_IMAGE_INFO__BITMAP_PRESENT_BITMAP;
      FirmwareImageInfoMsg.bitmap.len = firmware_image_info->bitmap.len;
      FirmwareImageInfoMsg.bitmap.data = firmware_image_info->bitmap.data;
    }
    if(firmware_image_info->has_isdefault) {
      FirmwareImageInfoMsg.is_default_present_case = FIRMWARE_IMAGE_INFO__IS_DEFAULT_PRESENT_IS_DEFAULT;
      FirmwareImageInfoMsg.isdefault = firmware_image_info->isdefault;
    }
    if(firmware_image_info->has_isrunning) {
      FirmwareImageInfoMsg.is_running_present_case = FIRMWARE_IMAGE_INFO__IS_RUNNING_PRESENT_IS_RUNNING;
      FirmwareImageInfoMsg.isrunning = firmware_image_info->isrunning;
    }
    if(firmware_image_info->has_loadtime) {
      FirmwareImageInfoMsg.load_time_present_case = FIRMWARE_IMAGE_INFO__LOAD_TIME_PRESENT_LOAD_TIME;
      FirmwareImageInfoMsg.loadtime = firmware_image_info->loadtime;
    }
    if(firmware_image_info->has_hwinfo) {
      FirmwareImageInfoMsg.hwinfo = &HardwareInfoMsg;
      if(firmware_image_info->hwinfo.has_hwid) {
        HardwareInfoMsg.hw_id_present_case = HARDWARE_INFO__HW_ID_PRESENT_HW_ID;
        HardwareInfoMsg.hwid = firmware_image_info->hwinfo.hwid;
      }
      if(firmware_image_info->hwinfo.has_vendorhwid) {
        HardwareInfoMsg.vendor_hw_id_present_case = HARDWARE_INFO__VENDOR_HW_ID_PRESENT_VENDOR_HW_ID;
        HardwareInfoMsg.vendorhwid = firmware_image_info->hwinfo.vendorhwid;
      }
    }

    rv = csmptlv_write(buf, len, tlvid, (ProtobufCMessage *)&FirmwareImageInfoMsg);
    if (rv == 0) {
      DPRINTF("csmpagent_firmwareImageInfo: csmptlv_write error!\n");
      return -1;
    }
  }

  DPRINTF("csmpagent_firmwareImageInfo: csmptlv_write [%ld] bytes to buffer!\n", rv);
  return rv;
}
