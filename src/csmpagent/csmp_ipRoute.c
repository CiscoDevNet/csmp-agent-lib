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

int csmp_get_ipRoute(tlvid_t tlvid, uint8_t *buf, size_t len)
{
  size_t rv = 0;
  uint32_t i, num;
  uint8_t *pbuf = buf;
  uint32_t used = 0;

  DPRINTF("csmpagent_ipRoute: start working.\n");


  IP_Route *ip_route = NULL;
  ip_route = g_csmptlvs_get(tlvid, &num);

  if(ip_route) {
    for(i = 0; i < num; i++) {
      IPRoute IPRouteMsg = IPROUTE__INIT;

      if(ip_route->has_inetcidrrouteindex) {
        IPRouteMsg.inet_cidr_route_index_present_case = IPROUTE__INET_CIDR_ROUTE_INDEX_PRESENT_INET_CIDR_ROUTE_INDEX;
        IPRouteMsg.inetcidrrouteindex = ip_route->inetcidrrouteindex;
      }
      if(ip_route->has_inetcidrroutedesttype) {
        IPRouteMsg.inet_cidr_route_dest_type_present_case = IPROUTE__INET_CIDR_ROUTE_DEST_TYPE_PRESENT_INET_CIDR_ROUTE_DEST_TYPE;
        IPRouteMsg.inetcidrroutedesttype = ip_route->inetcidrroutedesttype;
      }
      if(ip_route->has_inetcidrroutedest) {
        IPRouteMsg.inet_cidr_route_dest_present_case = IPROUTE__INET_CIDR_ROUTE_DEST_PRESENT_INET_CIDR_ROUTE_DEST;
        IPRouteMsg.inetcidrroutedest.len = ip_route->inetcidrroutedest.len;
        IPRouteMsg.inetcidrroutedest.data = ip_route->inetcidrroutedest.data;
      }
      if(ip_route->has_inetcidrroutepfxlen) {
        IPRouteMsg.inet_cidr_route_pfx_len_present_case = IPROUTE__INET_CIDR_ROUTE_PFX_LEN_PRESENT_INET_CIDR_ROUTE_PFX_LEN;
        IPRouteMsg.inetcidrroutepfxlen = ip_route->inetcidrroutepfxlen;
      }
      if(ip_route->has_inetcidrroutenexthoptype) {
        IPRouteMsg.inet_cidr_route_next_hop_type_present_case = IPROUTE__INET_CIDR_ROUTE_NEXT_HOP_TYPE_PRESENT_INET_CIDR_ROUTE_NEXT_HOP_TYPE;
        IPRouteMsg.inetcidrroutenexthoptype = ip_route->inetcidrroutenexthoptype;
      }
      if(ip_route->has_inetcidrroutenexthop) {
        IPRouteMsg.inet_cidr_route_next_hop_present_case = IPROUTE__INET_CIDR_ROUTE_NEXT_HOP_PRESENT_INET_CIDR_ROUTE_NEXT_HOP;
        IPRouteMsg.inetcidrroutenexthop.len = ip_route->inetcidrroutenexthop.len;
        IPRouteMsg.inetcidrroutenexthop.data = ip_route->inetcidrroutenexthop.data;
      }
      if(ip_route->has_inetcidrrouteifindex) {
        IPRouteMsg.inet_cidr_route_if_index_present_case = IPROUTE__INET_CIDR_ROUTE_IF_INDEX_PRESENT_INET_CIDR_ROUTE_IF_INDEX;
        IPRouteMsg.inetcidrrouteifindex = ip_route->inetcidrrouteifindex;
      }
      if(ip_route->has_inetcidrroutetype) {
        IPRouteMsg.inet_cidr_route_type_present_case = IPROUTE__INET_CIDR_ROUTE_TYPE_PRESENT_INET_CIDR_ROUTE_TYPE;
        IPRouteMsg.inetcidrroutetype = ip_route->inetcidrroutetype;
      }
      if(ip_route->inetcidrrouteproto) {
        IPRouteMsg.inet_cidr_route_proto_present_case = IPROUTE__INET_CIDR_ROUTE_PROTO_PRESENT_INET_CIDR_ROUTE_PROTO;
        IPRouteMsg.inetcidrrouteproto = ip_route->inetcidrrouteproto;
      }
      if(ip_route->has_inetcidrrouteage) {
        IPRouteMsg.inet_cidr_route_age_present_case = IPROUTE__INET_CIDR_ROUTE_AGE_PRESENT_INET_CIDR_ROUTE_AGE;
        IPRouteMsg.inetcidrrouteage = ip_route->inetcidrrouteage;
      }

      rv = csmptlv_write(pbuf, len-used, tlvid, (ProtobufCMessage *)&IPRouteMsg);
      if (rv == 0) {
        DPRINTF("csmpagent_ipRoute: csmptlv_write error!\n");
        return -1;
      }
      pbuf += rv; used += rv;
    }
  }
  DPRINTF("csmpagent_ipRoute: csmptlv_write [%ld] bytes to buffer!\n", rv);
  return used;
}
