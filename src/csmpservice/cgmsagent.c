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

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "coap.h"
#include "coapclient.h"
#include "csmp.h"
#include "csmptlv.h"
#include "csmpagent.h"
#include "cgmsagent.h"
#include "CsmpTlvs.pb-c.h"
#include "osal_common.h"

#define OUTBUF_SIZE 1048
static osal_sockaddr_t NMS_addr;
static uint8_t g_outbuf[OUTBUF_SIZE];

enum {
  REASON_COLDSTART = 1,
  REASON_ADMIN = 2,
  REASON_IP_CHANGE = 3,
  REASON_NMS_CHANGE = 4,
  REASON_NMS_REDIRECT = 5,
  REASON_NMS_ERROR = 6,
  REASON_8021X_AUTH_CHANGE = 7,
  REASON_OUTAGE_RECOVERY = 8
};

extern uint8_t g_csmplib_status;
extern uint32_t g_csmplib_reginterval_min;
extern uint32_t g_csmplib_reginterval_max;
extern csmp_subscription_list_t g_csmplib_report_list;

uint32_t m_code;
uint32_t m_tlvlist[MAX_NOTIFY_TLVID_CNT];
uint8_t m_token_length;
uint8_t m_token[COAP_MAX_TKL];
uint32_t m_errorlist[MAX_NOTIFY_TLVID_CNT];

int doSendtlvs(tlvid_t *list, uint32_t list_cnt, coap_transaction_type_t txn_type,
                           char name, int32_t tlvindex, bool prepend,
                           uint8_t token_length, uint8_t *token);

void doNotification(bool is_noPost) {
  tlvid_t list[] = {{0,DEVICE_ID_TLVID},{0,CGMSNOTIFICATION_TLVID}};

  if (is_noPost)
  {
    doSendtlvs(list,2,COAP_NON,'c',-1,false,0,NULL);
  }
  else
  {
    doSendtlvs(list,2,COAP_NON,'c',-1,false,m_token_length,m_token);
  }
}

void csmpNotify(bool is_reg, uint32_t code, uint32_t *errlist, uint8_t token_length, uint8_t *token) {
  m_code = code;
  m_token_length = token_length;

  if (m_token_length != 0) {
    memcpy(m_token, token, m_token_length);
  }

  if (code == CSMP_CGMS_ERR_PROCESS) {
    memcpy(m_tlvlist, errlist, sizeof(m_tlvlist));
  }

  if (!is_reg) {
    doNotification(false);
  }
}

void sendNotification(uint32_t code, uint32_t *errlist) {
  csmpNotify(true, code, errlist, 0, NULL);
  DPRINTF("CgmsAgent: Response error %d.\n", code);
  doNotification(true);
}

int doSendtlvs(tlvid_t *list, uint32_t list_cnt, coap_transaction_type_t txn_type,
                           char name, int32_t tlvindex, bool prepend,
                           uint8_t token_length, uint8_t *token) {
  uint8_t *pbuf = g_outbuf;
  coap_uri_seg_t url;
  int rvi, used = 0;
  uint32_t i;
  tlvid_t list_pre[2] = {{0,SESSION_ID_TLVID},{0,CURRENT_TIME_TLVID}};

  url.len = 1;
  url.val = (uint8_t *)&name;

  if (prepend) {
    for(i = 0; i < 2; i++)  {
      rvi = csmpagent_get(list_pre[i], pbuf, OUTBUF_SIZE-used, -1);
      if (rvi < 0) {
        DPRINTF("CgmsAgent: Unable to write TLV %u.%u\n",list_pre[i].vendor,list_pre[i].type);
        return -1;
      }
      pbuf += rvi; used += rvi;
    }
  }

  for (i = 0; i < list_cnt; i++) {
    rvi = csmpagent_get(list[i], pbuf, OUTBUF_SIZE-used, tlvindex);
    if (rvi < 0) {
      DPRINTF("CgmsAgent: Unable to write TLV %u.%u\n",list[i].vendor,list[i].type);
      if (rvi == CSMP_OP_TLV_WR_ERROR) {
        DPRINTF("CgmsAgent: Current Buffer is insufficient. TLV %u.%u\n",list[i].vendor,list[i].type);
        break;
      } else if (rvi == CSMP_OP_UNSUPPORTED) {
        DPRINTF("CgmsAgent: Unsupported TLV %u.%u\n",list[i].vendor,list[i].type);
        rvi = 0;  // ignore unsupported tlvs
      } else {
        DPRINTF("CgmsAgent: Unable to get TLV %u.%u\n",list[i].vendor,list[i].type);
        rvi = 0;
      }
      return -1;
    }
    pbuf += rvi; used += rvi;
  }

  if (used) {
    rvi =  coapclient_request(&NMS_addr, txn_type, COAP_POST, token_length, token,
                              &url,1,NULL,0,g_outbuf,used);
    if (rvi<0) {
      DPRINTF("CsmpAgent: CoapClient.request failed! list[1] = e%u.%u\n",list[1].vendor,list[1].type);
    }
  }
  return rvi;
}

void report_timer_fired() {

  g_csmplib_stats.metrics_reports++;
  doSendtlvs(g_csmplib_report_list.list, g_csmplib_report_list.cnt,COAP_NON,'c',-1,true,0,NULL);
}

void reset_rpttimer() {
  osal_trickle_timer_stop(rpt_timer);
  osal_trickle_timer_start(rpt_timer, g_csmplib_report_list.period, g_csmplib_report_list.period,
                          (trickle_timer_fired_t)report_timer_fired);
}

void process_reg(const uint8_t *buf,size_t len, bool preload_only) {
  const uint8_t *pbuf = buf;
  tlvid_t tlvid;
  uint32_t tlvlen;
  uint32_t used = 0;
  int rv = 0;
  uint8_t errIndex = 0;

  while (used < len) {
    rv = csmptlv_readTL(pbuf,len - used,&tlvid,&tlvlen);
    if (rv == 0)
      return;
    tlvlen += rv;

    switch (tlvid.type) {
      case SIGNATURE_TLVID:
      case SIGNATURE_VALIDITY_TLVID:
        break;
      default:
        rv = csmpagent_post(tlvid,pbuf,tlvlen,NULL,0,NULL,0);
        if (rv < 0){
          if (!preload_only){
            m_errorlist[errIndex] = tlvid.type;
            errIndex++;
            if (errIndex == MAX_NOTIFY_TLVID_CNT)
              return;
          }
          return;
        }
        break;
    }
    pbuf += tlvlen; used += tlvlen;
  }

  if (!preload_only) {
   if(used == 0)
      return;
   osal_trickle_timer_stop(reg_timer);
   g_csmplib_status = REGISTRATION_SUCCESS;

   if(g_csmplib_report_list.period != 0)
     report_timer_fired();

   osal_trickle_timer_start(rpt_timer, g_csmplib_report_list.period, g_csmplib_report_list.period,
                           (trickle_timer_fired_t)report_timer_fired);
  }
  return;
}

void register_timer_fired() {
  tlvid_t list[] = {{0,DEVICE_ID_TLVID},{0,CURRENT_TIME_TLVID},
                    {0,HARDWARE_DESC_TLVID},{0,INTERFACE_DESC_TLVID},{0,IPADDRESS_TLVID},
                    {0,IPROUTE_TLVID},{0,INTERFACE_METRICS_TLVID},{0,IPROUTE_RPLMETRICS_TLVID},
                    {0,WPANSTATUS_TLVID}, {0,RPLINSTANCE_TLVID}, {0,FIRMWARE_IMAGE_INFO_TLVID}};
  uint32_t list_cnt = sizeof(list)/sizeof(tlvid_t);

  g_csmplib_stats.reg_attempts++;
  doSendtlvs(list,list_cnt,COAP_CON,'r',-1,false,0,NULL);
}


void response_handler(struct sockaddr_in6 *from, uint16_t status, const void *body, uint16_t body_len)
{
  int sigStat = 0;

  (void) from;	// To avoid the unused-parameter warning.
  DPRINTF("CgmsAgent: CoapClient.response with status=%d body_len=%d\n",status,body_len);

  if ((status/100) != 2) {
    if (g_csmplib_status == REGISTRATION_SUCCESS) {
      // Something went wrong at the NMS. Re-register

      osal_trickle_timer_start(reg_timer, g_csmplib_reginterval_min, g_csmplib_reginterval_max,
                         (trickle_timer_fired_t)register_timer_fired);
      g_csmplib_status = REGISTRATION_IN_PROGRESS;
      g_csmplib_stats.nms_errors++;
    }
    else {
      g_csmplib_stats.reg_fails++;
      g_csmplib_stats.reg_fails_stats.error_coap++;
    }

    DPRINTF("CgmsAgent: Response status Check failed.\n");
    sendNotification(CSMP_CGMS_ERR_COAP, NULL);
    return;
  }

  if (body_len > 0) {
    sigStat = checkSignature(body,body_len, true);
    if(sigStat <= 0) {
      if(sigStat == 0)
        g_csmplib_stats.sig_no_signature++;

      DPRINTF("CgmsAgent: Response Signature Check failed.\n");
      g_csmplib_stats.reg_fails++;
      g_csmplib_stats.reg_fails_stats.error_signature++;
      sendNotification(CSMP_CGMS_ERR_SIGNATURE, NULL);
      return;
    }
  }

  switch (g_csmplib_status) {
  case REGISTRATION_IN_PROGRESS:
    process_reg(body,body_len,false);
    if (g_csmplib_status == REGISTRATION_SUCCESS)  {
      g_csmplib_stats.reg_succeed++;
      DPRINTF("CgmsAgent: Registration Complete!\n");
    }
    else {
      sendNotification(CSMP_CGMS_ERR_SIGNATURE, m_errorlist);
      g_csmplib_stats.reg_fails++;
      g_csmplib_stats.reg_fails_stats.error_process++;
    }
    break;
  }
  return;
}

bool cgmsagent_stop()
{
  int ret = 0;
  osal_trickle_timer_stop(reg_timer);
  osal_trickle_timer_stop(rpt_timer);
  ret = coapclient_stop();
  if(ret < 0)
    return false;
  else
    return true;
}

bool register_start(struct in6_addr *NMSaddr, bool update)
{
  int ret = 0;

  if(!update) {
    ret = coapclient_open(response_handler);
    if (ret < 0) {
      DPRINTF("coapclient_open failed.\n");
      return false;
    }
  }

  memset(&NMS_addr, 0, sizeof(NMS_addr));
  NMS_addr.sin6_family = AF_INET6;
  NMS_addr.sin6_port = htons(CSMP_DEFAULT_PORT);
  memcpy(NMS_addr.sin6_addr.s6_addr, NMSaddr, sizeof(struct in6_addr));

  g_csmplib_status = REGISTRATION_IN_PROGRESS;
  osal_trickle_timer_start(reg_timer, g_csmplib_reginterval_min, g_csmplib_reginterval_max,
      (trickle_timer_fired_t)register_timer_fired);
  return true;
}
