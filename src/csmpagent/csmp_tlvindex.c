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
#include "csmpagent.h"
#include "csmptlv.h"
#include "CsmpTlvs.pb-c.h"

#define NUM_TLVS 17
static char *ptlvs[NUM_TLVS] = {
  TLV_INDEX_ID_STRING,
  DEVICE_ID_ID_STRING,
  SESSION_ID_ID_STRING,
  GROUP_ASSIGN_ID_STRING,
  GROUP_INFO_ID_STRING,
  REPORT_SUBSCRIBE_ID_STRING,

  HARDWARE_DESC_ID_STRING,
  INTERFACE_DESC_ID_STRING,
  IPADDRESS_ID_STRING,
  IPROUTE_ID_STRING,
  CURRENT_TIME_ID_STRING,
  UPTIME_ID_STRING,
  INTERFACE_METRICS_ID_STRING,
  IPROUTE_RPLMETRICS_ID_STRING,
  WPANSTATUS_ID_STRING,
  RPLINSTANCE_ID_STRING,
  FIRMWARE_IMAGE_INFO_ID_STRING
};

int csmp_get_tlvindex(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  int i = 0;
  int j = 0;
  size_t rv = 0;

  DPRINTF("csmpagent_tlvindex: start working.\n");
  TlvIndex TlvIndexMsg = TLV_INDEX__INIT;
  TlvIndexMsg.n_tlvid = NUM_TLVS;
  TlvIndexMsg.tlvid = (char **)ptlvs;

  rv = csmptlv_write(buf, len, tlvid, (ProtobufCMessage *)&TlvIndexMsg);
  if (rv == 0) {
    DPRINTF("csmpagent_tlvindex: csmptlv_write error!\n");
    return -1;
  } else {
    DPRINTF("csmpagent_tlvindex: csmptlv_write [%ld] bytes to buffer!\n", rv);
    return rv;
  }
}
