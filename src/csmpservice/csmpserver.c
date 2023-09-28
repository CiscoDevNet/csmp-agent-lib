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

extern void csmpNotify(bool is_reg, uint32_t code, uint32_t *errlist, uint8_t token_length, uint8_t *token);

bool checkExempt(tlvid_t tlvid) {
  const tlvid_t exempt_list[] = {{0,DESCRIPTION_REQUEST_TLVID},{0,IMAGE_BLOCK_TLVID}};
  uint32_t list_cnt = sizeof(exempt_list)/sizeof(tlvid_t);
  uint32_t i;

  for (i = 0; i < list_cnt; i++) {
    if ((tlvid.vendor == exempt_list[i].vendor) &&
        (tlvid.type == exempt_list[i].type))
      return true;
  }
  return false;
}

void recv_request(struct sockaddr_in6 *from,
	coap_transaction_type_t tx_type,
    uint16_t tx_id,
    uint8_t token_length,
	uint8_t *token,
    coap_method_t method,
    const coap_uri_seg_t *url,
	uint32_t url_cnt,
    const coap_uri_seg_t *query,
	uint32_t query_cnt,
    const void *body,
	uint16_t body_len)
{
  DPRINTF("request received!\n");
  tlvid_t tlvid = {0,0};
  int32_t tlvindex = -1L;
  uint8_t *out_buf = NULL;
  size_t out_len = 0;
  uint16_t coap_status = COAP_CODE_BAD_REQ;
  int rv = 0;
  tlvid_t tlvid_default[2] = {{0, SESSION_ID_TLVID},{0, CURRENT_TIME_TLVID}};
  uint32_t i;
  uint8_t err_tlvid_count = 0;
  uint32_t err_tlvid[MAX_NOTIFY_TLVID_CNT] = {0};

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
#else
  (void)tx_type;	// Null expression to avoid unused-parameter warning
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
        uint32_t i;
        bool vendorFlag = false;

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
          /*In case of vendor TLV i.e. 127, we're considering continuous three arguments
          1st : vendor TLV (127)
          2nd : vendor subtype
          3rd : IANAEN i.e. vendorID
          Once, we're encountering the vendor TLV (127), it's type is being overwritten by the subtype (2nd argument)
          E.g. GetTLV 127 8 4
          tlvid.type = 127
          if (tlvid.type == 127)
          {
            tlvid.vendor = 4 //(tlvid+2).type
            tlvid.type = 8   //(tlvid+1).type
          }
          Hence, for vendor TLV, the type would be equal to vendor subtype (instead of 127)*/
          if (tlvlist[i].type == VENDOR_TLVID)
          {
            tlvlist[i].vendor = tlvlist[i+2].type;
            tlvlist[i].type = tlvlist[i+1].type;
            vendorFlag = true;
          }
          rv = csmpagent_get(tlvlist[i], out_buf, OUTBUF_MAX - out_len, tlvindex);
          if (vendorFlag)
          {
            i = i+2;
            vendorFlag = false;
          }
          if (rv < 0) {
            if (tlvcnt > 1)  {
              rv = 0;
            }
            else {
              if (rv == CSMP_OP_UNSUPPORTED)
                coap_status = COAP_CODE_NOT_FOUND;
              else if (rv == CSMP_OP_TLV_RD_ERROR)
                coap_status = COAP_CODE_BAD_REQ;
              else if (rv == CSMP_OP_FAILED)
                coap_status = COAP_CODE_INTERNAL_SERVER_ERROR;
              else
                coap_status = COAP_CODE_FORBIDDEN;

              out_len = 0;
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

        int sigStat = checkSignature(ibuf,body_len, false);

        if (sigStat < 0) {
          DPRINTF("CsmpServer: POST Signature Check failed.\n");
          coap_status = COAP_CODE_UNAUTHORIZED; // Unauthorized
          break;
        }
        if (checkGroup(ibuf,body_len) == false) {
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

          rv = csmpagent_post(tlvid, ibuf, rv + tlvlen, obuf, OUTBUF_MAX - oused, &rvo, tlvindex);
          if (rv < 0) {
            if (tx_type == COAP_NON){
              err_tlvid[err_tlvid_count] = tlvid.type;
              err_tlvid_count++;
              if (err_tlvid_count == MAX_NOTIFY_TLVID_CNT) {
                goto done;
              }
            }
            else {
              coap_status = COAP_CODE_NOT_FOUND; // Not Found
              goto done;
            }
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

       if (err_tlvid_count > 0) {
            break;
        }

       if (rv >= 0) {
         if (oused) {
           for(i=0;i<2;i++) {
             rv = csmpagent_get(tlvid_default[i], obuf, OUTBUF_MAX - oused, 0);
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
    if (tx_type == COAP_CON) {
      DPRINTF("CsmpServer: Sending Response [out_len=%u], [coap_status=%u]\n",(int)out_len, coap_status);
      coapserver_response(from, COAP_ACK, tx_id, token_length, token, coap_status, m_RespBuf, out_len);
    }
    else if (method == COAP_POST) {
      if (err_tlvid_count > 0) {
        csmpNotify(false, CSMP_CGMS_ERR_PROCESS, err_tlvid, token_length, token);
      }
      else {
        csmpNotify(false, CSMP_CGMS_SUC_PROCESS, NULL, token_length, token);
      }
    }

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
