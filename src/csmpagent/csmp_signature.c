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
#include <sys/time.h>
#include "csmp.h"
#include "csmptlv.h"
#include "csmpagent.h"
#include "CsmpTlvs.pb-c.h"

static Signature g_SigMsg = SIGNATURE__INIT;
static SignatureValidity g_SigValidityMsg = SIGNATURE_VALIDITY__INIT;
static uint8_t signature_data[88] = {0};


int checkSignature(const uint8_t *buf, uint32_t len) {

#if 0
  const uint8_t *ptlv;
  uint32_t seclen;
  int rv;
  struct timeval tv = {0};
  uint8_t *sig = NULL;
  uint8_t *sigend = NULL;
  size_t siglen;

  ptlv = csmptlv_find(buf,len,tlvid,&msglen);

  if (!ptlv) {
    DPRINTF("CsmpServer: Cannot locate required Signature TLV\n");
    return 0;
  }

  rv = csmpagent_post(tlvid, ptlv, msglen,NULL,0,NULL,0);
  if (rv == 0) {
    DPRINTF("CsmpServer: Problem parsing Signature TLV\n");
    return -1;
  }

  seclen = (uint32_t)(ptlv - buf);
  tlvid.type = SIGNATURE_VALIDITY_TLVID;

//  if (m_pCsmpCfg->reqvalidcheckpost) {
    ptlv = csmptlv_find(buf,len,tlvid,&msglen);
    if (!ptlv) {
      DPRINTF("CsmpServer: Cannot locate required SignatureValidity TLV\n");
      return -1;
    }
    rv = csmpagent_post(tlvid, ptlv, msglen,NULL,0,NULL,0);
    if (rv == 0) {
      DPRINTF("CsmpServer: Problem parsing SignatureValidity TLV\n");
      return -1;
    }
//  }

  //extract Ecdsa-Sig-Value
  if(g_SigMsg.has_value) {
    sig = g_SigMsg.value.data;
    sigend = g_SigMsg.value.data + g_SigMsg.value.len;
    if (*sig++ != 0x30) {//(ASN1_UNIVERSAL | ASN1_CONSTRUCTED | ASN1_SEQUENCE)
      g_csmplib_stats.sig_bad_auth++;
      return -1;
    }
    sig++; //total len

    if (*sig++ != 0x06) {//(ASN1_UNIVERSAL | ASN1_PRIMITIVE | ASN1_OBJECT_IDENTIFIER)
      g_csmplib_stats.sig_bad_auth++;
      return -1;
    }
    size_t id_len = *sig++;
    sig += id_len; //object identifier

    if (*sig++ != 0x03) {//(ASN1_UNIVERSAL | ASN1_PRIMITIVE | ASN1_BIT_STRING)
      g_csmplib_stats.sig_bad_auth++;
      return -1;
    }
    sig++; sig++; //len, num of unused bits
  }
  siglen = sigend - sig;

  gettimeofday(&tv, NULL);
  if (g_SigValidityMsg.has_notbefore && (g_SigValidityMsg.notbefore <= tv.tv_sec) &&
      g_SigValidityMsg.has_notafter && (g_SigValidityMsg.notafter >= tv.tv_sec)) {
    if (g_SigMsg.has_value &&
        g_csmplib_signature_verify((uint8_t *)buf,seclen,sig,siglen)) {
      g_csmplib_stats.sig_ok++;
      return 1;
    }
    else {
      g_csmplib_stats.sig_bad_auth++;
      return -1;
    }
  }
  else {
    g_csmplib_stats.sig_bad_validity++;
    DPRINTF("CsmpServer: invalid time \n");
    return -1;
  }
#else
  // Temporary for first release.
  tlvid_t tlvid = {0,SIGNATURE_TLVID};
  uint32_t msglen;
  csmptlv_find(buf,len,tlvid,&msglen);
  return 1;
#endif
}

int csmp_put_signature(const uint8_t *buf, size_t len)
{
  Signature *SignatureMsg = NULL;
  tlvid_t tlvid0;
  uint32_t tlvlen;
  const uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;

  DPRINTF("Received POST Signature TLV\n");

  rv = csmptlv_readTL(pbuf, len, &tlvid0, &tlvlen);
  if ((rv == 0) || (tlvid0.type != SIGNATURE_TLVID)) {
    return -1;
  }
  pbuf += rv; used += rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&SignatureMsg, &signature__descriptor);
  if (rv == 0) {
    return -1;
  }
  pbuf += rv; used += rv;

  if (SignatureMsg->value_present_case) {
    memcpy(signature_data, SignatureMsg->value.data, SignatureMsg->value.len);
    g_SigMsg.value_present_case  = SignatureMsg->value_present_case;
    g_SigMsg.value.len = SignatureMsg->value.len;
    g_SigMsg.value.data = signature_data;
    DPRINTF("Processed POST Signature TLV\n");
  }
  csmptlv_free((ProtobufCMessage *)SignatureMsg);
  return used;
}

int csmp_put_signatureValidity(const uint8_t *buf, size_t len)
{
  SignatureValidity *SignatureValidityMsg = NULL;
  tlvid_t tlvid0;
  uint32_t tlvlen;
  const uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;

  DPRINTF("Received POST SignatureValidity TLV\n");

  rv = csmptlv_readTL(pbuf, len, &tlvid0, &tlvlen);
  if ((rv == 0) || (tlvid0.type != SIGNATURE_VALIDITY_TLVID)) {
    return -1;
  }
  pbuf += rv; used += rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&SignatureValidityMsg, &signature_validity__descriptor);
  if (rv == 0) {
    return -1;
  }
  pbuf += rv; used += rv;

  if (SignatureValidityMsg->not_before_present_case) {
    g_SigValidityMsg.not_before_present_case = SignatureValidityMsg->not_before_present_case;
    g_SigValidityMsg.notbefore = SignatureValidityMsg->notbefore;
  }
  if (SignatureValidityMsg->not_after_present_case) {
    g_SigValidityMsg.not_after_present_case = SignatureValidityMsg->not_after_present_case;
    g_SigValidityMsg.notafter = SignatureValidityMsg->notafter;
  }

  DPRINTF("Processed POST SignatureValidity TLV\n");

  csmptlv_free((ProtobufCMessage *)SignatureValidityMsg);
  return used;
}
