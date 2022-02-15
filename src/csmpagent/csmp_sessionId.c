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
#include "csmptlv.h"
#include "csmpagent.h"
#include "CsmpTlvs.pb-c.h"

static SessionID gSessionIDVal = SESSION_ID__INIT;
static char sessionID[17] = {0};

int csmp_get_sessionID(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  size_t rv = 0;

  DPRINTF("csmpagent_sessionID: start working.\n");

  if(gSessionIDVal.id == NULL)
    return 0;

  rv = csmptlv_write(buf, len, tlvid, (ProtobufCMessage *)&gSessionIDVal);
  if (rv == 0) {
    DPRINTF("csmpagent_sessionID: csmptlv_write error!\n");
    return -1;
  } else {
    DPRINTF("csmpagent_sessionID: csmptlv_write [%ld] bytes to buffer!\n", rv);
    return rv;
  }
}

int csmp_put_sessionID(tlvid_t tlvid, const uint8_t *buf, size_t len, uint8_t *out_buf, size_t out_size, size_t *out_len, int32_t tlvindex)
{
  SessionID *SessionIDMsg = NULL;
  tlvid_t tlvid0;
  uint32_t tlvlen;
  const uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;

  DPRINTF("Received POST sessionID TLV\n");

  rv = csmptlv_readTL(pbuf, len, &tlvid0, &tlvlen);
  if ((rv == 0) || (tlvid0.type != SESSION_ID_TLVID)) {
    return -1;
  }
  pbuf += rv; used += rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&SessionIDMsg, &session_id__descriptor);
  if (rv == 0) {
    return -1;
  }
  pbuf += rv; used += rv;

  if (SessionIDMsg->id_present_case && SessionIDMsg->id) {
    snprintf(sessionID,strlen(SessionIDMsg->id)+1,"%s",SessionIDMsg->id);
    gSessionIDVal.id = sessionID;
    gSessionIDVal.id_present_case = SessionIDMsg->id_present_case;
    DPRINTF("Processed POST sessionID TLV with id value: %s\n", sessionID);
  }
  csmptlv_free((ProtobufCMessage *)SessionIDMsg);

  return used;
}
