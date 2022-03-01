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
#include "CsmpTlvs.pb-c.h"

int csmp_get_ipRouteRplMetrics(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  size_t rv = 0;
  uint32_t i, num;
  uint8_t *pbuf = buf;
  uint32_t used = 0;

  DPRINTF("csmpagent_ipRouteRplMetrics: start working.\n");
  IPRouteRPLMetrics IPRouteRPLMetricsMsg = IPROUTE_RPLMETRICS__INIT;

  IPRoute_RPLMetrics *iproute_rplmetrics = NULL;
  iproute_rplmetrics = g_csmptlvs_get(tlvid, &num);

  if(iproute_rplmetrics) {
    if(iproute_rplmetrics->has_inetcidrrouteindex) {
      IPRouteRPLMetricsMsg.inet_cidr_route_index_present_case = IPROUTE_RPLMETRICS__INET_CIDR_ROUTE_INDEX_PRESENT_INET_CIDR_ROUTE_INDEX;
      IPRouteRPLMetricsMsg.inetcidrrouteindex = iproute_rplmetrics->inetcidrrouteindex;
    }

    if(iproute_rplmetrics->has_instanceindex) {
      IPRouteRPLMetricsMsg.instance_index_present_case = IPROUTE_RPLMETRICS__INSTANCE_INDEX_PRESENT_INSTANCE_INDEX;
      IPRouteRPLMetricsMsg.instanceindex = iproute_rplmetrics->instanceindex;
    }

    if(iproute_rplmetrics->has_rank) {
      IPRouteRPLMetricsMsg.rank_present_case = IPROUTE_RPLMETRICS__RANK_PRESENT_RANK;
      IPRouteRPLMetricsMsg.rank = iproute_rplmetrics->rank;
    }

    if(iproute_rplmetrics->has_hops) {
      IPRouteRPLMetricsMsg.hops_present_case = IPROUTE_RPLMETRICS__HOPS_PRESENT_HOPS;
      IPRouteRPLMetricsMsg.hops = iproute_rplmetrics->hops;
    }

    if(iproute_rplmetrics->has_pathetx) {
      IPRouteRPLMetricsMsg.path_etx_present_case = IPROUTE_RPLMETRICS__PATH_ETX_PRESENT_PATH_ETX;
      IPRouteRPLMetricsMsg.pathetx = iproute_rplmetrics->pathetx;
    }

    if(iproute_rplmetrics->has_linketx) {
      IPRouteRPLMetricsMsg.link_etx_present_case = IPROUTE_RPLMETRICS__LINK_ETX_PRESENT_LINK_ETX;
      IPRouteRPLMetricsMsg.linketx = iproute_rplmetrics->linketx;
    }

    if(iproute_rplmetrics->has_rssiforward) {
      IPRouteRPLMetricsMsg.rssi_forward_present_case = IPROUTE_RPLMETRICS__RSSI_FORWARD_PRESENT_RSSI_FORWARD;
      IPRouteRPLMetricsMsg.rssiforward = iproute_rplmetrics->rssiforward;
    }

    if(iproute_rplmetrics->has_rssireverse) {
      IPRouteRPLMetricsMsg.rssi_reverse_present_case = IPROUTE_RPLMETRICS__RSSI_REVERSE_PRESENT_RSSI_REVERSE;
      IPRouteRPLMetricsMsg.rssireverse = iproute_rplmetrics->rssireverse;
    }

    if(iproute_rplmetrics->has_lqiforward) {
      IPRouteRPLMetricsMsg.lqi_forward_present_case = IPROUTE_RPLMETRICS__LQI_FORWARD_PRESENT_LQI_FORWARD;
      IPRouteRPLMetricsMsg.lqiforward = iproute_rplmetrics->lqiforward;
    }

    if(iproute_rplmetrics->has_lqireverse) {
      IPRouteRPLMetricsMsg.lqi_reverse_present_case = IPROUTE_RPLMETRICS__LQI_REVERSE_PRESENT_LQI_REVERSE;
      IPRouteRPLMetricsMsg.lqireverse = iproute_rplmetrics->lqireverse;
    }

    if(iproute_rplmetrics->has_dagsize) {
      IPRouteRPLMetricsMsg.dag_size_present_case = IPROUTE_RPLMETRICS__DAG_SIZE_PRESENT_DAG_SIZE;
      IPRouteRPLMetricsMsg.dagsize = iproute_rplmetrics->dagsize;
    }

    rv = csmptlv_write(pbuf, len-used, tlvid, (ProtobufCMessage *)&IPRouteRPLMetricsMsg);
    if (rv == 0) {
      DPRINTF("csmpagent_ipRouteRplMetrics: csmptlv_write error!\n");
      return -1;
    }
    pbuf += rv; used += rv;
  }
  DPRINTF("csmpagent_ipRouteRplMetrics: csmptlv_write [%u] bytes to buffer!\n", used);
  return used;
}
