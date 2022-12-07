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

int csmp_get_wpanStatus(tlvid_t tlvid, uint8_t *buf, size_t len)
{
  size_t rv = 0;
  uint32_t i, num;
  uint8_t *pbuf = buf;
  uint32_t used = 0;

  DPRINTF("csmpagent_wpanStatus: start working.\n");


  WPAN_Status *wpan_status = NULL;
  wpan_status = g_csmptlvs_get(tlvid, &num);

  if(wpan_status) {
    for(i = 0; i < num; i++) {
      WPANStatus WPANStatusMsg = WPANSTATUS__INIT;

      if(wpan_status[i].has_ifindex) {
        WPANStatusMsg.if_index_present_case = WPANSTATUS__IF_INDEX_PRESENT_IF_INDEX;
        WPANStatusMsg.ifindex = wpan_status[i].ifindex;
      }
      if(wpan_status[i].has_ssid) {
        WPANStatusMsg.ssid_present_case = WPANSTATUS__SSID_PRESENT_SSID;
        WPANStatusMsg.ssid.len = wpan_status[i].ssid.len;
        WPANStatusMsg.ssid.data = wpan_status[i].ssid.data;
      }
      if(wpan_status[i].has_panid) {
        WPANStatusMsg.panid_present_case = WPANSTATUS__PANID_PRESENT_PANID;
        WPANStatusMsg.panid = wpan_status[i].panid;
      }
      if(wpan_status[i].has_master) {
        WPANStatusMsg.master_present_case = WPANSTATUS__MASTER_PRESENT_MASTER;
        WPANStatusMsg.master = wpan_status[i].master;
      }
      if(wpan_status[i].has_dot1xenabled) {
        WPANStatusMsg.dot1x_enabled_present_case = WPANSTATUS__DOT1X_ENABLED_PRESENT_DOT1X_ENABLED;
        WPANStatusMsg.dot1xenabled = wpan_status[i].dot1xenabled;
      }
      if(wpan_status[i].has_securitylevel) {
        WPANStatusMsg.security_level_present_case = WPANSTATUS__SECURITY_LEVEL_PRESENT_SECURITY_LEVEL;
        WPANStatusMsg.securitylevel = wpan_status[i].securitylevel;
      }
      if(wpan_status[i].has_rank) {
        WPANStatusMsg.rank_present_case = WPANSTATUS__RANK_PRESENT_RANK;
        WPANStatusMsg.rank = wpan_status[i].rank;
      }
      if(wpan_status[i].has_beaconvalid) {
        WPANStatusMsg.beacon_valid_present_case = WPANSTATUS__BEACON_VALID_PRESENT_BEACON_VALID;
        WPANStatusMsg.beaconvalid = wpan_status[i].beaconvalid;
      }
      if(wpan_status[i].has_beaconversion) {
        WPANStatusMsg.beacon_version_present_case = WPANSTATUS__BEACON_VERSION_PRESENT_BEACON_VERSION;
        WPANStatusMsg.beaconversion = wpan_status[i].beaconversion;
      }
      if(wpan_status[i].has_beaconage) {
        WPANStatusMsg.beacon_age_present_case = WPANSTATUS__BEACON_AGE_PRESENT_BEACON_AGE;
        WPANStatusMsg.beaconage = wpan_status[i].beaconage;
      }
      if(wpan_status[i].has_txpower) {
        WPANStatusMsg.tx_power_present_case = WPANSTATUS__TX_POWER_PRESENT_TX_POWER;
        WPANStatusMsg.txpower = wpan_status[i].txpower;
      }
      if(wpan_status[i].has_dagsize) {
        WPANStatusMsg.dag_size_present_case = WPANSTATUS__DAG_SIZE_PRESENT_DAG_SIZE;
        WPANStatusMsg.dagsize = wpan_status[i].dagsize;
      }
      if(wpan_status[i].has_metric) {
        WPANStatusMsg.metric_present_case = WPANSTATUS__METRIC_PRESENT_METRIC;
        WPANStatusMsg.metric = wpan_status[i].metric;
      }
      if(wpan_status[i].has_lastchanged) {
        WPANStatusMsg.last_changed_present_case = WPANSTATUS__LAST_CHANGED_PRESENT_LAST_CHANGED;
        WPANStatusMsg.lastchanged = wpan_status[i].lastchanged;
      }
      if(wpan_status[i].has_lastchangedreason) {
        WPANStatusMsg.last_changed_reason_present_case = WPANSTATUS__LAST_CHANGED_REASON_PRESENT_LAST_CHANGED_REASON;
        WPANStatusMsg.lastchangedreason = wpan_status[i].lastchangedreason;
      }
      if(wpan_status[i].has_demomodeenabled) {
        WPANStatusMsg.demo_mode_enabled_present_case = WPANSTATUS__DEMO_MODE_ENABLED_PRESENT_DEMO_MODE_ENABLED;
        WPANStatusMsg.demomodeenabled = wpan_status[i].demomodeenabled;
      }

      rv = csmptlv_write(pbuf, len-used, tlvid, (ProtobufCMessage *)&WPANStatusMsg);
      if (rv == 0) {
        DPRINTF("csmpagent_wpanStatus: csmptlv_write error!\n");
        return -1;
      }
      pbuf += rv; used += rv;
    }
  }
  DPRINTF("csmpagent_wpanStatus: csmptlv_write [%u] bytes to buffer!\n", used);
  return used;
}
