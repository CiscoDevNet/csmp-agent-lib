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

int csmp_get_interfaceDesc(tlvid_t tlvid, uint8_t *buf, size_t len)
{
  size_t rv = 0;
  uint32_t i, num;
  uint8_t *pbuf = buf;
  uint32_t used = 0;

  DPRINTF("csmpagent_interfaceDesc: start working.\n");


  Interface_Desc *interface_desc = NULL;
  interface_desc = g_csmptlvs_get(tlvid, &num);

  if(interface_desc) {
    for(i = 0; i < num; i++) {
      InterfaceDesc InterfaceDescMsg = INTERFACE_DESC__INIT;
      if(interface_desc[i].has_ifindex) {
        InterfaceDescMsg.if_index_present_case = INTERFACE_DESC__IF_INDEX_PRESENT_IF_INDEX;
        InterfaceDescMsg.ifindex = interface_desc[i].ifindex;
      }
      if(interface_desc[i].has_ifname) {
        InterfaceDescMsg.if_name_present_case = INTERFACE_DESC__IF_NAME_PRESENT_IF_NAME;
        InterfaceDescMsg.ifname = interface_desc[i].ifname;
      }
      if(interface_desc[i].has_ifdescr) {
        InterfaceDescMsg.if_descr_present_case = INTERFACE_DESC__IF_DESCR_PRESENT_IF_DESCR;
        InterfaceDescMsg.ifdescr = interface_desc[i].ifdescr;
      }
      if(interface_desc[i].has_iftype) {
        InterfaceDescMsg.if_type_present_case = INTERFACE_DESC__IF_TYPE_PRESENT_IF_TYPE;
        InterfaceDescMsg.iftype = interface_desc[i].iftype;
      }
      if(interface_desc[i].has_ifmtu) {
        InterfaceDescMsg.if_mtu_present_case = INTERFACE_DESC__IF_MTU_PRESENT_IF_MTU;
        InterfaceDescMsg.ifmtu = interface_desc[i].ifmtu;
      }
      if(interface_desc[i].has_ifphysaddress) {
        InterfaceDescMsg.if_phys_address_present_case = INTERFACE_DESC__IF_PHYS_ADDRESS_PRESENT_IF_PHYS_ADDRESS;
        InterfaceDescMsg.ifphysaddress.len = interface_desc[i].ifphysaddress.len;
        InterfaceDescMsg.ifphysaddress.data = interface_desc[i].ifphysaddress.data;
      }

      rv = csmptlv_write(pbuf, len-used, tlvid, (ProtobufCMessage *)&InterfaceDescMsg);
      if (rv == 0) {
        DPRINTF("csmpagent_interfaceDesc: csmptlv_write error!\n");
        return -1;
      }
      pbuf += rv; used += rv;
    }
  }
  DPRINTF("csmpagent_interfaceDesc: csmptlv_write [%u] bytes to buffer!\n", used);
  return used;

}
