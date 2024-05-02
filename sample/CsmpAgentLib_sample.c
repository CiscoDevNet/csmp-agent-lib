/*
 *  Copyright 2021-2024 Cisco Systems, Inc.
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

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include "CsmpAgentLib_sample.h"
#include "CsmpAgentLib_sample_util.h"
#include "CsmpAgentLib_sample_tlvs.h"
#include "csmp_service.h"
#include "csmp_info.h"
#include "signature_verify.h"
#include "osal.h"
#include "CsmpAgentLib_sample_config.h"
#endif
#if defined(OSAL_EFR32_WISUN)
#include "sl_system_init.h"
#include "sl_wisun_app_core_util.h"
#include "sl_wisun_ntp_timesync.h"
#endif

typedef struct thread_argument {
  int argc;
  char **argv;
} thread_argument_t;

// Linux Thread function
static void *csmp_sample_app_thr_fnc(void *arg)
{
  thread_argument_t *thread_arg = (thread_argument_t *)arg;
  int argc = thread_arg->argc;
  char **argv = thread_arg->argv;

  struct timeval tv = {0};
  csmp_service_status_t status;
  csmp_service_stats_t *stats_ptr;
  char *status_msg[] = {"CSMP service is not started\n",
                       "Failed to start CSMP service\n",
                       "Registration is in progress...\n",
                       "Regist to the NMS successfully\n"};
  int ret, i;
  char *endptr;
  bool sigFlag = false;

  osal_gettime(&tv, NULL);
  g_init_time = tv.tv_sec;

  /**************************************************************
    init the dev_config parameter of csmp_service_start func:
      * NMS address
      * EUI64
      * register interval(min, max)
  ***************************************************************/
  memset(&g_devconfig, 0, sizeof(dev_config_t));
  osal_inet_pton(AF_INET6, NMS_IP, &g_devconfig.NMSaddr.s6_addr);
  memcpy(g_devconfig.ieee_eui64.data, g_eui64, sizeof(g_eui64));
  g_devconfig.reginterval_min = reg_interval_min;
  g_devconfig.reginterval_max = reg_interval_max;

  for (i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-min") == 0) {   // reginterval_min
      if (++i >= argc)
        goto start_error;
      g_devconfig.reginterval_min = strtol(argv[i], &endptr, 0);
      if (*endptr != '\0')
        goto start_error;
    } else if (strcmp(argv[i], "-max") == 0) {   // reginterval_max
      if (++i >= argc)
        goto start_error;
      g_devconfig.reginterval_max = strtol(argv[i], &endptr, 0);
      if (*endptr != '\0')
        goto start_error;
    } else if (strcmp(argv[i], "-eid") == 0) {   // EUI64 address
      if (++i >= argc)
        goto start_error;
      memset(g_devconfig.ieee_eui64.data, 0, sizeof(g_devconfig.ieee_eui64.data));
      if (str2addr(argv[i], g_devconfig.ieee_eui64.data) < 0)
        goto start_error;
      if (*endptr != '\0')
        goto start_error;
    } else if (strcmp(argv[i], "-d") == 0) {  // NMS address
      if (++i >= argc)
        goto start_error;
      if (osal_inet_pton(AF_INET6, argv[i], &g_devconfig.NMSaddr.s6_addr) <= 0) {
        printf("NMS address in presentation format\n");
        goto start_error;
      }
    } else if (strcmp(argv[i], "-sig") == 0) {  // Signature Settings
      if (++i >= argc)
        goto start_error;
      if (strcmp(argv[i], "true") == 0) {
        printf("setting signature settings to TRUE\n");
        sigFlag = true;
      }
    }
  }

  //Setting signature settings according to input in command line
  if (sigFlag)
  {
    //csmp signature settings data true
    g_devconfig.csmp_sig_settings.reqsignedpost = true;
    g_devconfig.csmp_sig_settings.reqvalidcheckpost = true;
    g_devconfig.csmp_sig_settings.reqtimesyncpost = true;
    g_devconfig.csmp_sig_settings.reqseclocalpost = true;
    g_devconfig.csmp_sig_settings.reqsignedresp = true;
    g_devconfig.csmp_sig_settings.reqvalidcheckresp = true;
    g_devconfig.csmp_sig_settings.reqtimesyncresp = true;
    g_devconfig.csmp_sig_settings.reqseclocalresp = true;
  }
  else
  {
    //csmp signature settings data false
    g_devconfig.csmp_sig_settings.reqsignedpost = false;
    g_devconfig.csmp_sig_settings.reqvalidcheckpost = false;
    g_devconfig.csmp_sig_settings.reqtimesyncpost = false;
    g_devconfig.csmp_sig_settings.reqseclocalpost = false;
    g_devconfig.csmp_sig_settings.reqsignedresp = false;
    g_devconfig.csmp_sig_settings.reqvalidcheckresp = false;
    g_devconfig.csmp_sig_settings.reqtimesyncresp = false;
    g_devconfig.csmp_sig_settings.reqseclocalresp = false;
  }

  /* check reginterval_max and reginterval_min */
  if (g_devconfig.reginterval_max < g_devconfig.reginterval_min
      || g_devconfig.reginterval_min == 0
      || g_devconfig.reginterval_max > 36000) {
    printf("reg interval error\n");

    goto start_error;
  }

  /*************************************************************
    init the csmp_handle parameter of csmp_service_start func:
      * callback function for the GET TLV request
      * callback function for the POST TLV request
      * callback function for the signature verification
  **************************************************************/
  g_csmp_handle.csmptlvs_get = (csmptlvs_get_t)csmptlvs_get;
  g_csmp_handle.csmptlvs_post = (csmptlvs_post_t)csmptlvs_post;
  g_csmp_handle.signature_verify = (signature_verify_t)signature_verify;

  // start csmp agent lib service
  ret = csmp_service_start(&g_devconfig, &g_csmp_handle);
  if(ret < 0)
    printf("start csmp agent service: fail!\n");
  else
    printf("start csmp agent service: success!\n");

  // get the regmin and regmax
  printf("min : %" PRIuLEAST32 ", max = %" PRIuLEAST32 "\n",g_devconfig.reginterval_min, g_devconfig.reginterval_max);

  while(1) {
    osal_sleep_ms(g_devconfig.reginterval_min * 1000UL);

    // get the service status
    status = csmp_service_status();
    printf("%s\n",status_msg[status]);

    // get the stats of CSMP agent service
    stats_ptr = csmp_service_stats();
    printf("-------------- CSMP service stats --------------\n");
    printf(" reg_succeed: %" PRIuLEAST32 "\n reg_attempts: %" PRIuLEAST32 "\n reg_fails: %" PRIuLEAST32 "\n\
        \n *** reg_fail reason ***\n  error_coap: %" PRIuLEAST32 "\n  error_signature: %" PRIuLEAST32 "\n  error_process: %" PRIuLEAST32 "\n\
        \n metrics_reports: %" PRIuLEAST32 "\n csmp_get_succeed: %" PRIuLEAST32 "\n csmp_post_succeed: %" PRIuLEAST32 "\n\
        \n sig_ok: %" PRIuLEAST32 "\n sig_no_signature: %" PRIuLEAST32 "\n sig_bad_auth: %" PRIuLEAST32 "\n sig_bad_validity: %" PRIuLEAST32 "\n",\
        stats_ptr->reg_succeed,stats_ptr->reg_attempts,stats_ptr->reg_fails,\
        stats_ptr->reg_fails_stats.error_coap,stats_ptr->reg_fails_stats.error_signature,\
        stats_ptr->reg_fails_stats.error_process,stats_ptr->metrics_reports,\
        stats_ptr->csmp_get_succeed,stats_ptr->csmp_post_succeed,stats_ptr->sig_ok,\
        stats_ptr->sig_no_signature,stats_ptr->sig_bad_auth,stats_ptr->sig_bad_validity);
    printf("---------------------- end --------------------\n");
  }

  //stop csmp agent service
  ret = csmp_service_stop();
  if(ret)
    printf("stop csmp agent service: success!\n");
  else
    printf("stop csmp agent service: fail!\n");

start_error:
  printf("start csmp agent service: fail!\n");
  return NULL;
}

// FreeRTOS Thread function
#else

static void csmp_sample_app_thr_fnc(void *arg)
{

  (void) arg;

  struct timeval tv = {0};
  csmp_service_status_t status;
  csmp_service_stats_t *stats_ptr;
  char *status_msg[] = {"CSMP service is not started\n",
                       "Failed to start CSMP service\n",
                       "Registration is in progress...\n",
                       "Regist to the NMS successfully\n"};
  int ret;
  bool sigFlag = false;
  
  printf("Csmp Agent Lib Sample Application\n");

#if defined(OSAL_EFR32_WISUN)
  sl_wisun_app_core_util_connect_and_wait();

  if (sl_wisun_ntp_timesync() != SL_STATUS_OK) {
    printf("Warning: NTP Time Sync failed\n");
  }
#endif
  
  osal_gettimeofday(&tv, NULL);
  g_init_time = tv.tv_sec;

  /**************************************************************
    init the dev_config parameter of csmp_service_start func:
      * NMS address
      * EUI64
      * register interval(min, max)
  ***************************************************************/
  memset(&g_devconfig, 0, sizeof(dev_config_t));
  osal_inet_pton(AF_INET6, NMS_IP, &g_devconfig.NMSaddr.s6_addr);
  memcpy(g_devconfig.ieee_eui64.data, g_eui64, sizeof(g_eui64));
  g_devconfig.reginterval_min = reg_interval_min;
  g_devconfig.reginterval_max = reg_interval_max;

#if defined(CSMP_AGENT_REG_INTERVAL_MIN)
  g_devconfig.reginterval_min = CSMP_AGENT_REG_INTERVAL_MIN;
#endif

#if defined(CSMP_AGENT_REG_INTERVAL_MAX)
  g_devconfig.reginterval_max = CSMP_AGENT_REG_INTERVAL_MAX;
#endif

#if defined(CSMP_AGENT_EUI64_ADDRESS)
  memset(g_devconfig.ieee_eui64.data, 0, sizeof(g_devconfig.ieee_eui64.data));
  assert(str2addr(CSMP_AGENT_EUI64_ADDRESS, g_devconfig.ieee_eui64.data) >= 0);
#endif

#if defined(CSMP_AGENT_NMS_ADDRESS)
  assert(osal_inet_pton(AF_INET6, CSMP_AGENT_NMS_ADDRESS, &g_devconfig.NMSaddr.s6_addr) > 0);
#endif

#if defined(CSMP_AGENT_SIGNATURE_SETTINGS)
  if (CSMP_AGENT_SIGNATURE_SETTINGS) {
    printf("setting signature settings to TRUE\n");
    sigFlag = true;
  }
#endif

  //Setting signature settings according to input in command line
  if (sigFlag)
  {
    //csmp signature settings data true
    g_devconfig.csmp_sig_settings.reqsignedpost = true;
    g_devconfig.csmp_sig_settings.reqvalidcheckpost = true;
    g_devconfig.csmp_sig_settings.reqtimesyncpost = true;
    g_devconfig.csmp_sig_settings.reqseclocalpost = true;
    g_devconfig.csmp_sig_settings.reqsignedresp = true;
    g_devconfig.csmp_sig_settings.reqvalidcheckresp = true;
    g_devconfig.csmp_sig_settings.reqtimesyncresp = true;
    g_devconfig.csmp_sig_settings.reqseclocalresp = true;
  }
  else
  {
    //csmp signature settings data false
    g_devconfig.csmp_sig_settings.reqsignedpost = false;
    g_devconfig.csmp_sig_settings.reqvalidcheckpost = false;
    g_devconfig.csmp_sig_settings.reqtimesyncpost = false;
    g_devconfig.csmp_sig_settings.reqseclocalpost = false;
    g_devconfig.csmp_sig_settings.reqsignedresp = false;
    g_devconfig.csmp_sig_settings.reqvalidcheckresp = false;
    g_devconfig.csmp_sig_settings.reqtimesyncresp = false;
    g_devconfig.csmp_sig_settings.reqseclocalresp = false;
  }

  /* check reginterval_max and reginterval_min */
  if (g_devconfig.reginterval_max < g_devconfig.reginterval_min
      || g_devconfig.reginterval_min == 0
      || g_devconfig.reginterval_max > 36000) {
    printf("reg interval error\n");
    assert(false);
  }

  /*************************************************************
    init the csmp_handle parameter of csmp_service_start func:
      * callback function for the GET TLV request
      * callback function for the POST TLV request
      * callback function for the signature verification
  **************************************************************/
  g_csmp_handle.csmptlvs_get = (csmptlvs_get_t)csmptlvs_get;
  g_csmp_handle.csmptlvs_post = (csmptlvs_post_t)csmptlvs_post;
  g_csmp_handle.signature_verify = (signature_verify_t)signature_verify;

  // start csmp agent lib service
  ret = csmp_service_start(&g_devconfig, &g_csmp_handle);
  if(ret < 0)
    printf("start csmp agent service: fail!\n");
  else
    printf("start csmp agent service: success!\n");

  // get the regmin and regmax
  printf("min : %" PRIuLEAST32 ", max = %" PRIuLEAST32 "\n",g_devconfig.reginterval_min, g_devconfig.reginterval_max);

  while(1) {
    osal_sleep_ms(g_devconfig.reginterval_min * 1000UL);

    // get the service status
    status = csmp_service_status();
    printf("%s\n",status_msg[status]);

    // get the stats of CSMP agent service
    stats_ptr = csmp_service_stats();
    printf("-------------- CSMP service stats --------------\n");
    printf(" reg_succeed: %" PRIuLEAST32 "\n reg_attempts: %" PRIuLEAST32 "\n reg_fails: %" PRIuLEAST32 "\n\
        \n *** reg_fail reason ***\n  error_coap: %" PRIuLEAST32 "\n  error_signature: %" PRIuLEAST32 "\n  error_process: %" PRIuLEAST32 "\n\
        \n metrics_reports: %" PRIuLEAST32 "\n csmp_get_succeed: %" PRIuLEAST32 "\n csmp_post_succeed: %" PRIuLEAST32 "\n\
        \n sig_ok: %" PRIuLEAST32 "\n sig_no_signature: %" PRIuLEAST32 "\n sig_bad_auth: %" PRIuLEAST32 "\n sig_bad_validity: %" PRIuLEAST32 "\n",\
        stats_ptr->reg_succeed,stats_ptr->reg_attempts,stats_ptr->reg_fails,\
        stats_ptr->reg_fails_stats.error_coap,stats_ptr->reg_fails_stats.error_signature,\
        stats_ptr->reg_fails_stats.error_process,stats_ptr->metrics_reports,\
        stats_ptr->csmp_get_succeed,stats_ptr->csmp_post_succeed,stats_ptr->sig_ok,\
        stats_ptr->sig_no_signature,stats_ptr->sig_bad_auth,stats_ptr->sig_bad_validity);
    printf("---------------------- end --------------------\n");
  }

  //stop csmp agent service
  ret = csmp_service_stop();
  if(ret)
    printf("stop csmp agent service: success!\n");
  else
    printf("stop csmp agent service: fail!\n");
}
#endif

/**************************************************************
  usage: ./CsmpAgentLib_sample
          [-d NMS_ipv6_address]
          [-min reginterval_min]
          [-max reginterval_max]
          [-eid ieee_eui64]
***************************************************************/
int main(int argc, char **argv)
{
  static osal_task_t app_task = { 0 };
  thread_argument_t *args = NULL;
  osal_basetype_t ret = 0;

// Initialize thread arguments
#if defined(OSAL_FREERTOS_LINUX) || defined(OSAL_LINUX)
  thread_argument_t linux_arg = {
    .argc = argc,
    .argv = argv
  };
  args = &linux_arg;
#else
  (void) argc;
  (void) argv;
  args = NULL;
#endif

#if defined(OSAL_EFR32_WISUN)
    sl_system_init();
#endif

// Create Sample application task
  ret = osal_task_create(&app_task, 
                         NULL, 
                         0, 
                         0, 
                         csmp_sample_app_thr_fnc, 
                         args);
  assert(ret == 0);

// Start Kernel
  osal_kernel_start();

  for(;;){
#if defined(OSAL_LINUX)
  osal_sleep_ms(1000);
#else
  assert(0);
#endif
  }

  return 0;
}

