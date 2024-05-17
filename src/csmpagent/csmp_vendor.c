/*
 *  Copyright 2023 Cisco Systems, Inc.
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

#include <string.h>
#include "csmp.h"
#include "csmpinfo.h"
#include "cgmsagent.h"
#include "csmptlv.h"
#include "csmpagent.h"
#include "CsmpTlvs.pb-c.h"
#include "osal.h"

int csmp_get_vendor(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  size_t rv = 0;
  uint32_t num;
  int used = 0;

  (void)tlvindex; // Suppress unused param warning.
  DPRINTF("csmp_get_vendor: start working.\n");
  Vendor VendorMsg = VENDOR__INIT;

  Vendor_Specific *vendor_data = NULL;
  vendor_data = g_csmptlvs_get(tlvid, &num);

  if (vendor_data == NULL)
  {
    DPRINTF("csmp_get_vendor: vendor_data null!\n");
    return ERROR;
  }

  VendorMsg.if_data_present_case = VENDOR__IF_DATA_PRESENT_IF_DATA;
  VendorMsg.ifdata.len = vendor_data->data.len;
  VendorMsg.ifdata.data = vendor_data->data.data;
  rv = csmptlv_write(buf, len, tlvid, (ProtobufCMessage *)&VendorMsg);
  if (rv == 0) {
    DPRINTF("csmp_get_vendor: csmptlv_write error!\n");
    return ERROR;
  }
  used += rv;

  DPRINTF("csmp_get_vendor: csmptlv_write [%u] bytes to buffer!\n", used);
  return used;
}

int csmp_put_vendor(tlvid_t tlvid, const uint8_t *buf, size_t len, uint8_t *out_buf, size_t out_size, size_t *out_len, int32_t tlvindex)
{
  Vendor *VendorMsg = NULL;
  Vendor_Specific vendor_data = VENDOR_INIT;
  tlvid_t tlvid0;
  uint32_t tlvlen;
  const uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;

  (void) tlvid; // Suppress unused param compiler warning.
  (void) out_buf; // Suppress unused param compiler warning.
  (void) out_size; // Suppress unused param compiler warning.
  (void) out_len; // Suppress unused param compiler warning.
  (void) tlvindex; // Suppress unused param compiler warning.

  DPRINTF("Received POST Vendor TLV\n");

  rv = csmptlv_readTL(pbuf, len, &tlvid0, &tlvlen);
  if ((rv == 0) || (tlvid0.vendor == 0)) {
    return ERROR;
  }
  pbuf += rv; used += rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&VendorMsg, &vendor__descriptor);
  if (rv == 0) {
    return ERROR;
  }
  pbuf += rv; used += rv;

  if (VendorMsg->if_data_present_case) {
    vendor_data.has_data = true;
    vendor_data.data.len = VendorMsg->ifdata.len;
    memcpy(vendor_data.data.data,VendorMsg->ifdata.data,vendor_data.data.len);
  }

  g_csmptlvs_post(tlvid0, &vendor_data);

  DPRINTF("Processed POST Vendor TLV\n");

  csmptlv_free((ProtobufCMessage *)VendorMsg);
  return used;
}
