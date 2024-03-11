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
    (ret == pdPASS ? 0 : -1)

extern uint8_t g_csmplib_eui64[8];

/// Vars for trickle timers
static struct trickle_timer timers[timer_num];
static trickle_timer_fired_t timer_fired[timer_num];
static int32_t m_remaining = ((1UL << 31) - 1) / 1000; /* max int32_t */
static bool m_timert_isrunning = false;

static void osal_update_timer();
static void osal_alarm_fired(TimerHandle_t xTimer);

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
    vTaskStartScheduler();
}

osal_basetype_t osal_task_create (
   osal_task_t * thread,
   const char * name,
   uint32_t priority,
   size_t stacksize,
   void *(*entry) (void * arg),
   void * arg)
{
    osal_basetype_t ret = 0;
    ret = xTaskCreate((TaskFunction_t)entry, 
                      name, 
                      ((PTHREAD_STACK_MIN / sizeof(size_t)) + stacksize), 
                      arg, 
                      priority, 
                      thread);

    return __ret_freertos2posix(ret);
}

osal_basetype_t osal_task_cancel(osal_task_t thread)
{
    vTaskDelete(thread);
    return 0;
}

osal_basetype_t osal_task_setcanceltype()
{
    return (0);
}

osal_basetype_t osal_task_sigmask(int how, const osal_sigset_t *set, osal_sigset_t *oldset)
{
    (void) how;
    (void) set;
    (void) oldset;
    return (0);
}

//sem_init
osal_basetype_t osal_sem_create(osal_sem_t * sem, uint16_t value)
{
    if (sem == NULL) {
        return -1;
    }

    *sem = xSemaphoreCreateCounting(0xFFFF, value);

    return *sem == NULL ? -1 : 0;
}

//sem_post
osal_basetype_t osal_sem_post(osal_sem_t * sem)
{
    osal_basetype_t ret = 0;

    if (sem == NULL) {
        return -1;
    }
    ret = xSemaphoreGive(*sem);

    return __ret_freertos2posix(ret);
}

//sem_wait
osal_basetype_t osal_sem_wait(osal_sem_t * sem, osal_time_t timeout)
{
    osal_basetype_t ret = 0;
    if (sem == NULL) {
        return -1;
    }

    ret = xSemaphoreTake(*sem, timeout);

    return __ret_freertos2posix(ret);
}

//sem_destroy
osal_basetype_t osal_sem_destroy(osal_sem_t *sem)
{
    if (sem == NULL) {
        return -1;
    }
    vSemaphoreDelete(*sem);
    return 0;
}

//Socket
osal_socket_handle_t osal_socket(osal_basetype_t domain, osal_basetype_t type, osal_basetype_t protocol)
{
    return socket(domain, type, protocol);
}

//bind
osal_basetype_t osal_bind(osal_socket_handle_t sockd, osal_sockaddr_t *addr, 
                            osal_socklen_t addrlen)
{
    return bind(osal_sockfd, (const struct sockaddr *)(osal_addr), addrlen);
}

//recvfrom
osal_ssize_t osal_recvfrom(int sockd, void *buf, size_t len, int flags,
                    osal_sockaddr_t *src_addr, osal_socklen_t *addrlen)
{
    return recvfrom(sockfd, buf, len, flags, (struct sockaddr*)(src_addr), addrlen);
}

//sendmsg
ssize_t osal_sendmsg(int sockd, const struct msghdr msg, int flags){
    return sendmsg(sockfd, &msg, flags);
}

//sendto
ssize_t osal_sendto(int sockd, const void *buf, size_t len, int flags,
                      const osal_sockaddr_t *dest_addr, osal_socklen_t addrlen)
{
    return sendto(sockfd, buf, len, flags, (struct sockaddr*)(dest_addr), addrlen);
}

//inet_pton

osal_basetype_t osal_inet_pton(int af, const char *src, void *dst)
{
    return inet_pton(af, src, dst);
}

//select
osal_basetype_t osal_select(int nsds, osal_sd_set_t *readsds, osal_sd_set_t *writesds,
                  osal_sd_set_t *exceptsds, struct timeval *timeout)
{
    return select(nfds, readfds, writefds, exceptfds, timeout);
}

void osal_update_sockaddr(osal_sockaddr *listen_addr, uint16_t sport)
{
    listen_addr->sin6_family = AF_INET6;
    listen_addr->sin6_addr = in6addr_any;
    listen_addr->sin6_port = htons(sport);
}

void osal_sd_zero(osal_sd_set_t *set)
{
    FD_ZERO(set);
}

void osal_sd_set(int sd, osal_sd_set_t *set)
{
    FD_SET(fd, set);
}

osal_basetype_t osal_sd_isset(int sd, osal_sd_set_t *set)
{
    return(FD_ISSET(fd, set));
}

//gettimeofday
osal_basetype_t osal_gettime(struct timeval *tv, struct timezone *tz)
{
    return gettimeofday(tv, tz);
}

//settime
osal_basetype_t osal_settime(struct timeval *tv, struct timezone *tz)
{
    return settimeofday(tv, tz);
}

osal_sighandler_t osal_signal(int signum, osal_sighandler_t handler)
{
    (void) signum;
    (void) handler;
    return (0);
}

osal_basetype_t osal_sigprocmask(int how, const osal_sigset_t *set, osal_sigset_t *oldset)
{
    (void) how;
    (void) set;
    (void) oldset;
    return (0);
}

osal_basetype_t osal_sigemptyset(osal_sigset_t *set)
{
    (void) set;
    return (0);
}

osal_basetype_t osal_sigaddset(osal_sigset_t *set, int signum)
{
    (void) set;
    (void) signum;
    return (0);
}

void osal_print_formatted_ip(const osal_sockaddr *sockadd)
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


/****************************************************************************
 * @fn   osal_trickle_timer_start
 *
 * @brief starts trickle timer
 *
 * input parameters
 *  @param[in] timerid timer identification value
 *  @param[in] imin minimum timer value
 *  @param[in] imax maximum timer value
 *  @param[in] trickle_timer_fired  timer fired 
 * output parameters
 * @return none 
 *****************************************************************************/
void osal_trickle_timer_start(timerid_t timerid, uint32_t imin, uint32_t imax, trickle_timer_fired_t trickle_timer_fired)
{
  uint32_t min;
  struct timeval tv = {0};
  uint32_t seed = 0;

  if(!m_timert_isrunning) {
    m_timert_isrunning = true;
  }

  if(timerid == reg_timer) {
    DPRINTF("register trickle timer start\n");
  }
  else if(timerid == rpt_timer) {
    DPRINTF("metrics report trickle timer start\n");
  }

  osal_gettimeofday(&tv, NULL);

  seed = (((uint16_t)g_csmplib_eui64[6] << 8) | g_csmplib_eui64[7]);
  srand(seed);
  timers[timerid].t0 = tv.tv_sec + (random()%imin);
  timers[timerid].icur = imin;
  timers[timerid].imin = imin;
  timers[timerid].imax = imax;
  timers[timerid].is_running = true;
  timer_fired[timerid] = trickle_timer_fired;
  min = timers[timerid].icur >> 1;
  timers[timerid].tfire = timers[timerid].t0 + min + (random() % (timers[timerid].icur - min));
  xTimerStart(timers[timerid].timer, 0);
  osal_update_timer();
}

/****************************************************************************
 * @fn   osal_trickle_timer_stop
 *
 * @brief stop trickle timer
 *
 * input parameters
 *  @param[in] timerid timer identification value
 * output parameters
 * @return none 
 *****************************************************************************/
void osal_trickle_timer_stop(timerid_t timerid)
{
  uint8_t i;

  timers[timerid].is_running = false;
  if(timerid == reg_timer) {
    DPRINTF("register trickle timer stop\n");
  }
  else if(timerid == rpt_timer) {
    DPRINTF("metrics report trickle timer stop\n");
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
  return malloc(size);
}
void osal_free(void *ptr)
{
  free(ptr);
}
void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    char * pcTaskName )
{
    ( void ) xTask;
    ( void ) pcTaskName;
    printf("Stack overflow: %s\n", pcTaskName);
    for( ;; );
}

static void osal_update_timer() {
  uint32_t now;
  struct timeval tv = {0};
  uint8_t i;
//   bool flag = false;

  m_remaining = (1UL << 31) - 1; /* max int32_t */
  osal_gettimeofday(&tv, NULL);
  now = tv.tv_sec;

  for (i = 0; i < timer_num; i++) {
    struct trickle_timer *timer = &timers[i];
    int32_t remaining;

    if (timer->is_running == false)
      continue;
    remaining = timer->tfire - now;
    if (remaining < m_remaining) {
      m_remaining = remaining;
    //   flag = true;
      if (m_remaining <= 0) {
        xTimerPendFunctionCall((PendedFunction_t)osal_alarm_fired, 
                               NULL, 
                               0, 
                               pdFALSE);
      } else {
        xTimerChangePeriod(timers[i].timer, pdMS_TO_TICKS(m_remaining * 1000), 0);
      }
    }
  }
//   if(flag)
//     osal_sem_post(&sem);
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

    osal_gettimeofday(&tv, NULL);
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

    if(i == reg_timer) {
      DPRINTF("register trickle timer fired\n");
    }
    else if(i == rpt_timer) {
      DPRINTF("metrics report trickle timer fired\n");
    }

    timer_fired[i]();
    min = timer->icur >> 1;
    timer->tfire = timer->t0 + min + (random() % (timer->icur - min));
  }
  osal_update_timer();
}

