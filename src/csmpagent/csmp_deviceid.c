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
#include "csmpinfo.h"
#include "csmpagent.h"
#include "csmpfunction.h"
#include "csmptlv.h"
#include "CsmpTlvs.pb-c.h"

extern uint8_t g_csmplib_eui64[8];

int csmp_get_deviceid(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  (void)tlvindex; // Suppress unused param compiler warning.
  size_t rv = 0;
  char id[128];
  DeviceID DeviceIDMsg = DEVICE_ID__INIT;

  memset(id, 0, sizeof(id));
  snprintf(id,sizeof(id),"%02X%02X%02X%02X%02X%02X%02X%02X",
                 g_csmplib_eui64[0],g_csmplib_eui64[1],g_csmplib_eui64[2],g_csmplib_eui64[3],
                 g_csmplib_eui64[4],g_csmplib_eui64[5],g_csmplib_eui64[6],g_csmplib_eui64[7]);

  DPRINTF("csmpagent_deviceid: start working.\n");
  DeviceIDMsg.type_present_case = DEVICE_ID__TYPE_PRESENT_TYPE;
  DeviceIDMsg.type = 1;

  DeviceIDMsg.id_present_case = DEVICE_ID__ID_PRESENT_ID;
  DeviceIDMsg.id = id;

  rv = csmptlv_write(buf, len, tlvid, (ProtobufCMessage *)&DeviceIDMsg);
  if (rv == 0) {
    DPRINTF("csmpagent_deviceid: csmptlv_write error!\n");
    return -1;
  } else {
    DPRINTF("csmpagent_deviceid: csmptlv_write [%ld] bytes to buffer!\n", rv);
    return rv;
  }
}
