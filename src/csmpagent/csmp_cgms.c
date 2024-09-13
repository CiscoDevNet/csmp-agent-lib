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

#include <stdlib.h>
#include <string.h>
#include "csmp.h"
#include "cgmsagent.h"
#include "csmptlv.h"
#include "csmpagent.h"
#include "CsmpTlvs.pb-c.h"

extern uint32_t m_code;
extern uint32_t m_tlvlist[MAX_NOTIFY_TLVID_CNT];

int csmp_get_cgmsNotification(tlvid_t tlvid, uint8_t *buf, size_t len)
{
  CGMSNotification CGMSNotificationMsg = CGMSNOTIFICATION__INIT;
  uint32_t tlvIndex;
  uint8_t *pbuf = buf;
  size_t rv, used = 0;
  uint32_t CGMSNotificationTlvs[MAX_NOTIFY_TLVID_CNT] = {0};

  DPRINTF("csmpagent_cgmsNotification: start working.\n");

  CGMSNotificationMsg.code_present_case = CGMSNOTIFICATION__CODE_PRESENT_CODE;
  CGMSNotificationMsg.code = m_code;

  if (CGMSNotificationMsg.code == CSMP_CGMS_ERR_PROCESS)
  {
    uint8_t cnt = 0;
    for (tlvIndex = 0;tlvIndex < MAX_NOTIFY_TLVID_CNT;tlvIndex++) {
      if (m_tlvlist[tlvIndex] != 0){
        CGMSNotificationTlvs[tlvIndex] = m_tlvlist[tlvIndex];
        cnt++;
      }
      else {
        break;
      }
    }
    CGMSNotificationMsg.n_tlvs = cnt;
    CGMSNotificationMsg.tlvs = CGMSNotificationTlvs;
  }

  rv = csmptlv_write(pbuf, len-used, tlvid, (ProtobufCMessage *)&CGMSNotificationMsg);
  if (rv == 0) {
    DPRINTF("csmpagent_cgmsNotification: csmptlv_write error!\n");
    return CSMP_OP_TLV_WR_ERROR;
  }
  used += rv;

  DPRINTF("csmpagent_cgmsNotification: csmptlv_write [%ld] bytes to buffer!\n", used);
  return used;
}

int csmp_get_cgmsStats(tlvid_t tlvid, uint8_t *buf, size_t len)
{
  CGMSStats CGMStatsMsg = CGMSSTATS__INIT;
  uint8_t *pbuf = buf;
  size_t rv, used = 0;
  csmp_service_stats_t *stats_ptr;

  DPRINTF("csmpagent_cgmsStats: start working.\n");

  stats_ptr = csmp_service_stats();

  CGMStatsMsg.sig_ok_present_case = CGMSSTATS__SIG_OK_PRESENT_SIG_OK;
  CGMStatsMsg.sigok = stats_ptr->sig_ok;

  CGMStatsMsg.sig_bad_auth_present_case = CGMSSTATS__SIG_BAD_AUTH_PRESENT_SIG_BAD_AUTH;
  CGMStatsMsg.sigbadauth = stats_ptr->sig_bad_auth;

  CGMStatsMsg.sig_bad_validity_present_case = CGMSSTATS__SIG_BAD_VALIDITY_PRESENT_SIG_BAD_VALIDITY;
  CGMStatsMsg.sigbadvalidity = stats_ptr->sig_bad_validity;

  CGMStatsMsg.sig_no_sync_present_case = CGMSSTATS__SIG_NO_SYNC_PRESENT_SIG_NO_SYNC;
  CGMStatsMsg.signosync = stats_ptr->sig_no_sync;

  CGMStatsMsg.reg_succeed_present_case = CGMSSTATS__REG_SUCCEED_PRESENT_REG_SUCCEED;
  CGMStatsMsg.regsucceed = stats_ptr->reg_succeed;

  CGMStatsMsg.reg_attempts_present_case = CGMSSTATS__REG_ATTEMPTS_PRESENT_REG_ATTEMPTS;
  CGMStatsMsg.regattempts = stats_ptr->reg_attempts;

  CGMStatsMsg.reg_holds_present_case = CGMSSTATS__REG_HOLDS_PRESENT__NOT_SET;
  //CGMStatsMsg.regholds = stats_ptr->sig_ok;

  CGMStatsMsg.reg_fails_present_case = CGMSSTATS__REG_FAILS_PRESENT_REG_FAILS;
  CGMStatsMsg.regfails = stats_ptr->reg_fails;

  CGMStatsMsg.nms_errors_present_case = CGMSSTATS__NMS_ERRORS_PRESENT_NMS_ERRORS;
  CGMStatsMsg.nmserrors = stats_ptr->nms_errors;

  rv = csmptlv_write(pbuf, len-used, tlvid, (ProtobufCMessage *)&CGMStatsMsg);
  if (rv == 0) {
    DPRINTF("csmpagent_cgmsStats: csmptlv_write error!\n");
    return CSMP_OP_TLV_WR_ERROR;
  }
  pbuf += rv; used += rv;

  DPRINTF("csmpagent_cgmsStats: csmptlv_write [%ld] bytes to buffer!\n", used);
  return used;

}
