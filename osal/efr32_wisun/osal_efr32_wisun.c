/*
 *  Copyright 2024 Cisco Systems, Inc.
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

#include "osal.h"
#include "../../src/lib/debug.h"
#include "sl_system_kernel.h"

#define OSAL_EFR32_WISUN_MIN_STACK_SIZE_WORDS 4096

// SIZES
#define SHA1_HASH_SIZE        20
#define SHA256_HASH_SIZE      32
#define FILE_NAME_SIZE        128
#define VERSION_SIZE          32
#define BITMAP_SIZE           32
#define HWID_SIZE             32
#define BLOCK_SIZE            1024

// IMAGE SLOT INFO
#define CSMP_FWMGMT_ACTIVE_SLOTS      3          // 0-RUN, 1-UPLOAD, 2-BACKUP
#define CSMP_FWMGMT_SLOTIMG_SIZE      (30*1024)  // ~30 Kb
#define CSMP_FWMGMT_BLKMAP_CNT        (32)

struct trickle_timer {
  uint32_t t0;
  uint32_t tfire;
  uint32_t icur;
  uint32_t imin;
  uint32_t imax;
  uint8_t is_running :1;
  TimerHandle_t timer;
};

#define __ret_freertos2posix(ret) \
    (ret == pdPASS ? OSAL_SUCCESS : OSAL_FAILURE)

extern uint8_t g_csmplib_eui64[8];

/// Vars for trickle timers
static struct trickle_timer timers[timer_num];
static trickle_timer_fired_t timer_fired[timer_num];
static int32_t m_remaining = ((1UL << 31) - 1) / 1000; /* max int32_t */
static bool m_timert_isrunning = false;

static void osal_update_timer();
static void osal_alarm_fired(TimerHandle_t xTimer);
static void osal_alarm_fired_pend_fnc(void * param1, uint32_t param2);
static void print_csmp_slot_hdr(const struct _Csmp_Slothdr *slot_hdr);

void osal_kernel_start(void)
{
    for (BaseType_t i = 0; i < timer_num; i++) {
        timers[i].is_running = false;
        timers[i].timer = xTimerCreate("trickle_timer", 
                                       pdMS_TO_TICKS(m_remaining * 1000), 
                                       pdTRUE, 
                                       (void *)i, 
                                       osal_alarm_fired);
        assert(timers[i].timer != NULL);
        xTimerStop(timers[i].timer, 0);

    }

    sl_system_kernel_start();
}

osal_basetype_t osal_task_create(osal_task_t * thread,
                                 const char * name,
                                 uint32_t priority,
                                 size_t stacksize,
                                 osal_task_fnc_t entry,
                                 void * arg)
{
    osal_basetype_t ret = 0;
    ret = xTaskCreate(entry, 
                      name, 
                      (OSAL_EFR32_WISUN_MIN_STACK_SIZE_WORDS + stacksize), 
                      arg, 
                      priority, 
                      thread);

    return __ret_freertos2posix(ret);
}

osal_basetype_t osal_task_cancel(osal_task_t thread)
{
    vTaskDelete(thread);
    return OSAL_SUCCESS;
}

osal_basetype_t osal_task_setcanceltype()
{
    return OSAL_SUCCESS;
}

osal_basetype_t osal_task_sigmask(osal_basetype_t how, 
                                  const osal_sigset_t *set, 
                                  osal_sigset_t *oldset)
{
  (void) how;
  (void) set;
  (void) oldset;
  return OSAL_FAILURE;
}

osal_basetype_t osal_sem_create(osal_sem_t * sem, uint16_t value)
{
    if (sem == NULL) {
        return OSAL_FAILURE;
    }

    *sem = xSemaphoreCreateCounting(0xFFFF, value);

    return *sem == NULL ? OSAL_FAILURE : OSAL_SUCCESS;
}

osal_basetype_t osal_sem_post(osal_sem_t * sem)
{
    osal_basetype_t ret = OSAL_FAILURE;

    if (sem == NULL) {
        return OSAL_FAILURE;
    }
    ret = xSemaphoreGive(*sem);

    return __ret_freertos2posix(ret);
}

osal_basetype_t osal_sem_wait(osal_sem_t * sem, osal_time_t timeout)
{
    osal_basetype_t ret = OSAL_FAILURE;
    if (sem == NULL) {
        return OSAL_FAILURE;
    }

    ret = xSemaphoreTake(*sem, timeout);

    return __ret_freertos2posix(ret);
}

osal_basetype_t osal_sem_destroy(osal_sem_t *sem)
{
    if (sem == NULL) {
        return OSAL_FAILURE;
    }
    vSemaphoreDelete(*sem);
    return OSAL_SUCCESS;
}

osal_socket_handle_t osal_socket(osal_basetype_t domain, 
                                 osal_basetype_t type, 
                                 osal_basetype_t protocol)
{
    return socket(domain, type, protocol);
}

osal_ssize_t osal_recvfrom(osal_socket_handle_t sockd, void *buf, size_t len, osal_basetype_t flags,
                           osal_sockaddr_t *src_addr, osal_socklen_t *addrlen)
{
    return recvfrom(sockd, buf, len, flags, (struct sockaddr*)(src_addr), addrlen);
}

osal_ssize_t osal_sendmsg(osal_socket_handle_t sockd, const struct msghdr msg, osal_basetype_t flags)
{
    return sendmsg(sockd, &msg, flags);
}

osal_basetype_t osal_bind(osal_socket_handle_t sockd, osal_sockaddr_t *addr, osal_socklen_t addrlen)
{
    return bind(sockd, (const struct sockaddr *)addr, addrlen);
}

osal_ssize_t osal_sendto(osal_socket_handle_t sockd, const void *buf, size_t len, osal_basetype_t flags,
                         const osal_sockaddr_t *dest_addr, osal_socklen_t addrlen)
{
    return sendto(sockd, buf, len, flags, (struct sockaddr*)(dest_addr), addrlen);
}

osal_basetype_t osal_inet_pton(osal_basetype_t af, const char *src, void *dst)
{
    return inet_pton(af, src, dst);
}

osal_basetype_t osal_select(osal_basetype_t nsds, osal_sd_set_t *readsds, osal_sd_set_t *writesds,
                            osal_sd_set_t *exceptsds, struct timeval *timeout)
{
    (void)nsds;
    (void)readsds;
    (void)writesds;
    (void)exceptsds;
    (void)timeout;
    return OSAL_FAILURE;  
}


void osal_sd_zero(osal_sd_set_t *set)
{
    (void) set;
}

void osal_sd_set(osal_socket_handle_t sd, osal_sd_set_t *set)
{
    (void) sd;
    (void) set;
}

osal_basetype_t osal_sd_isset(osal_socket_handle_t sd, osal_sd_set_t *set)
{
    (void) sd;
    (void) set;
    return OSAL_FAILURE;
}

void osal_update_sockaddr(osal_sockaddr_t *listen_addr, uint16_t sport)
{
    listen_addr->sin6_family = AF_INET6;
    listen_addr->sin6_addr = in6addr_any;
    listen_addr->sin6_port = htons(sport);
}

osal_basetype_t osal_gettime(struct timeval *tv, struct timezone *tz)
{
  sl_sleeptimer_timestamp_t time = 0;
  sl_sleeptimer_time_zone_offset_t timezone = (SL_WISUN_NTP_TIMESYNC_TIMEZONE_UTC_OFFSET_HOUR * 60 * 60);
  
  time = sl_sleeptimer_get_time();
  timezone = sl_sleeptimer_get_tz();
  
  if (tv == NULL) {
    return OSAL_FAILURE;
  }

  tv->tv_sec = time;
  tv->tv_usec = 0UL;

  if (tz != NULL) {
    tz->tz_minuteswest = timezone / 60;
    tz->tz_dsttime = 0;
  }

  return OSAL_SUCCESS;
}

osal_basetype_t osal_settime(struct timeval *tv, struct timezone *tz)
{
  sl_sleeptimer_timestamp_t time = 0;
  if (tv == NULL) {
    return OSAL_FAILURE;
  }
  time = tv->tv_sec + tv->tv_usec / 1000000;
  if (tz != NULL) {
    sl_sleeptimer_set_time(time);
    sl_sleeptimer_set_tz(tz->tz_minuteswest * 60);
  }
  return OSAL_SUCCESS;
}
osal_sighandler_t osal_signal(osal_basetype_t signum, osal_sighandler_t handler)
{
  (void) signum;
  (void) handler;
  return NULL;
}

osal_basetype_t osal_sigprocmask(osal_basetype_t how, const osal_sigset_t *set, osal_sigset_t *oldset)
{
  (void) how;
  (void) set;
  (void) oldset;
  return OSAL_FAILURE;
}

osal_basetype_t osal_sigemptyset(osal_sigset_t *set)
{
  (void) set;
  return OSAL_FAILURE;
}

osal_basetype_t osal_sigaddset(osal_sigset_t *set, osal_basetype_t signum)
{
  (void) set;
  (void) signum;
  return OSAL_FAILURE;
}

void osal_print_formatted_ip(const osal_sockaddr_t *sockadd)
{
    (void) sockadd;
    DPRINTF("[%x:%x:%x:%x:%x:%x:%x:%x]:%hu\n",
      ((uint16_t)sockadd->sin6_addr.s6_addr[0] << 8) | sockadd->sin6_addr.s6_addr[1],
      ((uint16_t)sockadd->sin6_addr.s6_addr[2] << 8) | sockadd->sin6_addr.s6_addr[3],
      ((uint16_t)sockadd->sin6_addr.s6_addr[4] << 8) | sockadd->sin6_addr.s6_addr[5],
      ((uint16_t)sockadd->sin6_addr.s6_addr[6] << 8) | sockadd->sin6_addr.s6_addr[7],
      ((uint16_t)sockadd->sin6_addr.s6_addr[8] << 8) | sockadd->sin6_addr.s6_addr[9],
      ((uint16_t)sockadd->sin6_addr.s6_addr[10] << 8) |  sockadd->sin6_addr.s6_addr[11],
      ((uint16_t)sockadd->sin6_addr.s6_addr[12] << 8) |  sockadd->sin6_addr.s6_addr[13],
      ((uint16_t)sockadd->sin6_addr.s6_addr[14] << 8) |  sockadd->sin6_addr.s6_addr[15],
      ntohs(sockadd->sin6_port));
}

void osal_trickle_timer_start(osal_timerid_t timerid, uint32_t imin, uint32_t imax, 
                              trickle_timer_fired_t trickle_timer_fired)
{
  uint32_t min;
  struct timeval tv = {0};
  uint32_t seed = 0;

  if(!m_timert_isrunning) {
    m_timert_isrunning = true;
  }

  switch (timerid) {
    case reg_timer: DPRINTF("Register trickle timer start\n");
    break;

    case rpt_timer: DPRINTF("Periodic metrics report trickle timer start\n");
    break;

    case lrq_timer: DPRINTF("Firmware load request timer start\n");
    break;

    case async_timer: DPRINTF("Async CSMP Reponse timer start\n");
    break;

    default: DPRINTF("Error: Invalid trickle timer id\n");
  }

  osal_gettime(&tv, NULL);

  seed = (((uint16_t)g_csmplib_eui64[6] << 8) | g_csmplib_eui64[7]);
  srand(seed);
  timers[timerid].t0 = tv.tv_sec + (rand()%imin);
  timers[timerid].icur = imin;
  timers[timerid].imin = imin;
  timers[timerid].imax = imax;
  timers[timerid].is_running = true;
  timer_fired[timerid] = trickle_timer_fired;
  min = timers[timerid].icur >> 1;
  timers[timerid].tfire = timers[timerid].t0 + min + (rand() % (timers[timerid].icur - min));
  xTimerStart(timers[timerid].timer, 0);
  osal_update_timer();
}

void osal_trickle_timer_stop(osal_timerid_t timerid)
{
  uint8_t i;

  timers[timerid].is_running = false;

  switch (timerid) {
    case reg_timer: DPRINTF("Register trickle timer stop\n");
    break;

    case rpt_timer: DPRINTF("Periodic metrics report trickle timer stop\n");
    break;

    case lrq_timer: DPRINTF("Firmware load request timer stop\n");
    break;

    case async_timer: DPRINTF("Async CSMP Reponse timer stop\n");
    break;

    default: DPRINTF("Error: Invalid trickle timer id\n");
  }

  for(i = 0; i < timer_num; i++) {
    if(timers[i].is_running)
      return;
  }
  m_timert_isrunning = false;
  xTimerStop(timers[timerid].timer, 0);
}


void *osal_malloc(size_t size)
{
  return sl_malloc(size);
}

void osal_free(void *ptr)
{
  sl_free(ptr);
}

void osal_sleep_ms(uint64_t ms)
{
  vTaskDelay(pdMS_TO_TICKS(ms));
}


void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    char * pcTaskName )
{
    ( void ) xTask;
    ( void ) pcTaskName;
    DPRINTF("Stack overflow: %s\n", pcTaskName);
    for( ;; );
}

static void osal_update_timer() {
  uint32_t now;
  struct timeval tv = {0};
  uint8_t i;

  m_remaining = ((1UL << 31) - 1) / 1000;
  osal_gettime(&tv, NULL);
  now = tv.tv_sec;

  for (i = 0; i < timer_num; i++) {
    struct trickle_timer *timer = &timers[i];
    int32_t remaining;

    if (timer->is_running == false)
      continue;
    remaining = timer->tfire - now;
    if (remaining < m_remaining) {
      m_remaining = remaining;
      if (m_remaining <= 0) {
        xTimerPendFunctionCall(osal_alarm_fired_pend_fnc, 
                               NULL, 
                               0, 
                               pdFALSE);
      } else {
        xTimerChangePeriod(timers[i].timer, pdMS_TO_TICKS(m_remaining * 1000), 0);
      }
    }
  }
}

static void osal_alarm_fired(TimerHandle_t xTimer)
{
  uint32_t min;
  uint8_t i;

  (void) xTimer;

  for (i = 0; i < timer_num; i++) {
    struct trickle_timer *timer = &timers[i];
    uint32_t now;
    struct timeval tv = {0};

    osal_gettime(&tv, NULL);
    now = tv.tv_sec;

    if (timer->is_running == false)
      continue;

    if ((int32_t)(timer->tfire - now) > 0)
      continue;

    // update t0 to next interval
    timer->t0 += timer->icur;

    // double interval size
    timer->icur <<= 1;
    if (timer->icur > timer->imax)
      timer->icur = timer->imax;

    switch (i) {
      case reg_timer: DPRINTF("Register trickle timer fired\n");
      break;

      case rpt_timer: DPRINTF("Periodic metrics report trickle timer fired\n");
      break;

      case lrq_timer: DPRINTF("Firmware load request timer fired\n");
      break;

      case async_timer: DPRINTF("Async CSMP Reponse timer fired\n");
      break;

      default: DPRINTF("Error: Invalid trickle timer id\n");
    }

    timer_fired[i]();
    min = timer->icur >> 1;
    timer->tfire = timer->t0 + min + (rand() % (timer->icur - min));
  }
  osal_update_timer();
}

static void osal_alarm_fired_pend_fnc(void * param1, uint32_t param2)
{
  (void) param1;
  (void) param2;
  osal_alarm_fired(NULL);
}


static void print_csmp_slot_hdr(const struct _Csmp_Slothdr *slot_hdr)
{
  DPRINTF("filehash: ");
  for (int i = 0; i < SHA256_HASH_SIZE; i++) {
    DPRINTF("%02x,", slot_hdr->filehash[i]);
  }
  DPRINTF("\n");
  DPRINTF("filename: %s\n", slot_hdr->filename);
  DPRINTF("version: %s\n", slot_hdr->version);
  DPRINTF("hwid: %s\n", slot_hdr->hwid);
  DPRINTF("filesize: %lu\n", slot_hdr->filesize);
  DPRINTF("blocksize: %lu\n", slot_hdr->blocksize);
  DPRINTF("reportintervalmin: %lu\n", slot_hdr->reportintervalmin);
  DPRINTF("reportintervalmax: %lu\n", slot_hdr->reportintervalmax);
  DPRINTF("status: 0x%08x\n", slot_hdr->status);
  DPRINTF("nblkmap: ");
  for (int i = 0; i < CSMP_FWMGMT_BLKMAP_CNT; i++) {
    DPRINTF("%08x,", slot_hdr->nblkmap[i]);
  }
  DPRINTF("\n");
}

/**
 * @brief Read firmware slot data from storage
 *
 * @param slotid indicates RUN/UPLOAD/BACKUP slot
 * @return int 0 for success -1 for failure
 */
osal_basetype_t osal_read_firmware(uint8_t slotid, void* slot, uint32_t size)
{
  const struct _Csmp_Slothdr *slot_hdr = (struct _Csmp_Slothdr *)slot;

  switch(slotid) {
    case RUN_IMAGE:
      DPRINTF("Reading Run Slot:\n");
      break;
    case UPLOAD_IMAGE:
      DPRINTF("Reading Upload Slot:\n");
      break;
    case BACKUP_IMAGE:
      DPRINTF("Reading Backup Slot:\n");
      break;
    default:
      printf("read_firmware: Invalid slot id\n");
      return OSAL_FAILURE;
  }
  print_csmp_slot_hdr(slot_hdr);

  return OSAL_SUCCESS;
}

/**
 * @brief Write firmware slot data to storage
 *
 * @param slotid indicates RUN/UPLOAD/BACKUP slot
 * @return int 0 for success -1 for failure
 */
osal_basetype_t osal_write_firmware(uint8_t slotid, void* slot, uint32_t size)
{
  const struct _Csmp_Slothdr *slot_hdr = (struct _Csmp_Slothdr *)slot;

  switch(slotid) {
    case RUN_IMAGE:
      DPRINTF("Writing Run Slot:\n");
      break;
    case UPLOAD_IMAGE:
      DPRINTF("Writing Upload Slot:\n");
      break;
    case BACKUP_IMAGE:
      DPRINTF("Writing Backup Slot:\n");
      break;
    default:
      printf("read_firmware: Invalid slot id\n");
      return OSAL_FAILURE;
  }
  print_csmp_slot_hdr(slot_hdr);

  return OSAL_SUCCESS;
}
