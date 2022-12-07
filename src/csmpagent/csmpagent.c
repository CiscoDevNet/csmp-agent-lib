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

#include "csmp.h"
#include "csmpagent.h"
#include "csmpfunction.h"

int csmpagent_get(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  switch (tlvid.type) {
    case TLV_INDEX_TLVID:
      return csmp_get_tlvindex(tlvid, buf, len, tlvindex);
      break;
    case DEVICE_ID_TLVID:
      return csmp_get_deviceid(tlvid, buf, len, tlvindex);
      break;
    case SESSION_ID_TLVID:
      return csmp_get_sessionID(tlvid, buf, len, tlvindex);
      break;
    case GROUP_ASSIGN_TLVID:
      return csmp_get_groupAssign(tlvid, buf, len, tlvindex);
      break;
    case GROUP_INFO_TLVID:
      return csmp_get_groupInfo(tlvid, buf, len, tlvindex);
      break;
    case REPORT_SUBSCRIBE_TLVID:
      return csmp_get_reportSubscribe(tlvid, buf, len, tlvindex);
      break;
    case HARDWARE_DESC_TLVID:
      return csmp_get_hardwareDesc(tlvid, buf, len, tlvindex);
      break;
    case INTERFACE_DESC_TLVID:
      return csmp_get_interfaceDesc(tlvid, buf, len, tlvindex);
      break;
    case IPADDRESS_TLVID:
      return csmp_get_ipAddress(tlvid, buf, len, tlvindex);
      break;
    case IPROUTE_TLVID:
      return csmp_get_ipRoute(tlvid, buf, len, tlvindex);
      break;
    case CURRENT_TIME_TLVID:
      return csmp_get_currenttime(tlvid, buf, len, tlvindex);
      break;
    case UPTIME_TLVID:
      return csmp_get_uptime(tlvid, buf, len, tlvindex);
      break;
    case INTERFACE_METRICS_TLVID:
      return csmp_get_interfaceMetrics(tlvid, buf, len, tlvindex);
      break;
    case IPROUTE_RPLMETRICS_TLVID:
      return csmp_get_ipRouteRplMetrics(tlvid, buf, len, tlvindex);
      break;
    case WPANSTATUS_TLVID:
      return csmp_get_wpanStatus(tlvid, buf, len, tlvindex);
      break;
    case RPLINSTANCE_TLVID:
      return csmp_get_rplInstance(tlvid, buf, len, tlvindex);
      break;
    case FIRMWARE_IMAGE_INFO_TLVID:
      return csmp_get_firmwareImageInfo(tlvid, buf, len, tlvindex);
      break;
    default:
      DPRINTF("csmpagent_get: doesn't support get option of tlv:%u.%u\n",tlvid.vendor,tlvid.type);
      return 0;
  }
}

int csmpagent_post(tlvid_t tlvid, const uint8_t *buf, size_t len, uint8_t *out_buf, size_t out_size, size_t *out_len, int32_t tlvindex)
{
  switch (tlvid.type) {
    case CURRENT_TIME_TLVID:
      return csmp_put_currenttime(tlvid, buf, len, out_buf, out_size, out_len, tlvindex);
      break;
    case SESSION_ID_TLVID:
      return csmp_put_sessionID(tlvid, buf, len, out_buf, out_size, out_len, tlvindex);
      break;
    case SIGNATURE_TLVID:
      return csmp_put_signature(tlvid, buf, len, out_buf, out_size, out_len, tlvindex);
      break;
    case SIGNATURE_VALIDITY_TLVID:
      return csmp_put_signatureValidity(tlvid, buf, len, out_buf, out_size, out_len, tlvindex);
      break;
    case GROUP_ASSIGN_TLVID:
      return csmp_put_groupAssign(tlvid, buf, len, out_buf, out_size, out_len, tlvindex);
      break;
    case GROUP_MATCH_TLVID:
      return csmp_put_groupMatch(tlvid, buf, len, out_buf, out_size, out_len, tlvindex);
      break;
    case REPORT_SUBSCRIBE_TLVID:
      return csmp_put_reportSubscribe(tlvid, buf, len, out_buf, out_size, out_len, tlvindex);
      break;
    default:
      DPRINTF("csmpagent_get: doesn't support post option of tlv:%u.%u\n",tlvid.vendor,tlvid.type);
      return 0;
  }
}
