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

#include "csmp.h"
#include "csmpinfo.h"
#include "csmptlv.h"
#include "csmpagent.h"
#include "CsmpTlvs.pb-c.h"

int csmp_get_firmwareImageInfo(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  size_t rv = 0;
  uint32_t num;

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
