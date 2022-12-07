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

#include <stdlib.h>
#include <string.h>
#include "csmp.h"
#include "cgmsagent.h"
#include "csmptlv.h"
#include "csmpagent.h"
#include "CsmpTlvs.pb-c.h"

#define MAX_CNT 15
#define MAX_LEN 8

csmp_subscription_list_t g_csmplib_report_list;

int csmp_get_reportSubscribe(tlvid_t tlvid, uint8_t *buf, size_t len)
{
  ReportSubscribe ReportSubscribeMsg = REPORT_SUBSCRIBE__INIT;
  uint32_t i;
  uint8_t *pbuf = buf;
  size_t rv, used = 0;
  char **tlvlist = NULL;

  tlvlist = malloc(MAX_CNT * sizeof(void *));

  DPRINTF("csmpagent_reportSubscribe: start working.\n");
  ReportSubscribeMsg.interval_present_case = REPORT_SUBSCRIBE__INTERVAL_PRESENT_INTERVAL;
  ReportSubscribeMsg.interval = g_csmplib_report_list.period;

  for (i = 0;i < g_csmplib_report_list.cnt;i++) {
    tlvlist[i] = malloc(MAX_LEN);
    csmptlv_id2str(tlvlist[i],MAX_LEN,&g_csmplib_report_list.list[i]);
  }
  ReportSubscribeMsg.n_tlvid = g_csmplib_report_list.cnt;
  ReportSubscribeMsg.tlvid = tlvlist;

  rv = csmptlv_write(pbuf,len - used,tlvid,(ProtobufCMessage *)&ReportSubscribeMsg);
  if (rv == 0) {
    DPRINTF("csmpagent_reportSubscribe: csmptlv_write error!\n");
    return -1;
  } else {
    DPRINTF("csmpagent_reportSubscribe: csmptlv_write [%ld] bytes to buffer!\n", rv);
  }

  pbuf += rv; used += rv;

  for (i = 0;i < g_csmplib_report_list.cnt;i++)
    free(tlvlist[i]);
  free(tlvlist);

  return used;
}

int csmp_put_reportSubscribe(const uint8_t *buf, size_t len)
{
  ReportSubscribe *ReportSubscribeMsg = NULL;
  tlvid_t tlvid0;
  uint32_t tlvlen;
  uint32_t newcnt = 0;
  const uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;
  long unsigned int i;

  DPRINTF("Received POST reportSubscribe TLV\n");

  rv = csmptlv_readTL(pbuf, len, &tlvid0, &tlvlen);
  if ((rv == 0) || (tlvid0.type != REPORT_SUBSCRIBE_TLVID)) {
    return -1;
  }
  pbuf += rv; used += rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&ReportSubscribeMsg, &report_subscribe__descriptor);
  if (rv == 0) {
    return -1;
  }
  pbuf += rv; used += rv;

  if ((ReportSubscribeMsg->interval_present_case == REPORT_SUBSCRIBE__INTERVAL_PRESENT_INTERVAL) &&
      (g_csmplib_report_list.period != ReportSubscribeMsg->interval)) {
    g_csmplib_report_list.period = ReportSubscribeMsg->interval;
    if (g_csmplib_report_list.period != 0)
      reset_rpttimer();

    DPRINTF("ReportSubscribeMsg: interval=%u\n",g_csmplib_report_list.period);
  }

  DPRINTF("ReportSubscribeMsg: n_tlvids=%u tlvid[] = [ ",
           (uint32_t)ReportSubscribeMsg->n_tlvid);
  for (i = 0;(i < ReportSubscribeMsg->n_tlvid) && (i < MAX_CNT);i++) {
    tlvid_t newid;
    int result;
    result = csmptlv_str2id(ReportSubscribeMsg->tlvid[i],&newid);
    DPRINTF("e%u.%u,",newid.vendor,newid.type);
    if (result == 0)
      continue;

    if ((g_csmplib_report_list.list[i].vendor != newid.vendor) || (g_csmplib_report_list.list[i].type != newid.type)) {
      g_csmplib_report_list.list[i] = newid;
    }
    newcnt++;
  }
  DPRINTF(" ]\n");
  if (g_csmplib_report_list.cnt != newcnt) {
    g_csmplib_report_list.cnt = newcnt;
  }

  DPRINTF("Processed POST %s TLV with size=%d\n", ReportSubscribeMsg->base.descriptor->name, (int)used);

  csmptlv_free((ProtobufCMessage *)ReportSubscribeMsg);
  return used;
}
