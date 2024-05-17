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

struct trickle_timer {
  uint32_t t0;
  uint32_t tfire;
  uint32_t icur;
  uint32_t imin;
  uint32_t imax;
  uint8_t is_running :1;
};
extern uint8_t g_csmplib_eui64[8];

static struct trickle_timer timers[timer_num];
static trickle_timer_fired_t timer_fired[timer_num];

static osal_task_t timer_id_task;
static osal_sem_t sem;

static int32_t m_remaining = (1UL << 31) - 1; /* max int32_t */
static bool m_timert_isrunning = false;

static void osal_update_timer();
static void osal_alarm_fired();

osal_basetype_t osal_task_create(osal_task_t * thread,
                                 const char * name,
                                 uint32_t priority,
                                 size_t stacksize,
                                 void* (*entry) (void * arg),
                                 void * arg)
{
    /* Silence compiler warnings about unused parameters. */
   (void) name;
   (void) priority;
   osal_basetype_t ret;
   pthread_attr_t attr;
   
   if (thread == NULL) {
       return OSAL_FAILURE;
   }

   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN + stacksize);
  
   ret = pthread_create(thread, &attr, (void *)entry, arg);
   if (ret != 0){
       DPRINTF("pthread_create failed: %d\n", ret);
       return OSAL_FAILURE;
   }
   ret = pthread_detach(*thread);
   if (ret != 0){
       DPRINTF("pthread_detach failed: %d\n", ret);
       return OSAL_FAILURE;
   }

   return OSAL_SUCCESS;
}

osal_basetype_t osal_task_cancel(osal_task_t thread)
{
    return (pthread_cancel(thread));
}

osal_basetype_t osal_task_setcanceltype(osal_basetype_t type, osal_basetype_t *oldtype){
    return (pthread_setcanceltype(type, oldtype));
}

osal_basetype_t osal_task_sigmask(osal_basetype_t how, const osal_sigset_t *set, osal_sigset_t *oldset)
{
    return (pthread_sigmask(how, set, oldset));
}

osal_basetype_t osal_sem_create(osal_sem_t * sem, uint16_t value)
{
    return (sem_init(sem, 0, value));
}

osal_basetype_t osal_sem_post(osal_sem_t * sem)
{
    return (sem_post(sem));
}

osal_basetype_t osal_sem_wait(osal_sem_t * sem, osal_time_t timeout)
{
    /* Silence compiler warnings about unused parameters. */
    (void) timeout;
    return(sem_wait(sem));
}

osal_basetype_t osal_sem_destroy(osal_sem_t *sem)
{
    return (sem_destroy(sem)); 
}

osal_socket_handle_t osal_socket(osal_basetype_t domain, osal_basetype_t type, osal_basetype_t protocol)
{
    return(socket(domain, type, protocol));
}

osal_basetype_t osal_bind(osal_socket_handle_t sockd, osal_sockaddr_t *addr, osal_socklen_t addrlen)
{
    return (bind(sockd, (const struct sockaddr *)(addr), addrlen));
}

osal_ssize_t osal_recvfrom(osal_socket_handle_t sockd, void *buf, size_t len, osal_basetype_t flags,
                        osal_sockaddr_t *src_addr, osal_socklen_t *addrlen)
{
    return (recvfrom(sockd, buf, len, flags, (struct sockaddr*)(src_addr), addrlen));

}

osal_ssize_t osal_sendmsg(osal_socket_handle_t sockd, const struct msghdr msg, osal_basetype_t flags)
{
    return(sendmsg(sockd, &msg, flags));
}

osal_ssize_t osal_sendto(osal_socket_handle_t sockd, const void *buf, size_t len, osal_basetype_t flags,
                         const osal_sockaddr_t *dest_addr, osal_socklen_t addrlen)
{
    return(sendto(sockd, buf, len, flags, (struct sockaddr*)(dest_addr), addrlen));
}

osal_basetype_t osal_inet_pton(osal_basetype_t af, const char *src, void *dst)
{
    return(inet_pton(af, src, dst));
}

osal_basetype_t osal_select(osal_basetype_t nsds, osal_sd_set_t *readsds, osal_sd_set_t *writesds,
                  osal_sd_set_t *exceptsds, struct timeval *timeout)
{
    return(select(nsds, readsds, writesds, exceptsds, timeout));
}

void osal_update_sockaddr(osal_sockaddr_t *listen_addr, uint16_t sport)
{
    listen_addr->sin6_family = AF_INET6;
    listen_addr->sin6_addr = in6addr_any;
    listen_addr->sin6_port = htons(sport);
}

void osal_sd_zero(osal_sd_set_t *set)
{
    FD_ZERO(set);
}

void osal_sd_set(osal_socket_handle_t sd, osal_sd_set_t *set)
{
    FD_SET(sd, set);
}

osal_basetype_t osal_sd_isset(osal_socket_handle_t sd, osal_sd_set_t *set)
{
    return(FD_ISSET(sd, set));
}

osal_basetype_t osal_gettime(struct timeval *tv, struct timezone *tz)
{
    return(gettimeofday(tv, tz));
}

osal_basetype_t osal_settime(struct timeval *tv, struct timezone *tz)
{
    return(settimeofday(tv, tz));
}

osal_sighandler_t osal_signal(osal_basetype_t signum, osal_sighandler_t handler)
{
    return(signal(signum, handler));
}

osal_basetype_t osal_sigprocmask(osal_basetype_t how, const osal_sigset_t *set, osal_sigset_t *oldset)
{
    return(sigprocmask(how, set, oldset));
}

osal_basetype_t osal_sigemptyset(osal_sigset_t *set)
{
     return(sigemptyset(set));
}

osal_basetype_t osal_sigaddset(osal_sigset_t *set, osal_basetype_t signum)
{
    return(sigaddset(set, signum));
}

void osal_print_formatted_ip(const osal_sockaddr_t *sockadd)
{
    /* Silence compiler warnings about unused parameters. */
    (void)sockadd;
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

static void *osal_timer_thread(void* arg) {
  osal_sigset_t set;
  /* Silence compiler warnings about unused parameters. */
  (void)arg;
  osal_sigemptyset(&set);
  osal_sigaddset(&set, SIGALRM);
  osal_task_sigmask(SIG_UNBLOCK, &set, NULL);

  osal_signal(SIGALRM, osal_alarm_fired);
  while(1) {
    osal_sem_wait(&sem, 0);  //Suspend thread until sem_post()
    if (m_remaining <= 0)
      osal_alarm_fired();
    else {
      DPRINTF("trickle timer next fired time:%d sec\n", m_remaining);
      alarm(m_remaining);
    }
  }
  return NULL;
}

static void osal_alarm_fired(void)
{
  uint32_t min;
  uint8_t i;

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

static void osal_update_timer() {
  uint32_t now;
  struct timeval tv = {0};
  uint8_t i;
  bool flag = false;

  m_remaining = (1UL << 31) - 1; /* max int32_t */
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
      flag = true;
    }
  }
  if(flag)
    osal_sem_post(&sem);
}
void osal_trickle_timer_start(osal_timerid_t timerid, uint32_t imin, uint32_t imax, trickle_timer_fired_t trickle_timer_fired)
{
  uint32_t min;
  struct timeval tv = {0};
  uint32_t seed = 0;
  osal_sigset_t set;
  osal_basetype_t ret = 0;

  if(!m_timert_isrunning) {
    osal_sigemptyset(&set);
    osal_sigaddset(&set, SIGALRM);
    osal_sigprocmask(SIG_BLOCK, &set, NULL);

    osal_sem_create(&sem, 0);
    osal_task_create(&timer_id_task, NULL, 0, 0, osal_timer_thread, NULL);
    ret = osal_task_create(&timer_id_task, NULL, 0, 0, osal_timer_thread, NULL);
    assert(ret == OSAL_SUCCESS);
    
    m_timert_isrunning = true;
  }

  if(timerid == reg_timer) {
    DPRINTF("register trickle timer start\n");
  }
  else if(timerid == rpt_timer) {
    DPRINTF("metrics report trickle timer start\n");
  }

  osal_gettime(&tv, NULL);

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
  osal_update_timer();
}
void osal_trickle_timer_stop(osal_timerid_t timerid)
{
  uint8_t i;
  osal_sigset_t set;

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

  osal_task_cancel(timer_id_task);
  osal_sem_destroy(&sem);

  osal_sigemptyset(&set);
  osal_sigaddset(&set, SIGALRM);
  osal_task_sigmask(SIG_UNBLOCK, &set, NULL);
  m_timert_isrunning = false;
}

void *osal_malloc(size_t size)
{
  return malloc(size);
}

void osal_free(void *ptr)
{
  free(ptr);
}