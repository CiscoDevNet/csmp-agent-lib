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
#include "CsmpTlvs.pb-c.h"

int csmp_get_ipAddress(tlvid_t tlvid, uint8_t *buf, size_t len)
{
  size_t rv = 0;
  uint32_t i, num;
  uint8_t *pbuf = buf;
  uint32_t used = 0;

  DPRINTF("csmpagent_ipAddress: start working.\n");


  IP_Address *ip_address = NULL;
  ip_address = g_csmptlvs_get(tlvid, &num);

  if(ip_address) {
    for(i = 0; i < num; i++) {
      IPAddress IPAddressMsg = IPADDRESS__INIT;

      if(ip_address[i].has_ipaddressindex) {
        IPAddressMsg.ip_address_index_present_case = IPADDRESS__IP_ADDRESS_INDEX_PRESENT_IP_ADDRESS_INDEX;
        IPAddressMsg.ipaddressindex = ip_address[i].ipaddressindex;
      }
      if(ip_address[i].has_ipaddressaddrtype) {
        IPAddressMsg.ip_address_addr_type_present_case = IPADDRESS__IP_ADDRESS_ADDR_TYPE_PRESENT_IP_ADDRESS_ADDR_TYPE;
        IPAddressMsg.ipaddressaddrtype = ip_address[i].ipaddressaddrtype;
      }
      if(ip_address[i].has_ipaddressaddr) {
        IPAddressMsg.ip_address_addr_present_case = IPADDRESS__IP_ADDRESS_ADDR_PRESENT_IP_ADDRESS_ADDR;
        IPAddressMsg.ipaddressaddr.len = ip_address[i].ipaddressaddr.len;
        IPAddressMsg.ipaddressaddr.data = ip_address[i].ipaddressaddr.data;
      }
      if(ip_address[i].has_ipaddressifindex) {
        IPAddressMsg.ip_address_if_index_present_case = IPADDRESS__IP_ADDRESS_IF_INDEX_PRESENT_IP_ADDRESS_IF_INDEX;
        IPAddressMsg.ipaddressifindex = ip_address[i].ipaddressifindex;
      }
      if(ip_address[i].has_ipaddresstype) {
        IPAddressMsg.ip_address_type_present_case = IPADDRESS__IP_ADDRESS_TYPE_PRESENT_IP_ADDRESS_TYPE;
        IPAddressMsg.ipaddresstype = ip_address[i].ipaddresstype;
      }
      if(ip_address[i].has_ipaddressorigin) {
        IPAddressMsg.ip_address_origin_present_case = IPADDRESS__IP_ADDRESS_ORIGIN_PRESENT_IP_ADDRESS_ORIGIN;
        IPAddressMsg.ipaddressorigin = ip_address[i].ipaddressorigin;
      }
      if(ip_address[i].has_ipaddressstatus) {
        IPAddressMsg.ip_address_status_present_case = IPADDRESS__IP_ADDRESS_STATUS_PRESENT_IP_ADDRESS_STATUS;
        IPAddressMsg.ipaddressstatus = ip_address[i].ipaddressstatus;
      }
      if(ip_address[i].has_ipaddresscreated) {
        IPAddressMsg.ip_address_created_present_case = IPADDRESS__IP_ADDRESS_CREATED_PRESENT_IP_ADDRESS_CREATED;
        IPAddressMsg.ipaddresscreated = ip_address[i].ipaddresscreated;
      }
      if(ip_address[i].has_ipaddresslastchanged) {
        IPAddressMsg.ip_address_last_changed_present_case = IPADDRESS__IP_ADDRESS_LAST_CHANGED_PRESENT_IP_ADDRESS_LAST_CHANGED;
        IPAddressMsg.ipaddresslastchanged = ip_address[i].ipaddresslastchanged;
      }
      if(ip_address[i].has_ipaddresspfxlen) {
        IPAddressMsg.ip_address_pfx_len_present_case = IPADDRESS__IP_ADDRESS_PFX_LEN_PRESENT_IP_ADDRESS_PFX_LEN;
        IPAddressMsg.ipaddresspfxlen = ip_address[i].ipaddresspfxlen;
      }

      rv = csmptlv_write(pbuf, len-used, tlvid, (ProtobufCMessage *)&IPAddressMsg);
      if (rv == 0) {
        DPRINTF("csmpagent_ipAddress: csmptlv_write error!\n");
        return -1;
      }
      pbuf += rv; used += rv;
    }
  }
  DPRINTF("csmpagent_ipAddress: csmptlv_write [%ld] bytes to buffer!\n", rv);

  return used;
}
