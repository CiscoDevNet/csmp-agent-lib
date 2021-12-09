/*
 * Cisco CSMP Reference Implementation Source Code License Agreement v1.0
 *
 * These terms govern your use of Cisco Systems, Inc.’s CSMP Reference
 * Implementation Source Code and associated documentation (“Licensed
 * Materials”). By accessing, downloading, copying, distributing, calling, or
 * otherwise making any use of the Licensed Materials, you accept and agree to
 * be bound by the following terms and conditions (the “License”). If you are
 * accepting the License on behalf of an entity, you represent that you have the
 * authority to do so (either you or the entity, “you”). All use of the Licensed
 * Materials must be in accordance with this License and all rights not
 * expressly granted herein are reserved.

 * 1.	LICENSE GRANT.
 * Subject to Your compliance with the terms of this License, Cisco grants you
 * a limited, non-exclusive, perpetual, revocable, non-sublicensable,
 * royalty-free, worldwide, and non-transferable license to: (a) use the
 * Licensed Materials for the purpose of internal development, testing,
 * maintenance and support of your products to interoperate with Cisco 802.15.4
 * border routers -- including without limitation the  Cisco 1000 Series
 * Connected Grid Routers – and Cisco IoT Field Network Director and any
 * successor products (collectively the “Cisco Products”), and for no other
 * purpose; (b) use and reproduce a reasonable number of copies of the Licensed
 * Materials solely for the internal development, testing, back-up, maintenance
 * and support of your products as permitted under this License; (c) make
 * corrections and bug fixes to the Licensed Materials only if such corrections
 * and bug fixes do not interfere with or inhibit the interoperability of your
 * products with the Cisco Products; and (d) distribute the CSMP Reference
 * Implementation Source Code solely in compiled, binary format with your
 * products to your customers solely to enable your products to interoperate
 * with the Cisco Products.

 * 2.	CONDITIONS.
 * You shall not use the Licensed Materials to: (a) substantially replicate and
 * compete with the Cisco Products; (b) collect, store, or transmit confidential
 * , sensitive, or protected data or information without authorization; (c)
 * disable, disrupt, circumvent, interfere with, or otherwise violate the
 * security of any Cisco product, service, or user;  (d) access any application,
 * system, service, computer, data, account, or network without authorization;
 * (e) develop features or functions in stand-alone applications that do not
 * interoperate with the Cisco Products;  or (f) sell or offer to sell the
 * Licensed Products on a standalone basis. The Cisco Products are licensed
 * under their own separate terms and you shall not use the Licensed Materials
 * in any way that violates or is inconsistent with those terms (for more
 * information, please visit: www.cisco.com/go/terms). Failure to comply with
 * any part of this section or section 2 constitutes a material breach of this
 * License, terminates the license granted in Section 1 above, and may result in
 * further repercussions and actions against you, at Cisco’s sole discretion.

 * 3.	OWNERSHIP.
 * The Licensed Materials, in whole or in part, in all forms, and any
 * corrections or bug fixes thereto that you make are the sole and exclusive
 * property of Cisco.   You irrevocably transfer, convey, and assign to Cisco
 * all of your right, title, and interest in any corrections or bug fixes you
 * make to the Licensed Materials.  You shall send the corrections or bug fixes
 * to ask-crmesh-dev@cisco.com.  This License does not grant you permission to
 * use any trade names, trademarks, service marks, or product names of Cisco.
 * If you provide any feedback to Cisco regarding the Licensed Materials, you
 * agree that Cisco, its suppliers, and its customers shall be free to use and
 * incorporate such feedback into the Licensed Materials and Cisco products and
 * services without restriction, payment, or additional consideration of any
 * kind. If you initiate or participate in any litigation against Cisco, its
 * partners, or its customers (including cross-claims and counter-claims)
 * alleging that the Licensed Materials or its use infringe any patent,
 * copyright, or other intellectual property right, then all rights granted to
 * you under this License shall terminate immediately.

 * 4.	CONFIDENTIAL INFORMATION.
 * The Licensed Materials, whether in whole or in part, contain confidential
 * information and proprietary property of Cisco.  Licensee acknowledges that
 * the Licensed Materials contain trade secrets and other confidential
 * information which are the property of Cisco.  You shall keep the Licensed
 * Materials in strict confidence and take all reasonable steps to prevent: (a)
 * unauthorized persons from having access to the Licensed Materials; (b)
 * disclosure of the Licensed Materials to any third party; and (c) unauthorized
 * copying of the Licensed Materials.

 * 5.	PROPRIETARY NOTICES.
 * You agree to maintain and reproduce all copyright, trademarks and other
 * proprietary notices on all copies of the Licensed Materials, in any form, in
 * the same form and manner that such copyright, trademarks, and other
 * proprietary notices are included on the Licensed Materials.

 * 6.	NO SUPPORT.
 * Nothing in this License shall require Cisco to provide any support, services,
 * updates, upgrades, bug fixes or any other modifications to the Licensed
 * Materials.  You shall be solely responsible for providing support to your
 * customers for any problems or issues that they encounter by using the CSMP
 * Reference Implementation Source Code in binary form that you distributed.
 * Cisco may, at its own sole discretion, update, extend, enhance, or otherwise
 * modify the Licensed Materials without notice.  If Cisco provides updates to
 * you and no separate agreement accompanies such updates, then such updates are
 * subject to this License.

 * 7.	LIMITATION OF LIABILITY.
 * CISCO SHALL HAVE NO LIABILITY IN CONNECTION WITH OR RELATING TO THIS LICENSE
 * OR USE OF THE LICENSED MATERIALS, FOR DAMAGES OF ANY KIND, INCLUDING BUT NOT
 * LIMITED TO DIRECT, INCIDENTAL, AND CONSEQUENTIAL DAMAGES, OR FOR ANY LOSS OF
 * USE, DATA, INFORMATION, PROFITS, BUSINESS, OR GOODWILL, HOWEVER CAUSED, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

 * 8.	DISCLAIMER OF WARRANTY.
 * CISCO PROVIDES THE LICENSED MATERIALS “AS IS” WITH ALL FAULTS, WITHOUT
 * WARRANTY OF ANY KIND. TO THE MAXIMUM EXTENT PERMITTED BY LAW, ALL EXPRESS AND
 * IMPLIED CONDITIONS, REPRESENTATIONS, AND WARRANTIES INCLUDING, WITHOUT
 * LIMITATION, ANY IMPLIED WARRANTY OR CONDITION OF MERCHANTABILITY, FITNESS FOR
 * A PARTICULAR PURPOSE, NON-INFRINGEMENT, SATISFACTORY QUALITY,
 * NON-INTERFERENCE, AND ACCURACY, ARE HEREBY EXCLUDED AND EXPRESSLY DISCLAIMED
 * BY CISCO. CISCO DOES NOT WARRANT THAT THE LICENSED MATERIALS ARE SUITABLE FOR
 * YOUR USE, WILL OPERATE PROPERLY WITH YOUR APPLICATION, ARE ACCURATE OR
 * COMPLETE, OR ARE WITHOUT ERROR OR DEFECT.

 * 9.	GENERAL.
 * This License shall be governed by and interpreted in accordance with the laws
 * of the State of California, excluding its conflict of law’s provisions. You
 * agree to comply with all applicable United States export laws, rules, and
 * regulations. If any provision of this License is judged illegal, invalid, or
 * otherwise unenforceable, that provision shall be severed, and the rest of the
 * License shall remain in full force and effect. No failure by Cisco to enforce
 * any of its rights related to the Licensed Materials or to a breach of this
 * License in a particular situation will act as a waiver of such rights. In the
 * event of any inconsistencies with any other terms that may apply to the
 * Licensed Materials, this License shall take precedence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "coap.h"
#include "coapclient.h"
#include "csmp.h"
#include "csmptlv.h"
#include "csmpagent.h"
#include "cgmsagent.h"
#include "CsmpTlvs.pb-c.h"
#include "trickle_timer.h"

#define OUTBUF_SIZE 1048
static struct sockaddr_in6 NMS_addr;
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

uint32_t g_csmplib_notificationCode = 0;

int doSendtlvs(tlvid_t *list, uint32_t list_cnt, coap_transaction_type_t txn_type,
                           char name, int32_t tlvindex, bool prepend,
                           uint8_t token_length, uint8_t *token) {
  uint8_t *pbuf = g_outbuf;
  coap_uri_seg_t url;
  int i, rvi, used = 0;
  tlvid_t list_pre[2] = {{0,SESSION_ID_TLVID},{0,CURRENT_TIME_TLVID}};

  url.len = 1;
  url.val = (uint8_t *)&name;

  if (prepend) {
    for(i = 0; i < 2; i++)  {
      rvi = csmpagent_get(list_pre[i], pbuf, OUTBUF_SIZE-used, -1,0,0,NULL);
      if (rvi < 0) {
        DPRINTF("CgmsAgent: Unable to write TLV %u.%u\n",list_pre[i].vendor,list_pre[i].type);
        return -1;
      }
      pbuf += rvi; used += rvi;
    }
  }

  for (i = 0; i < list_cnt; i++) {
    rvi = csmpagent_get(list[i], pbuf, OUTBUF_SIZE-used, tlvindex,0,0,NULL);
    if (rvi < 0) {
      DPRINTF("CgmsAgent: Unable to write TLV %u.%u\n",list[i].vendor,list[i].type);
      return -1;
    }
    pbuf += rvi; used += rvi;
  }

  if (used) {
    rvi =  coapclient_request(&NMS_addr, txn_type, COAP_POST, token_length, token,
                              &url,1,NULL,0,g_outbuf,used);
    if (rvi<0)
      DPRINTF("CsmpAgent: CoapClient.request failed! list[1] = e%u.%u\n",list[1].vendor,list[1].type);
  }
  return rvi;
}

void report_timer_fired() {

  g_csmplib_stats.metrics_reports++;
  doSendtlvs(g_csmplib_report_list.list, g_csmplib_report_list.cnt,COAP_NON,'c',-1,true,0,NULL);
}

void reset_rpttimer() {
  trickle_timer_stop(rpt_timer);
  trickle_timer_start(rpt_timer, g_csmplib_report_list.period, g_csmplib_report_list.period,
                        (trickle_timer_fired_t)report_timer_fired);
}

void process_reg(const uint8_t *buf,size_t len, bool preload_only, struct sockaddr_in6 *srcaddr) {
  const uint8_t *pbuf = buf;
  tlvid_t tlvid;
  uint32_t tlvlen;
  uint32_t used = 0;
  int rv = 0;

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
        rv = csmpagent_post(tlvid,pbuf,tlvlen,NULL,0,NULL,0,srcaddr);
        if (rv < 0)
          return;
        break;
    }
    pbuf += tlvlen; used += tlvlen;
  }

  if (!preload_only) {
   if(used == 0)
      return;
   trickle_timer_stop(reg_timer);
   g_csmplib_status = REGISTRATION_SUCCESS;

   if(g_csmplib_report_list.period != 0)
     report_timer_fired();

     trickle_timer_start(rpt_timer, g_csmplib_report_list.period, g_csmplib_report_list.period,
                        (trickle_timer_fired_t)report_timer_fired);
  }
  return;
}

void register_timer_fired() {
  int ret = 0;
  int i = 0;
  tlvid_t list[] = {{0,DEVICE_ID_TLVID},{0,CURRENT_TIME_TLVID},
                    {0,HARDWARE_DESC_TLVID},{0,INTERFACE_DESC_TLVID},{0,IPADDRESS_TLVID},
                    {0,IPROUTE_TLVID},{0,INTERFACE_METRICS_TLVID},{0,IPROUTE_RPLMETRICS_TLVID},
                    {0,WPANSTATUS_TLVID}, {0,RPLINSTANCE_TLVID}, {0,FIRMWARE_IMAGE_INFO_TLVID}};
  uint32_t list_cnt = sizeof(list)/sizeof(tlvid_t);

  g_csmplib_stats.reg_attempts++;
  doSendtlvs(list,list_cnt,COAP_CON,'r',-1,false,0,NULL);
}


void response_handler(struct sockaddr_in6 *from,
           uint16_t status, const void *body, uint16_t body_len)
{
  int sigStat = 0;

  DPRINTF("CgmsAgent: CoapClient.response with status=%d body_len=%d\n",status,body_len);

  if ((status/100) != 2) {
    if (g_csmplib_status == REGISTRATION_SUCCESS) {
      // Something went wrong at the NMS. Re-register

      trickle_timer_start(reg_timer, g_csmplib_reginterval_min, g_csmplib_reginterval_max,
                         (trickle_timer_fired_t)register_timer_fired);
      g_csmplib_status = REGISTRATION_IN_PROGRESS;
    }
    else {
      g_csmplib_stats.reg_fails++;
      g_csmplib_stats.reg_fails_stats.error_coap++;
    }

    DPRINTF("CgmsAgent: Response status Check failed.\n");
    return;
  }

  if (body_len > 0) {
    sigStat = checkSignature(body,body_len,from);
    if(sigStat <= 0) {
      if(sigStat == 0)
        g_csmplib_stats.sig_no_signature++;

      DPRINTF("CgmsAgent: Response Signature Check failed.\n");
      g_csmplib_stats.reg_fails++;
      g_csmplib_stats.reg_fails_stats.error_signature++;
      return;
    }
  }

  switch (g_csmplib_status) {
  case REGISTRATION_IN_PROGRESS:
    process_reg(body,body_len,false,from);
    if (g_csmplib_status == REGISTRATION_SUCCESS)  {
      g_csmplib_stats.reg_succeed++;
      DPRINTF("CgmsAgent: Registration Complete!\n");
    }
    else {
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
  trickle_timer_stop(reg_timer);
  trickle_timer_stop(rpt_timer);
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
  trickle_timer_start(reg_timer, g_csmplib_reginterval_min, g_csmplib_reginterval_max,
      (trickle_timer_fired_t)register_timer_fired);
  return true;
}
