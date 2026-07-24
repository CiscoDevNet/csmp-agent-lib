/*
 *  Copyright 2026 Renesas Electronics Corporation
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

/**
 * @file  osal_renesas_wisun.c
 * @brief Implementation of the OSAL CSMP interface to be used within the Renesas Wi-SUN FAN sample application.
 *
 * @note This file calls Renesas-provided Wi-SUN FAN stack, timer, clock, memory, logging, and firmware update APIs. See
 * Vendors/Renesas/Readme.md for the Renesas library requirements and function usage summary.
 */

#include "osal.h"
#include "csmp.h"
#include "csmp_service.h"
#include "CsmpAgentLib_sample_tlvs.h"
#include "signature_verify.h"
#include "r_nwk_api.h"
#include "r_mem_tools.h"
#include "r_impl_utils.h"
#include "r_ipv6_helper.h"
#if R_FWUP_SUPPORT
#include "r_fwup_wrapper.h"
#endif
#include "lib/random.h"
#include "osal_renesas.h"

#define MY_HEAP_ID R_HEAP_ID_CSMP
#include "r_heap.h"

// R_LOG_PREFIX is processed by RLog code generator
#define R_LOG_PREFIX CSMP
#include "r_log_internal.h"
#if R_LOG_THRESHOLD > R_LOG_SEVERITY_OFF
#include "r_loggen_osal_renesas_wisun.h"
#endif

typedef struct r_trickle_timer_s
{
    r_timer_id_t          id;
    uint32_t              t0;
    uint32_t              tfire;
    uint32_t              icur;
    uint32_t              imin;
    uint32_t              imax;
    uint8_t               is_running : 1;
    r_timer_t             cb_timer;
    trickle_timer_fired_t csmp_cb;  //!< The CSMP callback function that should be executed once this timer expires
} r_trickle_timer_t;

static r_trickle_timer_t timers[timer_num];

uint16_t socket_ports[CSMP_MAX_SOCKETS];

csmp_handle_t r_csmp_handle;

/** The csmp module is uninitialized by default (R_FALSE) */
static r_boolean_t csmpIsInitialized = R_FALSE;

osal_ssize_t osal_sendmsg(osal_socket_handle_t sockd, const struct msghdr msg, osal_basetype_t flags)
{
    if (msg.msg_iov == NULL)
    {
        R_LOG_ERR("r_sendmsg failed: data vector is null");
        return R_RESULT_INVALID_PARAMETER;
    }

    if (msg.msg_name == NULL)
    {
        R_LOG_ERR("r_sendmsg failed: address is null");
        return R_RESULT_INVALID_PARAMETER;
    }

    const osal_sockaddr_t *addr = msg.msg_name;
    osal_socklen_t addrlen = msg.msg_namelen;

    osal_ssize_t total_sent = 0;

    for (size_t i = 0; i < msg.msg_iovlen; ++i)
    {
        const struct iovec *iov = &msg.msg_iov[i];
        const uint8_t *buf = (const uint8_t *)iov->iov_base;
        size_t len = iov->iov_len;

        while (len > 0)
        {
            osal_ssize_t n = osal_sendto(sockd, buf, len, flags, addr, addrlen);

            if (n <= 0)
            {
                return OSAL_FAILURE;
            }

            buf        += (size_t)n;
            len        -= (size_t)n;
            total_sent += n;
        }
    }

    return total_sent;
}

osal_ssize_t osal_sendto(osal_socket_handle_t sockd, const void *buf, size_t len, osal_basetype_t flags,
                         const osal_sockaddr_t *dest_addr, osal_socklen_t addrlen)
{
    if (sockd < 0 || sockd >= CSMP_MAX_SOCKETS || addrlen != sizeof(struct sockaddr_in6))
    {
        return R_RESULT_INVALID_PARAMETER;
    }

    struct sockaddr_in6* dest = (struct sockaddr_in6*)dest_addr;
    if (dest->sin6_family != AF_INET6)
    {
        return R_RESULT_INVALID_PARAMETER;
    }

    uint16_t dstPort = ntohs(dest->sin6_port);  // port in socket struct is network byte order but R_UDP_DataRequest expects host byte order
    uint16_t srcPort = ntohs(socket_ports[sockd]); // port in socket struct is network byte order but R_UDP_DataRequest expects host byte order
    r_result_t res = R_UDP_DataRequest(dest->sin6_addr.s6_addr, dstPort, srcPort, buf, len, 0);
    if (res == R_RESULT_SUCCESS)
    {
        R_LOG_DBG("Transmission of %{u16} bytes to %{ipv6addr}:%{u16} from port %{u16}", len, dest->sin6_addr.s6_addr, dstPort, srcPort);
        return len;
    }
    R_LOG_ERR("Transmission of %{u16} bytes to %{ipv6addr} failed: 0x%{hex8}", len, dest->sin6_addr.s6_addr, res);
    return OSAL_FAILURE;
}

void osal_print_formatted_ip(const osal_sockaddr_t *sockadd)
{
    R_LOG_DBG("[%{ipv6addr}]:%{u16}\n", sockadd->sin6_addr.s6_addr, sockadd->sin6_port);
}

static void R_TrickleTimer_HandleExpired(void* timer)
{
    r_trickle_timer_t* trickleTimer = timer;
    if (trickleTimer->id == reg_timer)
    {
        R_LOG_DBG("Register trickle timer fired");
    }
    else if (trickleTimer->id == rpt_timer)
    {
        R_LOG_DBG("Periodic metrics report trickle timer fired");
    }
    else if (trickleTimer->id == lrq_timer)
    {
        R_LOG_DBG("Firmware load request timer fired");
    }
    else if (trickleTimer->id == async_timer)
    {
        R_LOG_DBG("Async CSMP Reponse timer fired");
    }
    else
    {
        R_LOG_ERR("Invalid trickle timer id");
    }

    /* Set timer again */
    trickleTimer->t0 += trickleTimer->icur;  // update t0 to next interval

    /* Double interval size */
    trickleTimer->icur <<= 1;
    if (trickleTimer->icur > trickleTimer->imax)
    {
        trickleTimer->icur = trickleTimer->imax;
    }

    uint32_t min = trickleTimer->icur >> 1;
    trickleTimer->tfire = trickleTimer->t0 + min + (random32() % (trickleTimer->icur - min));
    r_result_t res = R_Timer_StartSecs(&trickleTimer->cb_timer, trickleTimer->icur, R_TrickleTimer_HandleExpired, trickleTimer);
    if (res != R_RESULT_SUCCESS)
    {
        R_LOG_ERR("Fatal error: Trickle timer could not be restarted");
    }

    /* Execute CSMP callback */
    trickleTimer->csmp_cb();
}

void osal_trickle_timer_start(osal_timerid_t timerid, uint32_t imin, uint32_t imax, trickle_timer_fired_t trickle_timer_fired)
{
    if (timerid >= timer_num)
    {
        R_LOG_ERR("Invalid trickle timer id");
        return;  // Invalid timer ID
    }

    if (timerid == reg_timer)
    {
        R_LOG_DBG("Register trickle timer start");
    }
    else if (timerid == rpt_timer)
    {
        R_LOG_DBG("Periodic metrics report trickle timer start");
    }
    else if (timerid == lrq_timer)
    {
        R_LOG_DBG("Firmware load request timer start");
    }
    else if (timerid == async_timer)
    {
        R_LOG_DBG("Async CSMP Reponse timer start");
    }

    timers[timerid].t0 = clock_seconds() + (random32() % imin);
    timers[timerid].icur = imin;
    timers[timerid].imin = imin;
    timers[timerid].imax = imax;
    uint32_t min = timers[timerid].icur >> 1;
    timers[timerid].tfire = timers[timerid].t0 + min + (random32() % (timers[timerid].icur - min));
    timers[timerid].id = timerid;
    timers[timerid].csmp_cb = trickle_timer_fired;

    r_result_t timerRes = R_Timer_StartSecs(&timers[timerid].cb_timer, timers[timerid].icur, R_TrickleTimer_HandleExpired, &timers[timerid]);
    if (timerRes != R_RESULT_SUCCESS)
    {
        R_LOG_ERR("Fatal error: Timer %{u8} could not be started: 0x%{hex8}", (uint8_t)timerid, timerRes);
        return;
    }

    R_LOG_DBG("Start timer %{u8} (expires in %{u32}s)", timerid, timers[timerid].icur);
    timers[timerid].is_running = R_TRUE;
}

void osal_trickle_timer_stop(osal_timerid_t timerid)
{
    if (timerid >= timer_num)
    {
        R_LOG_ERR("Invalid trickle timer id");
        return;  // Invalid timer ID
    }

    r_result_t res = R_Timer_Stop(&timers[timerid].cb_timer);

    if (timerid == reg_timer)
    {
        R_LOG_DBG("Register trickle timer stop");
    }
    else if (timerid == rpt_timer)
    {
        R_LOG_DBG("Periodic metrics report trickle timer stop");
    }
    else if (timerid == lrq_timer)
    {
        R_LOG_DBG("Firmware load request timer stop");
    }
    else if (timerid == async_timer)
    {
        R_LOG_DBG("Async CSMP Reponse timer stop");
    }

    switch (res)
    {
    case R_RESULT_SUCCESS:
        R_LOG_DBG("Timer %{u8} stopped", timerid);
        timers[timerid].is_running = R_FALSE;
        break;

    case R_RESULT_NOTHING_TO_DO:
        break;  // Do nothing (timer was not started)

    default:
        R_LOG_ERR("Trickle timer could not be stopped: 0x%{hex8}", res);
        break;
    }
}

void *osal_malloc(size_t size)
{
    return my_alloc(size);
}

void osal_free(void *ptr)
{
    my_free(ptr);
}

osal_basetype_t osal_gettime(struct timeval *tv, struct timezone *tz)
{
    if (tv == NULL)
    {
        return OSAL_FAILURE;
    }

    tv->tv_sec = R_CLOCK_GetUnixTimestamp();;
    tv->tv_usec = 0UL;

    if (tz != NULL)
    {
        tz->tz_minuteswest = 0;
        tz->tz_dsttime = 0;
    }

    return OSAL_SUCCESS;
}

osal_basetype_t osal_settime(struct timeval *tv, struct timezone *tz)
{
    if (tv == NULL)
    {
        return OSAL_FAILURE;
    }
    uint64_t currentUnixTime = tv->tv_sec + tv->tv_usec / 1000000;

    if (tz != NULL)
    {
        currentUnixTime += tz->tz_minuteswest * 60;
        if (tz->tz_dsttime)
        {
            currentUnixTime -= 3600;
        }
    }

    R_CLOCK_SetUnixTimestampReference(currentUnixTime);

    return OSAL_SUCCESS;
}

/*
 * The following functions are not used in any CSMP platform independent code.
 * For the Renesas platform, the functionalities are already provided by the dedicated
 * FW update module. The corresponding methods are then directly called when processing
 * the relevant TLVs.
 */
osal_basetype_t osal_system_reboot(struct in6_addr *NMSaddr)
{
    return OSAL_FAILURE;
}

osal_basetype_t osal_read_firmware(uint8_t slotid, uint8_t *data, uint32_t size)
{
    return OSAL_FAILURE;
}

osal_basetype_t osal_write_firmware(uint8_t slotid, uint8_t *data, uint32_t size)
{
    return OSAL_FAILURE;
}

osal_basetype_t osal_write_slothdr(uint8_t slotid, Csmp_Slothdr *slot)
{
    return OSAL_FAILURE;
}

osal_basetype_t osal_read_slothdr(uint8_t slotid, Csmp_Slothdr *slot)
{
    return OSAL_FAILURE;
}

osal_basetype_t osal_copy_firmware(uint8_t source_slotid, uint8_t dest_slotid, Csmp_Slothdr *slot)
{
    return OSAL_FAILURE;
}

osal_basetype_t osal_task_create(osal_task_t * thread,
                                 const char * name,
                                 uint32_t priority,
                                 size_t stacksize,
                                 osal_task_fnc_t entry,
                                 void * arg)
{
    return OSAL_SUCCESS;
}

osal_basetype_t osal_task_cancel(osal_task_t thread)
{
    return OSAL_SUCCESS;
}

osal_socket_handle_t osal_socket(osal_basetype_t domain,
                                 osal_basetype_t type,
                                 osal_basetype_t protocol)
{
    osal_socket_handle_t sockd = 0;
    uint16_t port = CSMP_DEFAULT_PORT + 1;
    while (sockd < CSMP_MAX_SOCKETS)
    {
        if (socket_ports[sockd] == 0)
        {
            socket_ports[sockd] = htons(port);
            return sockd;
        }
        port = ntohs(socket_ports[sockd]) + 1;
        sockd++;
    }
    return OSAL_FAILURE;
}

osal_basetype_t osal_socket_close(osal_socket_handle_t sockd)
{
    if (sockd < 0 || sockd >= CSMP_MAX_SOCKETS)
    {
        return OSAL_FAILURE;
    }
    socket_ports[sockd] = 0;
    return OSAL_SUCCESS;
}

osal_basetype_t osal_bind(osal_socket_handle_t sockd, osal_sockaddr_t *addr, osal_socklen_t addrlen)
{
    if (sockd >= CSMP_MAX_SOCKETS || sockd < 0)
    {
        return OSAL_FAILURE;
    }
    socket_ports[sockd] = addr->sin6_port;
    return OSAL_SUCCESS;
}

osal_basetype_t osal_task_setcanceltype()
{
    return OSAL_FAILURE;
}

void osal_update_sockaddr(osal_sockaddr_t *listen_addr, uint16_t sport)
{
    listen_addr->sin6_port = htons(sport);
}

int R_CSMP_Start(const r_ipv6addr_t* serverAddr, const uint8_t* localEui64, uint32_t regImin, uint32_t regImax)
{
    dev_config_t devConfig = { 0 };
    if (!serverAddr || R_IPv6_IsUnspecified(serverAddr) || R_IPv6_IsMulticast(serverAddr))
    {
        if (serverAddr)
        {
            R_LOG_WARN("CSMP could not be started due to invalid NMS address: %{ipv6addr}", serverAddr->bytes);
        }
        else
        {
            R_LOG_WARN("CSMP could not be started due to missing NMS address");
        }
        return R_RESULT_INVALID_PARAMETER;
    }
    R_memcpy(devConfig.NMSaddr.s6_addr, serverAddr->bytes, R_IPV6_ADDRESS_LENGTH);
    R_memcpy(devConfig.ieee_eui64.data, localEui64, R_EUI64_LEN);
    devConfig.reginterval_min = regImin;
    devConfig.reginterval_max = regImax;

    if (devConfig.reginterval_max < devConfig.reginterval_min || devConfig.reginterval_min == 0
        || devConfig.reginterval_max > 36000)
    {
        R_LOG_ERR("reg interval error (min=%{u32}; max=%{u32})", devConfig.reginterval_min, devConfig.reginterval_max);
        return R_RESULT_INVALID_PARAMETER;
    }

    /* Enable the CSMP signature parameters */
    devConfig.csmp_sig_settings.reqsignedpost = true;
    devConfig.csmp_sig_settings.reqvalidcheckpost = true;
    devConfig.csmp_sig_settings.reqtimesyncpost = true;
    devConfig.csmp_sig_settings.reqseclocalpost = true;
    devConfig.csmp_sig_settings.reqsignedresp = true;
    devConfig.csmp_sig_settings.reqvalidcheckresp = true;
    devConfig.csmp_sig_settings.reqtimesyncresp = true;
    devConfig.csmp_sig_settings.reqseclocalresp = true;

    /*************************************************************
      init the csmp_handle parameter of csmp_service_start func:
        * callback function for the GET TLV request
        * callback function for the POST TLV request
        * callback function for the signature verification
    **************************************************************/
    r_csmp_handle.csmptlvs_get = (csmptlvs_get_t)csmptlvs_get;
    r_csmp_handle.csmptlvs_post = (csmptlvs_post_t)csmptlvs_post;
    r_csmp_handle.signature_verify = (signature_verify_t)signature_verify;

    if (csmp_service_start(&devConfig, &r_csmp_handle) < 0)
    {
        R_LOG_ERR("start csmp agent service: fail!");
        return 1;
    }
    R_LOG_DBG("start csmp agent service: success!");

    /* Initialize csmpIsInitialized for the subsequent stop */
    csmpIsInitialized = R_TRUE;
    return 0;
}

int R_CSMP_Stop()
{
    if (csmpIsInitialized)
    {
        /* Set csmpIsInitialized to FALSE to avoid a double reset. */
        csmpIsInitialized = R_FALSE;

        if (csmp_service_stop())
        {
            R_LOG_DBG("stop csmp agent service: success!");
            return 0;
        }
        else
        {
            R_LOG_ERR("stop csmp agent service: fail!");
            return 1;
        }
    }
    /* return immediately */
    return 0;
}
