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
#include "osal.h"

extern csmp_cfg_t g_csmp_signature_settings;

static Signature g_SigMsg = SIGNATURE__INIT;
static SignatureValidity g_SigValidityMsg = SIGNATURE_VALIDITY__INIT;
static uint8_t signature_data[88] = {0};


int checkSignature(const uint8_t *buf, uint32_t len, bool agent)
{
  uint32_t msglen;
  const uint8_t *ptlv;
  uint32_t seclen;
  int rv, ret;
  struct timeval tv = {0};
  uint8_t *sig = NULL;
  uint8_t *sigend = NULL;
  size_t siglen;
  tlvid_t tlvid = {0,SIGNATURE_TLVID};

  if ((agent && !g_csmp_signature_settings.reqsignedresp) || (!agent && !g_csmp_signature_settings.reqsignedpost))
    return SUCCESS;

  ptlv = csmptlv_find(buf,len,tlvid,&msglen);

  if (!ptlv) {
    if (agent)
    {
      DPRINTF("CgmsAgent: Cannot locate required Signature TLV\n");
    }
    else
    {
      DPRINTF("CsmpServer: Cannot locate required Signature TLV\n");
    }
    return FAILURE;
  }

  rv = csmpagent_post(tlvid, ptlv, msglen,NULL,0,NULL,0);
  if (rv == 0) {
    if (agent)
    {
      DPRINTF("CgmsAgent: Problem parsing Signature TLV\n");
    }
    else
    {
      DPRINTF("CsmpServer: Problem parsing Signature TLV\n");
    }
    return ERROR;
  }

  seclen = (uint32_t)(ptlv - buf);
  tlvid.type = SIGNATURE_VALIDITY_TLVID;


  if ((agent && g_csmp_signature_settings.reqvalidcheckresp) || (!agent && g_csmp_signature_settings.reqvalidcheckpost)) {
    ptlv = csmptlv_find(buf,len,tlvid,&msglen);
    if (!ptlv) {
      if (agent)
      {
        DPRINTF("CgmsAgent: Cannot locate required SignatureValidity TLV\n");
      }
      else
      {
        DPRINTF("CsmpServer: Cannot locate required SignatureValidity TLV\n");
      }
      return ERROR;
    }
    rv = csmpagent_post(tlvid, ptlv, msglen,NULL,0,NULL,0);
    if (rv == 0) {
      if (agent)
      {
        DPRINTF("CgmsAgent: Problem parsing SignatureValidity TLV\n");
      }
      else
      {
        DPRINTF("CsmpServer: Problem parsing SignatureValidity TLV\n");
      }
      return ERROR;
    }
  }

  //extract Ecdsa-Sig-Value
  if(g_SigMsg.value_present_case == SIGNATURE__VALUE_PRESENT_VALUE) {
    sig = g_SigMsg.value.data;
    sigend = g_SigMsg.value.data + g_SigMsg.value.len;
    if (*sig++ != 0x30) {//(ASN1_UNIVERSAL | ASN1_CONSTRUCTED | ASN1_SEQUENCE)
      g_csmplib_stats.sig_bad_auth++;
      return ERROR;
    }
    sig++; //total len

    if (*sig++ != 0x06) {//(ASN1_UNIVERSAL | ASN1_PRIMITIVE | ASN1_OBJECT_IDENTIFIER)
      g_csmplib_stats.sig_bad_auth++;
      return ERROR;
    }
    size_t id_len = *sig++;
    sig += id_len; //object identifier

    if (*sig++ != 0x03) {//(ASN1_UNIVERSAL | ASN1_PRIMITIVE | ASN1_BIT_STRING)
      g_csmplib_stats.sig_bad_auth++;
      return ERROR;
    }
    sig++; sig++; //len, num of unused bits
  siglen = sigend - sig;
  }

  ret = osal_gettime(&tv, NULL);

  if (ret == OSAL_FAILURE)
  {
    g_csmplib_stats.sig_no_sync++;
    return ERROR;
  }

  if (g_SigValidityMsg.not_before_present_case && (g_SigValidityMsg.notbefore <= tv.tv_sec) &&
      g_SigValidityMsg.not_after_present_case && (g_SigValidityMsg.notafter >= tv.tv_sec)) {
    if (g_SigMsg.value_present_case &&
        g_csmplib_signature_verify((uint8_t *)buf,seclen,sig,siglen)) {
      g_csmplib_stats.sig_ok++;
      return SUCCESS;
    }
    else {
      g_csmplib_stats.sig_bad_auth++;
      return ERROR;
    }
  }
  else {
    g_csmplib_stats.sig_bad_validity++;
    //DPRINTF("CsmpServer: invalid time \n");
    if (agent)
    {
      DPRINTF("CgmsAgent: invalid time \n");
    }
    else
    {
      DPRINTF("CsmpServer: invalid time \n");
    }
    return ERROR;
  }
/*
  // Temporary for first release.
  tlvid_t tlvid = {0,SIGNATURE_TLVID};
  uint32_t msglen;
  csmptlv_find(buf,len,tlvid,&msglen);
  return 1;
  */
}

int csmp_put_signature(tlvid_t tlvid, const uint8_t *buf, size_t len, uint8_t *out_buf, size_t out_size, size_t *out_len, int32_t tlvindex)
{
  Signature *SignatureMsg = NULL;
  tlvid_t tlvid0;
  uint32_t tlvlen;
  const uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;

  (void) tlvid; // Suppress unused param compiler warning.
  (void) out_buf; // Suppress unused param compiler warning.
  (void) out_size; // Suppress unused param compiler warning.
  (void) out_len; // Suppress unused param compiler warning.
  (void) tlvindex; // Suppress unused param compiler warning.
  DPRINTF("Received POST Signature TLV\n");

  rv = csmptlv_readTL(pbuf, len, &tlvid0, &tlvlen);
  if ((rv == 0) || (tlvid0.type != SIGNATURE_TLVID)) {
    return CSMP_OP_TLV_RD_ERROR;
  }
  pbuf += rv; used += rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&SignatureMsg, &signature__descriptor);
  if (rv == 0) {
    return CSMP_OP_TLV_RD_ERROR;
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

int csmp_get_signature(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  Signature SignatureMsg = SIGNATURE__INIT;
  size_t rv;
  int used = 0;

  (void)tlvindex; // Suppress unused param warning.
  DPRINTF("csmpagent_signature: start working.\n");

  if (g_SigMsg.value_present_case) {
    SignatureMsg.value_present_case = g_SigMsg.value_present_case;
    SignatureMsg.value.len = g_SigMsg.value.len;
    SignatureMsg.value.data = g_SigMsg.value.data;
  }
  else {
    return CSMP_OP_TLV_RD_EMPTY;
  }

  rv = csmptlv_write(buf,len - used,tlvid,(ProtobufCMessage *)&SignatureMsg);
  if (rv == 0) {
    return CSMP_OP_TLV_WR_ERROR;
  }
  used += rv;

  DPRINTF("csmpagent_signature: csmptlv_write [%u] bytes to buffer!\n", used);

  return used;
}

int csmp_put_signatureValidity(tlvid_t tlvid, const uint8_t *buf, size_t len, uint8_t *out_buf, size_t out_size, size_t *out_len, int32_t tlvindex)
{
  SignatureValidity *SignatureValidityMsg = NULL;
  tlvid_t tlvid0;
  uint32_t tlvlen;
  const uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;

  (void) tlvid; // Suppress unused param compiler warning.
  (void) out_buf; // Suppress unused param compiler warning.
  (void) out_size; // Suppress unused param compiler warning.
  (void) out_len; // Suppress unused param compiler warning.
  (void) tlvindex; // Suppress unused param compiler warning.
  DPRINTF("Received POST SignatureValidity TLV\n");

  rv = csmptlv_readTL(pbuf, len, &tlvid0, &tlvlen);
  if ((rv == 0) || (tlvid0.type != SIGNATURE_VALIDITY_TLVID)) {
    return CSMP_OP_TLV_RD_ERROR;
  }
  pbuf += rv; used += rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&SignatureValidityMsg, &signature_validity__descriptor);
  if (rv == 0) {
    return CSMP_OP_TLV_RD_ERROR;
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

int csmp_get_signatureValidity(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  SignatureValidity SignatureValidityMsg = SIGNATURE_VALIDITY__INIT;
  size_t rv;
  int used = 0;

  (void)tlvindex; // Suppress unused param warning.
  DPRINTF("csmpagent_signatureValidity: start working.\n");

  if (g_SigValidityMsg.not_before_present_case) {
    SignatureValidityMsg.not_before_present_case = g_SigValidityMsg.not_before_present_case;
    SignatureValidityMsg.notbefore = g_SigValidityMsg.notbefore;
  }
  if (g_SigValidityMsg.not_after_present_case) {
    SignatureValidityMsg.not_after_present_case = g_SigValidityMsg.not_after_present_case;
    SignatureValidityMsg.notafter = g_SigValidityMsg.notafter;
  }

  if(!SignatureValidityMsg.not_before_present_case && !SignatureValidityMsg.not_after_present_case)
    return CSMP_OP_TLV_RD_EMPTY;

  rv = csmptlv_write(buf,len - used,tlvid,(ProtobufCMessage *)&SignatureValidityMsg);
  if (rv == 0) {
    return CSMP_OP_TLV_WR_ERROR;
  }
  used += rv;

  DPRINTF("csmpagent_signatureValidity: csmptlv_write [%u] bytes to buffer!\n", used);

  return used;
}

int csmp_put_signatureSettings(tlvid_t tlvid, const uint8_t *buf, size_t len, uint8_t *out_buf, size_t out_size, size_t *out_len, int32_t tlvindex)
{
  SignatureSettings *SignatureSettingsMsg = NULL;
  Signature_Settings signature_settings = SIGNATURE_SETTINGS_INIT;
  tlvid_t tlvid0;
  uint32_t tlvlen;
  const uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;

  (void) out_buf; // Suppress unused param compiler warning.
  (void) out_size; // Suppress unused param compiler warning.
  (void) out_len; // Suppress unused param compiler warning.
  (void) tlvindex; // Suppress unused param compiler warning.

  DPRINTF("Received POST signatureSettings TLV\n");

  rv = csmptlv_readTL(pbuf, len, &tlvid0, &tlvlen);
  if ((rv == 0) || (tlvid0.type != SIGNATURE_SETTINGS_TLVID)) {
    return CSMP_OP_TLV_RD_ERROR;
  }
  pbuf += rv; used += rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&SignatureSettingsMsg, &signature_settings__descriptor);
  if (rv == 0) {
    return CSMP_OP_TLV_RD_ERROR;
  }
  pbuf += rv; used += rv;

  if ((SignatureSettingsMsg->cert_present_case) && (SignatureSettingsMsg->cert.len > sizeof(signature_settings.cert.data)))
    return CSMP_OP_FAILED;

  if ((SignatureSettingsMsg->req_signed_post_present_case) &&
      (!signature_settings.has_reqsignedpost || (signature_settings.reqsignedpost != SignatureSettingsMsg->reqsignedpost)))
  {
    signature_settings.has_reqsignedpost = true;
    signature_settings.reqsignedpost = SignatureSettingsMsg->reqsignedpost;
  }
  if ((SignatureSettingsMsg->req_valid_check_post_present_case) &&
      (!signature_settings.has_reqvalidcheckpost || (signature_settings.reqvalidcheckpost != SignatureSettingsMsg->reqvalidcheckpost)))
  {
    signature_settings.has_reqvalidcheckpost = true;
    signature_settings.reqvalidcheckpost = SignatureSettingsMsg->reqvalidcheckpost;
  }
  if ((SignatureSettingsMsg->req_time_sync_post_present_case) &&
      (!signature_settings.has_reqtimesyncpost || (signature_settings.reqtimesyncpost != SignatureSettingsMsg->reqtimesyncpost)))
  {
    signature_settings.has_reqtimesyncpost = true;
    signature_settings.reqtimesyncpost = SignatureSettingsMsg->reqtimesyncpost;
  }
  if ((SignatureSettingsMsg->req_sec_local_post_present_case) &&
      (!signature_settings.has_reqseclocalpost || (signature_settings.reqseclocalpost != SignatureSettingsMsg->reqseclocalpost)))
  {
    signature_settings.has_reqseclocalpost = true;
    signature_settings.reqseclocalpost = SignatureSettingsMsg->reqseclocalpost;
  }
  if ((SignatureSettingsMsg->req_signed_resp_present_case) &&
      (!signature_settings.has_reqsignedresp || (signature_settings.reqsignedresp != SignatureSettingsMsg->reqsignedresp)))
  {
    signature_settings.has_reqsignedresp = true;
    signature_settings.reqsignedresp = SignatureSettingsMsg->reqsignedresp;
  }
  if ((SignatureSettingsMsg->req_valid_check_resp_present_case) &&
      (!signature_settings.has_reqvalidcheckresp || (signature_settings.reqvalidcheckresp != SignatureSettingsMsg->reqvalidcheckresp)))
  {
    signature_settings.has_reqvalidcheckresp = true;
    signature_settings.reqvalidcheckresp = SignatureSettingsMsg->reqvalidcheckresp;
  }
  if ((SignatureSettingsMsg->req_time_sync_resp_present_case) &&
      (!signature_settings.has_reqtimesyncresp || (signature_settings.reqtimesyncresp != SignatureSettingsMsg->reqtimesyncresp)))
  {
    signature_settings.has_reqtimesyncresp = true;
    signature_settings.reqtimesyncresp = SignatureSettingsMsg->reqtimesyncresp;
  }
  if ((SignatureSettingsMsg->req_sec_local_resp_present_case) &&
      (!signature_settings.has_reqseclocalresp || (signature_settings.reqseclocalresp != SignatureSettingsMsg->reqseclocalresp)))
  {
    signature_settings.has_reqseclocalresp = true;
    signature_settings.reqseclocalresp = SignatureSettingsMsg->reqseclocalresp;
  }

  if (SignatureSettingsMsg->cert_present_case)
  {
    if ((signature_settings.has_cert == false) ||
        (signature_settings.cert.len != SignatureSettingsMsg->cert.len) ||
        (memcmp(signature_settings.cert.data,SignatureSettingsMsg->cert.data,SignatureSettingsMsg->cert.len) != 0))
    {
      signature_settings.has_cert = true;
      signature_settings.cert.len = SignatureSettingsMsg->cert.len;
      memcpy(signature_settings.cert.data,SignatureSettingsMsg->cert.data,SignatureSettingsMsg->cert.len);
    }
  }

  g_csmptlvs_post(tlvid, &signature_settings);

  DPRINTF("csmpagent_signatureSettings: csmptlv_write [%u] bytes to buffer!\n", used);

  csmptlv_free((ProtobufCMessage *)SignatureSettingsMsg);
  return used;
}

int csmp_get_signatureSettings(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  size_t rv = 0, used = 0;
  uint32_t num;

  (void)tlvindex; // Suppress unused param warning.
  DPRINTF("signaturesettings: start working.\n");
  SignatureSettings SignatureSettingsMsg = SIGNATURE_SETTINGS__INIT;

  Signature_Settings *signature_settings = NULL;
  signature_settings = g_csmptlvs_get(tlvid, &num);

  SignatureSettingsMsg.req_signed_post_present_case = SIGNATURE_SETTINGS__REQ_SIGNED_POST_PRESENT_REQ_SIGNED_POST;
  SignatureSettingsMsg.req_valid_check_post_present_case = SIGNATURE_SETTINGS__REQ_VALID_CHECK_POST_PRESENT_REQ_VALID_CHECK_POST;
  SignatureSettingsMsg.req_time_sync_post_present_case = SIGNATURE_SETTINGS__REQ_TIME_SYNC_POST_PRESENT_REQ_TIME_SYNC_POST;
  SignatureSettingsMsg.req_sec_local_post_present_case = SIGNATURE_SETTINGS__REQ_SEC_LOCAL_POST_PRESENT_REQ_SEC_LOCAL_POST;
  SignatureSettingsMsg.req_signed_resp_present_case = SIGNATURE_SETTINGS__REQ_SIGNED_RESP_PRESENT_REQ_SIGNED_RESP;
  SignatureSettingsMsg.req_valid_check_resp_present_case = SIGNATURE_SETTINGS__REQ_VALID_CHECK_RESP_PRESENT_REQ_VALID_CHECK_RESP;
  SignatureSettingsMsg.req_time_sync_resp_present_case = SIGNATURE_SETTINGS__REQ_TIME_SYNC_RESP_PRESENT_REQ_TIME_SYNC_RESP;
  SignatureSettingsMsg.req_sec_local_resp_present_case = SIGNATURE_SETTINGS__REQ_SEC_LOCAL_RESP_PRESENT_REQ_SEC_LOCAL_RESP;

  SignatureSettingsMsg.reqsignedpost = signature_settings->reqsignedpost;
  SignatureSettingsMsg.reqvalidcheckpost = signature_settings->reqvalidcheckpost;
  SignatureSettingsMsg.reqtimesyncpost = signature_settings->reqtimesyncpost;
  SignatureSettingsMsg.reqseclocalpost = signature_settings->reqseclocalpost;
  SignatureSettingsMsg.reqsignedresp = signature_settings->reqsignedresp;
  SignatureSettingsMsg.reqvalidcheckresp = signature_settings->reqvalidcheckresp;
  SignatureSettingsMsg.reqtimesyncresp = signature_settings->reqtimesyncresp;
  SignatureSettingsMsg.reqseclocalresp = signature_settings->reqseclocalresp;

  rv = csmptlv_write(buf, len, tlvid, (ProtobufCMessage *)&SignatureSettingsMsg);
  if (rv == 0) {
    DPRINTF("csmpagent_signatureSettings: csmptlv_write error!\n");
    return CSMP_OP_TLV_WR_ERROR;
  }
  used += rv;

  DPRINTF("csmpagent_signatureSettings: csmptlv_write [%ld] bytes to buffer!\n", used);
  return used;
}
