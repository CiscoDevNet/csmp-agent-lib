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
#include "csmpinfo.h"
#include "csmpservice.h"
#include "csmptlv.h"
#include "CsmpTlvs.pb-c.h"

int csmp_get_uptime(tlvid_t tlvid, uint8_t *buf, size_t len)
{
  // struct timeval tv = {0};
  size_t rv = 0;
  uint32_t num;

  DPRINTF("csmpagent_uptime: start working.\n");
  Uptime UptimeMsg = UPTIME__INIT;

  Up_Time *up_time = NULL;
  up_time = g_csmptlvs_get(tlvid, &num);

  if(up_time) {
    if(up_time->has_sysuptime) {
      UptimeMsg.sys_up_time_present_case = UPTIME__SYS_UP_TIME_PRESENT_SYS_UP_TIME;
      UptimeMsg.sysuptime = up_time->sysuptime;
    }
  }

  rv = csmptlv_write(buf, len, tlvid,(ProtobufCMessage *)&UptimeMsg);
  if (rv == 0) {
    DPRINTF("csmpagent_uptime: csmptlv_write error!\n");
    return -1;
  } else {
    DPRINTF("csmpagent_uptime: csmptlv_write [%ld] bytes to buffer!\n", rv);
    return rv;
  }
}
