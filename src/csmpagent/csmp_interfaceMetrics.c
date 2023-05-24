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

#include <string.h>
#include "csmp.h"
#include "csmpinfo.h"
#include "csmptlv.h"
#include "csmpagent.h"
#include "csmpfunction.h"
#include "CsmpTlvs.pb-c.h"

int csmp_get_interfaceMetrics(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  size_t rv = 0;
  uint32_t i, num;
  uint8_t *pbuf = buf;
  uint32_t used = 0;

  (void)tlvindex; // Suppress unused param warning.
  DPRINTF("csmpagent_interfaceMetrics: start working.\n");

  Interface_Metrics *interface_metrics = NULL;
  interface_metrics = g_csmptlvs_get(tlvid, &num);

  if(interface_metrics) {
    for(i = 0; i < num; i++) {
      InterfaceMetrics InterfaceMetricsMsg = INTERFACE_METRICS__INIT;

      if(interface_metrics[i].has_ifindex) {
        InterfaceMetricsMsg.if_index_present_case = INTERFACE_METRICS__IF_INDEX_PRESENT_IF_INDEX;
        InterfaceMetricsMsg.ifindex = interface_metrics[i].ifindex;
      }
      if(interface_metrics[i].has_ifinspeed) {
        InterfaceMetricsMsg.if_in_speed_present_case = INTERFACE_METRICS__IF_IN_SPEED_PRESENT_IF_IN_SPEED;
        InterfaceMetricsMsg.ifinspeed = interface_metrics[i].ifinspeed;
      }
      if(interface_metrics[i].has_ifoutspeed) {
        InterfaceMetricsMsg.if_out_speed_present_case = INTERFACE_METRICS__IF_OUT_SPEED_PRESENT_IF_OUT_SPEED;
        InterfaceMetricsMsg.ifoutspeed = interface_metrics[i].ifoutspeed;
      }
      if(interface_metrics[i].has_ifadminstatus) {
        InterfaceMetricsMsg.if_admin_status_present_case = INTERFACE_METRICS__IF_ADMIN_STATUS_PRESENT_IF_ADMIN_STATUS;
        InterfaceMetricsMsg.ifadminstatus = interface_metrics[i].ifadminstatus;
      }
      if(interface_metrics[i].has_ifoperstatus) {
        InterfaceMetricsMsg.if_oper_status_present_case = INTERFACE_METRICS__IF_OPER_STATUS_PRESENT_IF_OPER_STATUS;
        InterfaceMetricsMsg.ifoperstatus = interface_metrics[i].ifoperstatus;
      }
      if(interface_metrics[i].has_iflastchange) {
        InterfaceMetricsMsg.if_last_change_present_case = INTERFACE_METRICS__IF_LAST_CHANGE_PRESENT_IF_LAST_CHANGE;
        InterfaceMetricsMsg.iflastchange = interface_metrics[i].iflastchange;
      }
      if(interface_metrics[i].has_ifinoctets) {
        InterfaceMetricsMsg.if_in_octets_present_case = INTERFACE_METRICS__IF_IN_OCTETS_PRESENT_IF_IN_OCTETS;
        InterfaceMetricsMsg.ifinoctets = interface_metrics[i].ifinoctets;
      }
      if(interface_metrics[i].has_ifoutoctets) {
        InterfaceMetricsMsg.if_out_octets_present_case = INTERFACE_METRICS__IF_OUT_OCTETS_PRESENT_IF_OUT_OCTETS;
        InterfaceMetricsMsg.ifoutoctets = interface_metrics[i].ifoutoctets;
      }
      if(interface_metrics[i].has_ifindiscards) {
        InterfaceMetricsMsg.if_in_discards_present_case = INTERFACE_METRICS__IF_IN_DISCARDS_PRESENT_IF_IN_DISCARDS;
        InterfaceMetricsMsg.ifindiscards = interface_metrics[i].ifindiscards;
      }
      if(interface_metrics[i].has_ifinerrors) {
        InterfaceMetricsMsg.if_in_errors_present_case = INTERFACE_METRICS__IF_IN_ERRORS_PRESENT_IF_IN_ERRORS;
        InterfaceMetricsMsg.ifinerrors = interface_metrics[i].ifinerrors;
      }
      if(interface_metrics[i].has_ifoutdiscards) {
        InterfaceMetricsMsg.if_out_discards_present_case = INTERFACE_METRICS__IF_OUT_DISCARDS_PRESENT_IF_OUT_DISCARDS;
        InterfaceMetricsMsg.ifoutdiscards = interface_metrics[i].ifoutdiscards;
      }
      if(interface_metrics[i].has_ifouterrors) {
        InterfaceMetricsMsg.if_out_errors_present_case = INTERFACE_METRICS__IF_OUT_ERRORS_PRESENT_IF_OUT_ERRORS;
        InterfaceMetricsMsg.ifouterrors = interface_metrics[i].ifouterrors;
      }

      rv = csmptlv_write(pbuf, len-used, tlvid, (ProtobufCMessage *)&InterfaceMetricsMsg);
      if (rv == 0) {
        DPRINTF("csmpagent_interfaceMetrics: csmptlv_write error!\n");
        return -1;
      }
      pbuf += rv; used += rv;
    }
  }
  DPRINTF("csmpagent_interfaceMetrics: csmptlv_write [%u] bytes to buffer!\n", used);

  return used;
}
