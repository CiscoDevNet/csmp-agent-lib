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
#include <string.h>
#include "osal.h"
#include "csmpinfo.h"
#include "csmpservice.h"
#include "cgmsagent.h"
#include "csmpserver.h"

uint8_t g_csmplib_status = SERVICE_NOT_START;

uint8_t g_csmplib_eui64[8];
uint32_t g_csmplib_reginterval_min = 0;
uint32_t g_csmplib_reginterval_max = 0;

csmp_service_stats_t g_csmplib_stats;

csmptlvs_get_t g_csmptlvs_get;
csmptlvs_post_t g_csmptlvs_post;
signature_verify_t g_csmplib_signature_verify;

csmp_cfg_t g_csmp_signature_settings;

int csmp_service_start(dev_config_t *devconfig, csmp_handle_t *csmp_handle) {
  bool ret;

  if(g_csmplib_status > SERVICE_START_FAILURE)
    return -1;

  g_csmplib_status = SERVICE_START_FAILURE;

  if((devconfig == NULL) || (csmp_handle == NULL))
    return -2;

  memset(g_csmplib_eui64, 0, sizeof(g_csmplib_eui64));
  memcpy(g_csmplib_eui64, devconfig->ieee_eui64.data, sizeof(g_csmplib_eui64));
  g_csmplib_reginterval_min = devconfig->reginterval_min;
  g_csmplib_reginterval_max = devconfig->reginterval_max;

  g_csmp_signature_settings.reqsignedpost = devconfig->csmp_sig_settings.reqsignedpost;
  g_csmp_signature_settings.reqvalidcheckpost = devconfig->csmp_sig_settings.reqvalidcheckpost;
  g_csmp_signature_settings.reqtimesyncpost = devconfig->csmp_sig_settings.reqtimesyncpost;
  g_csmp_signature_settings.reqseclocalpost = devconfig->csmp_sig_settings.reqseclocalpost;
  g_csmp_signature_settings.reqsignedresp = devconfig->csmp_sig_settings.reqsignedresp;
  g_csmp_signature_settings.reqvalidcheckresp = devconfig->csmp_sig_settings.reqvalidcheckresp;
  g_csmp_signature_settings.reqtimesyncresp = devconfig->csmp_sig_settings.reqtimesyncresp;
  g_csmp_signature_settings.reqseclocalresp = devconfig->csmp_sig_settings.reqseclocalresp;

  g_csmptlvs_get = csmp_handle->csmptlvs_get;
  g_csmptlvs_post = csmp_handle->csmptlvs_post;
  g_csmplib_signature_verify = csmp_handle->signature_verify;

  memset(&g_csmplib_stats, 0, sizeof(g_csmplib_stats));

  ret = csmpserver_enable();
  if(!ret)
    return -1;

  ret = register_start(&devconfig->NMSaddr, false);
  if(!ret) {
    csmpserver_disable();
    return -1;
  }

  g_csmplib_status = REGISTRATION_IN_PROGRESS;
  return 0;
}

bool csmp_devconfig_update(dev_config_t *devconfig) {

  if((devconfig == NULL) || (g_csmplib_status < REGISTRATION_IN_PROGRESS))
    return false;

  memset(g_csmplib_eui64, 0, sizeof(g_csmplib_eui64));
  memcpy(g_csmplib_eui64, devconfig->ieee_eui64.data, sizeof(g_csmplib_eui64));
  g_csmplib_reginterval_min = devconfig->reginterval_min;
  g_csmplib_reginterval_max = devconfig->reginterval_max;

  return register_start(&devconfig->NMSaddr, true);
}

bool csmp_service_stop() {
  bool ret = false;

  if(g_csmplib_status < REGISTRATION_IN_PROGRESS)
    return false;

  g_csmplib_status = SERVICE_NOT_START;
  memset(&g_csmplib_stats, 0, sizeof(g_csmplib_stats));

  ret = csmpserver_disable();
  if(!ret)
    return false;

  ret = cgmsagent_stop();
  return ret;
}

csmp_service_status_t csmp_service_status() {
  return g_csmplib_status;
}

csmp_service_stats_t* csmp_service_stats() {
  return &g_csmplib_stats;
}
