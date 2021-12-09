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
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "csmp.h"
#include "csmptlv.h"
#include "coapserver.h"
#include "csmpagent.h"
#include "CsmpTlvs.pb-c.h"

enum {
  URISEG_MAX_SIZE = 128,
  OUTBUF_SIZE = 1048,
  OUTBUF_MAX = 1024, // Provides margin to overflow
  QRY_LIST_MAX = 20,
  DEFAULT_DELAY = 30000, // 30 sec
};

static uint8_t m_RespBuf[OUTBUF_SIZE];

uint32_t strntoul(char *str, char **endptr, uint32_t len, int base);
bool getArgInt(char *key, const coap_uri_seg_t *list,
    uint32_t list_cnt, uint32_t *val);
bool getArgTlvList(char *key, const coap_uri_seg_t *list,
    uint32_t list_cnt, tlvid_t *vals, uint32_t *vals_cnt);
bool getArgString(char *key, const coap_uri_seg_t *list,
    uint32_t list_cnt, char* s, uint32_t *slen);


bool checkExempt(tlvid_t tlvid) {
  const tlvid_t exempt_list[] = {{0,DESCRIPTION_REQUEST_TLVID},{0,IMAGE_BLOCK_TLVID}};
  uint32_t list_cnt = sizeof(exempt_list)/sizeof(tlvid_t);
  int i;

  for (i = 0; i < list_cnt; i++) {
    if ((tlvid.vendor == exempt_list[i].vendor) &&
        (tlvid.type == exempt_list[i].type))
      return true;
  }
  return false;
}

void recv_request(struct sockaddr_in6 *from,
    coap_transaction_type_t tx_type, uint16_t tx_id,
    uint8_t token_length, uint8_t *token,
    coap_method_t method,
    const coap_uri_seg_t *url, uint32_t url_cnt,
    const coap_uri_seg_t *query, uint32_t query_cnt,
    const void *body, uint16_t body_len)
{
  DPRINTF("request recevied!\n");
  tlvid_t tlvid = {0,0};
  int32_t tlvindex = -1L;
  uint32_t delay = 0;
  uint8_t *out_buf = NULL;
  size_t out_len = 0;
  uint16_t coap_status = COAP_CODE_BAD_REQ;
  int rv = 0;
  tlvid_t tlvid_default[2] = {{0, SESSION_ID_TLVID},{0, CURRENT_TIME_TLVID}};
  int i;

#ifdef PRINTDEBUG
  printf("CsmpServer: ");
  switch (method) {
    case COAP_GET: printf("GET "); break;
    case COAP_POST: printf("POST "); break;
    case COAP_DELETE: printf("DELETE "); break;
    default: printf("UNKNOWN "); break;
  }

  switch (tx_type) {
    case COAP_CON: printf("[CON] "); break;
    case COAP_NON: printf("[NON] "); break;
    default: printf("[?] "); break;
  }
  for (i = 0;i < url_cnt;i++) {
    printf("/%.*s",url[i].len,url[i].val);
  }
  for (i = 0;i < query_cnt;i++) {
    printf("%c%.*s",(i) ? '&' : '?',query[i].len,query[i].val);
  }
  printf("\n");
#endif

  if ((url_cnt) && (strncmp((char *)url[0].val,"c",url[0].len) == 0)) {
    if ((url_cnt > 1) && (url[1].len < URISEG_MAX_SIZE-1)) {
      char item[URISEG_MAX_SIZE];
      memcpy(item,url[1].val,url[1].len);
      item[url[1].len] = '\0';
      csmptlv_str2id(item, &tlvid);
      if (url_cnt > 2 && (url[2].len < URISEG_MAX_SIZE-1)) {
        tlvindex = strntoul((char *)url[2].val,NULL,url[2].len,10);
      }
    }
    else {
      tlvid.type = 1;
    }
  }
  else {
    DPRINTF("CsmpServer: Invalid URL!");
    goto done;
  }

  out_buf = m_RespBuf;
  switch (method)
  {
    case COAP_GET:
      {
        uint32_t t1=0, t2=0;
        tlvid_t tlvlist[QRY_LIST_MAX] = {{0,0}};
        uint32_t tlvcnt = QRY_LIST_MAX;
        int i;

        getArgInt("t1=",query,query_cnt,&t1);
        getArgInt("t2=",query,query_cnt,&t2);
        if (getArgTlvList("q=", query, query_cnt, tlvlist, &tlvcnt)) {
          tlvindex = -1;
        }
        else {
          tlvlist[0] = tlvid;
          tlvcnt = 1;
        }

        for (i=0; i<tlvcnt; i++) {
          DPRINTF("CsmpServer: Getting %u.%u\n", tlvlist[i].vendor, tlvlist[i].type);
          rv = csmpagent_get(tlvlist[i], out_buf, OUTBUF_MAX - out_len, tlvindex,t1,t2,from);
          if (rv < 0) {
            if (tlvcnt > 1)  {
              rv = 0;
            }
            else {
              coap_status = COAP_CODE_NOT_FOUND; out_len = 0;
              goto done;
            }
          }
          out_buf += rv; out_len += (size_t) rv;
        }
        g_csmplib_stats.csmp_get_succeed++;
        coap_status = COAP_CODE_CONTENT;
      }
      break;

    case COAP_POST:
      {
        const uint8_t *ibuf = body;
        uint8_t *obuf = out_buf;
        uint32_t tlvlen = 0;
        uint32_t iused = 0, oused = 0;
        size_t rvo = 0;

        int sigStat = checkSignature(ibuf,body_len,from);

        if (sigStat < 0) {
          DPRINTF("CsmpServer: POST Signature Check failed.\n");
          coap_status = COAP_CODE_UNAUTHORIZED; // Unauthorized
          break;
        }
        if (checkGroup(ibuf,body_len,from) == false) {
          DPRINTF("CsmpServer: POST Group Match false.\n");
          break;
        }

        while (iused < body_len) {
          rv = csmptlv_readTL(ibuf, body_len-iused, &tlvid, &tlvlen);
          if (rv == 0) {
            rv = -1;
            break;
          }
          switch (tlvid.type) {

          case SIGNATURE_TLVID:
          case SIGNATURE_VALIDITY_TLVID:
          case GROUP_MATCH_TLVID:
            rv += tlvlen;
          break;

          default:

          if ((sigStat == 0) && (checkExempt(tlvid) == false)) {
            g_csmplib_stats.sig_no_signature++;
            coap_status = COAP_CODE_FORBIDDEN; // Forbidden
            goto done;
          }

          rv = csmpagent_post(tlvid, ibuf, rv + tlvlen, obuf, OUTBUF_MAX - oused, &rvo, tlvindex, from);
          if (rv < 0) {
            coap_status = COAP_CODE_NOT_FOUND; // Not Found
            goto done;
          }
         }
          ibuf += rv; iused += rv;
          obuf += rvo; oused += rvo;
          if (oused >= OUTBUF_MAX) {
            coap_status = COAP_CODE_INTERNAL_SERVER_ERROR;
            rv = -1;
            break;
          }
       }

       if (rv >= 0) {
         if (oused) {
           for(i=0;i<2;i++) {
             rv = csmpagent_get(tlvid_default[i], obuf, OUTBUF_MAX - oused, 0,0,0,from);
             if (rv < 0)
               break;
             obuf += rv; oused += rv;
           }
           out_len = oused;
         }
         g_csmplib_stats.csmp_post_succeed++;
         coap_status = COAP_CODE_CREATED;
       }
     }
     break;

    case COAP_DELETE:
     //not support any delete option
    default:
      coap_status = COAP_CODE_METHOD_NOT_ALLOWED;  // 405 - Method Not Allowed
      break;
  }

done:
    DPRINTF("CsmpServer: Sending Response [out_len=%u], [coap_status=%u]\n",(int)out_len, coap_status);
    coapserver_response(from, COAP_ACK, tx_id, token_length, token, coap_status, m_RespBuf, out_len);

  return;
}

uint32_t strntoul(char *str, char **endptr, uint32_t len, int base)
{
  char item[URISEG_MAX_SIZE];
  memcpy(item,str,len);
  item[len] = '\0';
  return strtoul(item,endptr,base);
}

bool getArgInt(char *key, const coap_uri_seg_t *list, uint32_t list_cnt, uint32_t *val)
{
  char item[URISEG_MAX_SIZE];
  uint32_t keylen = strlen(key);
  uint32_t i;

  for (i = 0;i < list_cnt;i++) {
    if ((strncmp(key,(char *)list[i].val,keylen) == 0) &&
        (list[i].len < sizeof(item))) {
      uint32_t vallen = list[i].len - keylen;
      *val = strntoul((char *)&list[i].val[keylen],NULL,vallen,10);
      return true;
    }
  }
  return false;
}

bool getArgTlvList(char *key, const coap_uri_seg_t *list, uint32_t list_cnt, tlvid_t *vals, uint32_t *vals_cnt)
{
  char item[URISEG_MAX_SIZE];
  uint32_t keylen = strlen(key);
  uint32_t i,j;

  for (i = 0;i < list_cnt;i++) {
    if ((strncmp(key,(char *)list[i].val,keylen) == 0) &&
        (list[i].len < sizeof(item))) {
      uint32_t vallen = list[i].len - keylen;
      char *tmp = item;
      memcpy(item,&list[i].val[keylen],vallen);
      item[vallen] = '\0';
      for (j=0;(j < *vals_cnt) && (*tmp != '\0');j++) {
        int rv;
        rv = csmptlv_str2id(tmp,&vals[j]);
        if (rv == 0)
          break;
        while ((*tmp) && (*tmp != ' ')) tmp++;
        while ((*tmp) && (*tmp == ' ')) tmp++;
      }
      *vals_cnt = j;
      return true;
    }
  }
  return false;
}

bool getArgString(char *key, const coap_uri_seg_t *list, uint32_t list_cnt, char* s, uint32_t *slen)
{
  uint32_t keylen = strlen(key);
  uint32_t i;

  for (i = 0;i < list_cnt;i++) {
    if ((strncmp(key,(char *)list[i].val,keylen) == 0) &&
        (list[i].len < *slen)) {
      uint32_t vallen = list[i].len - keylen;
      memcpy(s,&list[i].val[keylen],vallen);
      s[vallen] = '\0';
      *slen = vallen;
      return true;
    }
  }
  return false;
}

bool csmpserver_disable()
{
  int ret = 0;
  ret = coapserver_stop();
  if(ret < 0)
    return false;
  else
    return true;
}

bool csmpserver_enable()
{
  int ret = 0;
  ret = coapserver_listen(CSMP_DEFAULT_PORT, (recv_handler_t)recv_request);
  if(ret < 0)
    return false;
  else
    return true;
}
