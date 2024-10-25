/*
 *  Copyright 2023-2024 Cisco Systems, Inc.
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

#include <stdlib.h>
#include <string.h>
#include "csmp.h"
#include "csmpinfo.h"
#include "cgmsagent.h"
#include "csmptlv.h"
#include "csmpagent.h"
#include "CsmpTlvs.pb-c.h"

int csmp_get_vendorTlv(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  DPRINTF("** csmpagent_vendorTlv: GET for TLV %d Vendor-ID: %d\n", tlvid.type, tlvid.vendor);

  size_t rv = 0, used = 0;
  uint8_t *pbuf = buf;
  uint32_t num = 0, idx = 0;
  (void)tlvindex;

  VendorTlv VendorTlvMsg = VENDOR_TLV__INIT;
  Vendor_Tlv *vtlv = NULL;

  // Vendor-ID validation
  // Received Vendor-ID to match device's VENDOR_ID
  if (tlvid.vendor != VENDOR_ID) {
    DPRINTF("csmpagent_vendorTlv: csmptlv %d vendor-id mismatch (Expected:%d, Received:%d)\n", tlvid.type, VENDOR_ID, tlvid.vendor);
  }

  vtlv = g_csmptlvs_get(tlvid, &num);

  if (vtlv == NULL) {
    DPRINTF("csmpagent_vendorTlv: csmptlv %d GET failed!\n", tlvid.type);
    return CSMP_OP_TLV_RD_EMPTY;
  }
  
  while (used < len) {
    if (vtlv[idx].has_subtype) {
      VendorTlvMsg.sub_type_present_case = VENDOR_TLV__SUB_TYPE_PRESENT_SUB_TYPE; 
      VendorTlvMsg.subtype = vtlv[idx].subtype; 
    }
    if (vtlv[idx].has_value) {
      VendorTlvMsg.value_present_case = VENDOR_TLV__VALUE_PRESENT_VALUE; 
      VendorTlvMsg.value.len = vtlv[idx].value.len;
      VendorTlvMsg.value.data = vtlv[idx].value.data;
    }
    DPRINTF("Vendor TLV: GET[%d], ", idx);
    DPRINTF("subtype=%d, ", vtlv[idx].subtype);
    DPRINTF("value=[len=%lu, data=(0x%02x,0x%02x,0x%02x...0x%02x,0x%02x,0x%02x)]\n", 
            vtlv[idx].value.len, 
            vtlv[idx].value.data[0],
            vtlv[idx].value.data[1],
            vtlv[idx].value.data[2],
            vtlv[idx].value.data[VENDOR_MAX_DATA_LEN-3],
            vtlv[idx].value.data[VENDOR_MAX_DATA_LEN-2],
            vtlv[idx].value.data[VENDOR_MAX_DATA_LEN-1]);

    rv = csmptlv_write(pbuf, len, tlvid, (ProtobufCMessage *)&VendorTlvMsg);
    DPRINTF("csmpagent_vendorTlv: TLV127 idx=%d rv=%ld used=%ld len=%ld\n", idx, rv, used, len);
    DPRINTF("csmpagent_vendorTlv: csmptlv %d wrote [%ld] bytes to buffer for sub-type=%d\n", tlvid.type, rv, vtlv[idx].subtype);
    if (rv == 0) {
      DPRINTF("csmpagent_vendorTlv: csmptlv %d write error!\n", tlvid.type);
      return CSMP_OP_TLV_WR_ERROR;
    }
    pbuf += rv;
    used += rv;
    
    idx++;
    if (idx >= num) {
      DPRINTF("csmpagent_vendorTlv: Processed %d VendorTlv sub-types\n", idx);
      break;
    }
  }
  DPRINTF("csmpagent_vendorTlv: csmptlv %d wrote total [%ld] bytes to buffer!\n", tlvid.type, used);
  DPRINTF("** csmpagent_vendorTlv: GET for TLV %d Vendor-ID: %d done\n", tlvid.type, tlvid.vendor);
  return used;
}

int csmp_put_vendorTlv(tlvid_t tlvid, const uint8_t *buf, size_t len, uint8_t *out_buf, size_t out_size, size_t *out_len, int32_t tlvindex)
{
  DPRINTF("** csmpagent_vendorTlv: POST for TLV %d Vendor-ID: %d\n", tlvid.type, tlvid.vendor);
  
  uint32_t tlvlen;
  size_t rv = 0, used = 0;
  const uint8_t *pbuf = buf;
  tlvid_t tlvid_in;
  (void) tlvid;
  (void) out_buf;
  (void) out_size;
  (void) out_len;
  (void) tlvindex;

  VendorTlv *VendorTlvMsg = NULL;
  Vendor_Tlv vtlv = VENDOR_TLV_INIT;

  rv = csmptlv_readTL(pbuf, len, &tlvid_in, &tlvlen);
  if ((rv == 0) || (tlvid_in.type != VENDOR_TLVID)) {
    DPRINTF("csmpagent_vendorTlv: csmptlv %d vendor-id %d read error!\n", tlvid_in.type, tlvid_in.vendor);    return CSMP_OP_TLV_RD_ERROR;
  }
  // Vendor-ID validation
  // Received Vendor-ID to match device's VENDOR_ID
  if (tlvid_in.vendor != VENDOR_ID) {
    DPRINTF("csmpagent_vendorTlv: csmptlv %d vendor-id mismatch (Expected:%d, Received:%d)\n", tlvid_in.type, VENDOR_ID, tlvid_in.vendor);
  }
  pbuf += rv; used += rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&VendorTlvMsg,
                     &vendor_tlv__descriptor);
  if (rv == 0) {
    DPRINTF("csmpagent_vendorTlv: csmptlv %d vendor-id %d read error!\n", tlvid.type, tlvid.vendor);
    return CSMP_OP_TLV_RD_ERROR;
  }
  pbuf += rv; used += rv;

  DPRINTF("Vendor TLV: POST\n");
  DPRINTF("subtype=%d\n", VendorTlvMsg->subtype);
  DPRINTF("value=[len=%lu, data=(0x%02x,0x%02x,0x%02x...0x%02x,0x%02x,0x%02x)]\n", 
          VendorTlvMsg->value.len, 
          VendorTlvMsg->value.data[0],
          VendorTlvMsg->value.data[1],
          VendorTlvMsg->value.data[2],
          VendorTlvMsg->value.data[VENDOR_MAX_DATA_LEN-3],
          VendorTlvMsg->value.data[VENDOR_MAX_DATA_LEN-2],
          VendorTlvMsg->value.data[VENDOR_MAX_DATA_LEN-1]);
   
  if ((!VendorTlvMsg->sub_type_present_case) ||
      (!VendorTlvMsg->value_present_case)) {
    DPRINTF("csmpagent_vendorTlv: csmptlv %d vendor-id %d insufficient data error!\n", tlvid.type, tlvid.vendor);
    csmptlv_free((ProtobufCMessage *)VendorTlvMsg);
    return CSMP_OP_FAILED;
  }
  
  memset(&vtlv, 0, sizeof(vtlv));
  vtlv.has_subtype = true; 
  vtlv.subtype = VendorTlvMsg->subtype;
  vtlv.has_value = true;
  vtlv.value.len = (VendorTlvMsg->value.len <= sizeof(vtlv.value.data)) ?
                    VendorTlvMsg->value.len : sizeof(vtlv.value.data);
  memcpy(&vtlv.value.data, VendorTlvMsg->value.data, vtlv.value.len);

  g_csmptlvs_post(tlvid, &vtlv);

  csmptlv_free((ProtobufCMessage *)VendorTlvMsg);
  DPRINTF("** csmpagent_vendorTlv: POST for TLV %d Vendor-ID: %d done\n", tlvid.type, tlvid.vendor);
  return used;
}

