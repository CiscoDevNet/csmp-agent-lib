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
#include "CsmpTlvs.pb-c.h"

int csmp_get_currenttime(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  struct timeval tv = {0};
  size_t rv = 0;
  uint32_t num;

  DPRINTF("csmpagent_currenttime: start working.\n");
  CurrentTime CurrentTimeMsg = CURRENT_TIME__INIT;

  Current_Time *current_time = NULL;
  current_time = g_csmptlvs_get(tlvid, &num);

  if(current_time) {
    if(current_time->has_posix) {
      CurrentTimeMsg.posix_present_case = CURRENT_TIME__POSIX_PRESENT_POSIX;
      CurrentTimeMsg.posix = current_time->posix;
    }
    if(current_time->has_iso8601) {
      CurrentTimeMsg.iso8601_present_case = CURRENT_TIME__ISO8601_PRESENT_ISO8601;
      CurrentTimeMsg.iso8601 = current_time->iso8601;
    }
    if(current_time->has_source) {
      CurrentTimeMsg.source_present_case = CURRENT_TIME__SOURCE_PRESENT_SOURCE;
      CurrentTimeMsg.source = current_time->source;
    }
  }

  rv = csmptlv_write(buf, len, tlvid, (ProtobufCMessage *)&CurrentTimeMsg);
  if (rv == 0) {
    DPRINTF("csmpagent_currenttime: csmptlv_write error!\n");
    return -1;
  } else {
    DPRINTF("csmpagent_currenttime: csmptlv_write [%ld] bytes to buffer!\n", rv);
    return rv;
  }
}

int csmp_put_currenttime(tlvid_t tlvid, const uint8_t *buf, size_t len, uint8_t *out_buf, size_t out_size, size_t *out_len, int32_t tlvindex)
{
  CurrentTime *CurrentTimeMsg = NULL;
  Current_Time current_time = CURRENT_TIME_INIT;
  tlvid_t tlvid0;
  uint32_t tlvlen;
  const uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;

  DPRINTF("Received POST currenttime TLV\n");

  rv = csmptlv_readTL(pbuf, len, &tlvid0, &tlvlen);
  if ((rv == 0) || (tlvid0.type != CURRENT_TIME_TLVID)) {
    return -1;
  }
  pbuf += rv; used += rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&CurrentTimeMsg, &current_time__descriptor);
  if (rv == 0) {
    return -1;
  }
  pbuf += rv; used += rv;

  if (CurrentTimeMsg->posix_present_case) {
    current_time.has_posix = true;
    current_time.posix = CurrentTimeMsg->posix;
  }
  if (CurrentTimeMsg->iso8601_present_case) {
    current_time.has_iso8601 = true;
    strcpy(current_time.iso8601, CurrentTimeMsg->iso8601);
  }
  if (CurrentTimeMsg->source_present_case) {
    current_time.has_source = true;
    current_time.source = CurrentTimeMsg->source;
  }

  g_csmptlvs_post(tlvid, &current_time);

  DPRINTF("Processed POST %s TLV with size=%d\n", CurrentTimeMsg->base.descriptor->short_name, used);

  csmptlv_free((ProtobufCMessage *)CurrentTimeMsg);
  return used;
}
