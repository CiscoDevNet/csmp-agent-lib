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

#include "osal_common.h"
#include "../../src/lib/debug.h"
#include "sl_system_kernel.h"

#define OSAL_EFR32_WISUN_MIN_STACK_SIZE_WORDS 4096

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
static void osal_alarm_fired_pend_fnc(void * param1, uint32_t param2);

/****************************************************************************
 * @fn        osal_kernel_start
 * @brief     start the kernel
*****************************************************************************/
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

/****************************************************************************
 * @fn        osal_task_create
 *
 * @brief     create new task/thread. 
 * input parameters
 *  @param[in] thread address to store thread ID
 *  @param[in] name name of the task
 *  @param[in] priotiy of task 
 *  @param[in] stacksize size of the thread stack.
 *  @param[in] entry the task is created executing entry 
 *  @param[in] arg entry routine arguments 
 *
 * output parameters
 * @return    osal_basetype_t on success return 0 otherwise error value
 *****************************************************************************/
osal_basetype_t osal_task_create(
   osal_task_t * thread,
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

/****************************************************************************
 * @fn        osal_task_cancel
 *
 * @brief     function requests that thread be canceled. 
 * input parameters
 *  @param[in] thread thread ID to be canceled
 *
 * output parameters
 * @return   on success return 0 otherwise error value 
 *****************************************************************************/
osal_basetype_t osal_task_cancel(osal_task_t thread)
{
    vTaskDelete(thread);
    return 0;
}

/****************************************************************************
 * @fn        osal_task_setcanceltype()
 *
 * @brief     function atomically both sets the calling thread's cancelability 
 * 	      state and returns oldstate 
 * input parameters
 * output parameters
 * @return   on success return 0 otherwise error value 
 *****************************************************************************/
osal_basetype_t osal_task_setcanceltype()
{
    return 0;
}

/****************************************************************************
 * @fn        osal_sem_create()
 *
 * @brief   initialize an unnamed semaphore. 
 *
 * input parameters
 *  @param[in] sem address to store semaphore 
 *  @param[in] value initial value of semaphore
 *
 * output parameters
 * @return 0 on success; on error, -1 is returned 
 *****************************************************************************/
osal_basetype_t osal_sem_create(osal_sem * sem, uint16_t value)
{
    if (sem == NULL) {
        return -1;
    }

    *sem = xSemaphoreCreateCounting(0xFFFF, value);

    return *sem == NULL ? -1 : 0;
}

/****************************************************************************
 * @fn        osal_sem_post()
 *
 * @brief   increments (unlocks) the semaphore pointed to by sem 
 *
 * input parameters
 *  @param[in] sem semaphore 
 *
 * output parameters
 * @return 0 on success; on error, -1 is returned 
 *****************************************************************************/
osal_basetype_t osal_sem_post(osal_sem * sem)
{
    osal_basetype_t ret = 0;

    if (sem == NULL) {
        return -1;
    }
    ret = xSemaphoreGive(*sem);

    return __ret_freertos2posix(ret);
}

/****************************************************************************
 * @fn        osal_sem_wait()
 *
 * @brief   decrements (locks) the semaphore pointed to by sem
 *
 * input parameters
 *  @param[in] sem semaphore 
 *  @param[in] timeout value to wait if semaphore is not availbel.
 *
 * output parameters
 * @return 0 on success; on error, -1 is returned 
 *****************************************************************************/
osal_basetype_t osal_sem_wait(osal_sem * sem, osal_time_t timeout)
{
    osal_basetype_t ret = 0;
    if (sem == NULL) {
        return -1;
    }

    ret = xSemaphoreTake(*sem, timeout);

    return __ret_freertos2posix(ret);
}

/****************************************************************************
 * @fn        osal_sem_destroy()
 *
 * @brief  destroys the unnamed semaphore at the address pointed to by sem 
 *
 * input parameters
 *  @param[in] sem semaphore 
 *
 * output parameters
 * @return 0 on success; on error, -1 is returned 
 *****************************************************************************/
osal_basetype_t osal_sem_destroy(osal_sem *sem)
{
    if (sem == NULL) {
        return -1;
    }
    vSemaphoreDelete(*sem);
    return 0;
}

/****************************************************************************
 * @fn        osal_socket
 *
 * @brief   creates an endpoint for communication
 *
 * input parameters
 *  @param[in] domain communication domain
 *  @param[in] type communication semantics
 *  @param[in] protocol specifies a particular protocol to be used with the socket.
 *
 * output parameters
 * @return On success, a file descriptor for the new socket is returned.  
 *         On error, -1 is returned
 *****************************************************************************/
osal_socket_handle_t osal_socket(osal_basetype_t domain, osal_basetype_t type, osal_basetype_t protocol)
{
    return socket(domain, type, protocol);
}


/****************************************************************************
 * @fn        osal_bind
 *
 * @brief   assigns the address specified by osal_addr to the socket 
 *          referred to by the file descriptor osal_sockfd.
 *
 * input parameters
 *  @param[in] osal_sockfd socket file descriptor 
 *  @param[in] osal_addr network address
 *  @param[in] addrlen size in bytes of the address structure 
 *
 * output parameters
 * @return On success 0 is returned.  On error, -1 is returned 
 *****************************************************************************/
osal_basetype_t osal_bind(osal_socket_handle_t osal_sockfd, osal_sockaddr *osal_addr, 
                            osal_socklen addrlen)
{
    return bind(osal_sockfd, (const struct sockaddr *)(osal_addr), addrlen);
}

/****************************************************************************
 * @fn      osal_recvfrom
 *
 * @brief  used to receive messages from a socket. 
 *
 * input parameters
 *  @param[in] sockfd socket file descriptor 
 *  @param[out] buf contains received messages 
 *  @param[in] len size of buffer 
 *  @param[in] flags argument is formed by ORing one or many socket options 
 *  @param[in] src_addr source address 
 *  @param[in] addrlen size in bytes of the address structure
 *
 * output parameters
 * @return On success 0 is returned.  On error, -1 is returned 
 *****************************************************************************/
osal_ssize_t osal_recvfrom(int sockfd, void *buf, size_t len, int flags,
                    osal_sockaddr *src_addr, osal_socklen *addrlen)
{
    return recvfrom(sockfd, buf, len, flags, (struct sockaddr*)(src_addr), addrlen);
}

/****************************************************************************
 * @fn      osal_sendmsg
 *
 * @brief  used to transmit a message to another socket. 
 *
 * input parameters
 *  @param[in] sockfd socket file descriptor 
 *  @param[in] msghdr structure contains message meta data.
 *  @param[in] flags argument is formed by ORing one or many socket options 
 *
 * output parameters
 * @return On success return the number of bytes sent on error -1 is returned 
 *****************************************************************************/
ssize_t osal_sendmsg(int sockfd, const struct msghdr msg, int flags){
    return sendmsg(sockfd, &msg, flags);
}

/****************************************************************************
 * @fn      osal_sendto
 *
 * @brief  used to transmit a message to another socket. 
 *
 * input parameters
 *  @param[in] sockfd socket file descriptor 
 *  @param[in] buf message buffer
 *  @param[in] len data buffer len 
 *  @param[in] flags argument is formed by ORing one or many socket options 
 *  @param[in] dest_addr destination address 
 *  @param[in] addrlen address length
 *
 * output parameters
 * @return On success return the number of bytes sent on error -1 is returned 
 *****************************************************************************/
ssize_t osal_sendto(int sockfd, const void *buf, size_t len, int flags,
                      const osal_sockaddr *dest_addr, osal_socklen addrlen)
{
    return sendto(sockfd, buf, len, flags, (struct sockaddr*)(dest_addr), addrlen);
}

/****************************************************************************
 * @fn      osal_inet_pton
 *
 * @brief  This function converts the character string src into a network 
 *         address structure in the af address family 
 *
 * input parameters
 *  @param[in] af address family 
 *  @param[in] src character string 
 *  @param[out] dst network address structure
 *
 * output parameters
 * @return 1 on success
 *         0 if src does not contain a character string representing a valid
 *           network address in the specified address family
 *        -1 if af does not contain a valid address family
 *****************************************************************************/
osal_basetype_t osal_inet_pton(int af, const char *src, void *dst)
{
    return inet_pton(af, src, dst);
}

/****************************************************************************
 * @fn      osal_select
 *
 * @brief Allows a program to monitor multiple file descriptors, waiting until
 *        one or more of the file descriptors become "ready" for some class of 
 *        I/O operatio 
 *
 * input parameters
 *  @param[in] nfds This argument should be set to the highest-numbered file 
 *                  descriptor plus one
 *  @param[in] readfds The file descriptors in this set are watched to see if 
 *                     they are ready for reading 
 *  @param[in] writefds The file descriptors in this set are watched to see 
 *                      if they are ready for writing.
 *  @param[in] exceptfds The  file descriptors in this set are watched for 
 *                     "exceptional conditions". 
 *  @param[in] timeout The interval that osal_select() should block waiting 
 *                     for a file descriptor to become ready
 *
 * output parameters
 * @return on success returns the number of file descriptors contained in the 
 *         three returned descriptor sets 
 *         0 if timeout expired  before any file descriptors became ready. 
 *        -1 on error
 *****************************************************************************/
osal_basetype_t osal_select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout)
{
    (void)nfds;
    (void)readfds;
    (void)writefds;
    (void)exceptfds;
    (void)timeout;
    return 0;  
}

/****************************************************************************
 * @fn     osal_update_sockaddr 
 *
 * @brief  update the ipv6 socket address structure
 *
 * input parameters
 *  @param[in] listen_addr address of sockaddr structure 
 *  @param[in] sport source port 
 * output parameters
 * @return none
 *****************************************************************************/
void osal_update_sockaddr(osal_sockaddr *listen_addr, uint16_t sport)
{
    listen_addr->sin6_family = AF_INET6;
    listen_addr->sin6_addr = in6addr_any;
    listen_addr->sin6_port = htons(sport);
}

/****************************************************************************
 * @fn     osal_fd_zero 
 *
 * @brief clears (removes all file descriptors from) set 
 *
 * input parameters
 *  @param[in] set file descriptor set 
 * output parameters
 * @return none
 *****************************************************************************/
void osal_fd_zero(fd_set *set)
{
    (void) set;
}

/****************************************************************************
 * @fn     osal_fd_set 
 *
 * @brief  This  macro  adds the file descriptor fd to set
 *
 * input parameters
 *  @param[in] fd file descriptor 
 *  @param[in] set file descriptor set 
 * output parameters
 * @return none
 *****************************************************************************/
void osal_fd_set(int fd, fd_set *set)
{
    (void) fd;
    (void) set;
}

/****************************************************************************
 * @fn     osal_fd_isset 
 *
 * @brief  modifies  the  contents of the sets
 *
 * input parameters
 *  @param[in] fd file descriptor 
 *  @param[in] set file descriptor set 
 * output parameters
 * @return returns nonzero if the file  descriptor fd is present in set, 
 *         and zero if it is not 
 *****************************************************************************/
osal_basetype_t osal_fd_isset(int fd, fd_set *set)
{
    (void) fd;
    (void) set;
    return 0;
}

/****************************************************************************
 * @fn     osal_gettimeofday
 *
 * @brief get time as well as a timezone 
 *
 * input parameters
 *  @param[in] tv is a struct timeval 
 *  @param[in] tz is a struct timezone
 * output parameters
 * @return 0 for success, or -1 for failure 
 *****************************************************************************/
osal_basetype_t osal_gettimeofday(struct timeval *tv, struct timezone *tz)
{
  sl_sleeptimer_timestamp_64_t time = 0;
  sl_sleeptimer_time_zone_offset_t timezone = 0;
  
  time = sl_sleeptimer_get_time_64();
  timezone = sl_sleeptimer_get_tz();
  
  if (tv == NULL) {
    return -1;
  }

  tv->tv_sec = time;
  tv->tv_usec = 0UL;

  if (tz != NULL) {
    tz->tz_minuteswest = timezone / 60;
    tz->tz_dsttime = 0;
  }

  return 0;
}

/****************************************************************************
 * @fn     osal_settimeofday
 *
 * @brief set time as well as a timezone 
 *
 * input parameters
 *  @param[in] tv is a struct timeval 
 *  @param[in] tz is a struct timezone
 * output parameters
 * @return 0 for success, or -1 for failure 
 *****************************************************************************/
osal_basetype_t osal_settime(struct timeval *tv, struct timezone *tz)
{
  sl_sleeptimer_timestamp_64_t time = 0;
  if (tv == NULL) {
    return -1;
  }
  time = tv->tv_sec + tv->tv_usec / 1000000;
  if (tz != NULL) {
    sl_sleeptimer_set_time_64(time);
    sl_sleeptimer_set_tz(tz->tz_minuteswest * 60);
  }
  return 0;
}

/****************************************************************************
 * @fn    osal_print_formatted_ip
 *
 * @brief print formatted ip v6 address from sockadd for debugging purpose
 *
 * input parameters
 *  @param[in] sockadd socket address structure
 * output parameters
 * @return none 
 *****************************************************************************/
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

/****************************************************************************
 * @fn   osal_malloc
 *
 * @brief allocate memory
 *
 * input parameters
 *  @param[in] size size of memory to be allocated
 * output parameters
 * @return pointer to allocated memory on success, NULL on failure
 *****************************************************************************/
void *osal_malloc(size_t size)
{
  return sl_malloc(size);
}

/****************************************************************************
 * @fn   osal_calloc
 *
 * @brief allocate memory and set to zero
 *
 * input parameters
 *  @param[in] num number of elements
 *  @param[in] size size of memory to be allocated
 * output parameters
 * @return pointer to allocated memory on success, NULL on failure
 *****************************************************************************/
void *osal_calloc(size_t num, size_t size)
{
  return sl_calloc(num, size);
}

/****************************************************************************
 * @fn   osal_realloc
 *
 * @brief reallocate memory
 *
 * input parameters
 *  @param[in] ptr pointer to memory to be reallocated
 *  @param[in] size size of memory to be allocated
 * output parameters
 * @return pointer to allocated memory on success, NULL on failure
 *****************************************************************************/
void *osal_realloc(void *ptr, size_t size)
{
  return sl_realloc(ptr, size);
}

/****************************************************************************
 * @fn   osal_free
 *
 * @brief free memory
 *
 * input parameters
 *  @param[in] ptr pointer to memory to be freed
 * output parameters
 * @return none
 *****************************************************************************/
void osal_free(void *ptr)
{
  sl_free(ptr);
}

/****************************************************************************
 * @fn   osal_sleep_ms
 *
 * @brief sleep for given time
 *
 * input parameters
 *  @param[in] ms time in milliseconds to sleep
 * output parameters
 * @return none
 *****************************************************************************/
void osal_sleep_ms(uint64_t ms)
{
  vTaskDelay(pdMS_TO_TICKS(ms));
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
