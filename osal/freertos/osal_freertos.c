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

void osal_kernel_start(void)
{
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

    return ret == pdPASS ? 0 : -1;
}

osal_basetype_t osal_task_cancel(osal_task_t thread)
{
    (void) thread;
    return (0);
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
    (void) sem;
    (void) value;
    return (0);
}

//sem_post
osal_basetype_t osal_sem_post(osal_sem_t * sem)
{
    (void) sem;
    return (0);
}

//sem_wait
osal_basetype_t osal_sem_wait(osal_sem_t * sem, osal_time_t timeout)
{
    (void) sem;
    (void) timeout;
    return (0);
}

//sem_destroy
osal_basetype_t osal_sem_destroy(osal_sem_t *sem)
{
    (void) sem;
    return (0);
}
//Socket
osal_socket_handle_t osal_socket(osal_basetype_t domain, osal_basetype_t type, osal_basetype_t protocol)
{
    (void) domain;
    (void) type;
    (void) protocol;
    return (0);
}

//bind
osal_basetype_t osal_bind(osal_socket_handle_t sockd, osal_sockaddr_t *addr, 
                            osal_socklen_t addrlen)
{
    (void) osal_sockfd;
    (void) osal_addr;
    (void) addrlen;
    return (0);
}

//recvfrom
osal_ssize_t osal_recvfrom(int sockd, void *buf, size_t len, int flags,
                    osal_sockaddr_t *src_addr, osal_socklen_t *addrlen)
{
    (void) sockfd;
    (void) buf;
    (void) len;
    (void) flags;
    (void) src_addr;
    (void) addrlen;
    return (0);
}

//sendmsg
ssize_t osal_sendmsg(int sockd, const struct msghdr msg, int flags){
    (void) sockfd;
    (void) msg;
    (void) flags;
    return (0);
}

//sendto
ssize_t osal_sendto(int sockd, const void *buf, size_t len, int flags,
                      const osal_sockaddr_t *dest_addr, osal_socklen_t addrlen)
{
    (void) sockfd;
    (void) buf;
    (void) len;
    (void) flags;
    (void) dest_addr;
    (void) addrlen;
    return (0);
}

//inet_pton

osal_basetype_t osal_inet_pton(int af, const char *src, void *dst)
{
    (void) af;
    (void) src;
    (void) dst;
    return (0);
}

//select
osal_basetype_t osal_select(int nsds, osal_sd_set_t *readsds, osal_sd_set_t *writesds,
                  osal_sd_set_t *exceptsds, struct timeval *timeout)
{
    (void) nfds;
    (void) readfds;
    (void) writefds;
    (void) exceptfds;
    (void) timeout;
    return (0);
}

void osal_sd_zero(osal_sd_set_t *set)
{
    (void) set;
}

void osal_sd_set(int sd, osal_sd_set_t *set)
{
    (void) fd;
    (void) set;
}

osal_basetype_t osal_sd_isset(int sd, osal_sd_set_t *set)
{
    (void) fd;
    (void) set;
    return (0);
}

//gettimeofday
osal_basetype_t osal_gettime(struct timeval *tv, struct timezone *tz)
{
    (void) tv;
    (void) tz;
    return (0);
}

//settime
osal_basetype_t osal_settime(struct timeval *tv, struct timezone *tz)
{
    (void) tv;
    (void) tz;
    return (0);
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

void osal_trickle_timer_start(timerid_t timerid, uint32_t imin, uint32_t imax, trickle_timer_fired_t trickle_time_fired)
{
    (void) timerid;
    (void) imin;
    (void) imax;
    (void) trickle_time_fired;

}
void osal_trickle_timer_stop(timerid_t timerid)
{
    (void) timerid;
}

void osal_print_formatted_ip(const osal_sockaddr *sockAdd)
{
    (void) sockAdd;
}
void osal_update_sockaddr(osal_sockaddr *listen_addr, uint16_t sport)
{
    (void) listen_addr;
    (void) sport;
}

void *osal_malloc(size_t size)
{
    (void) size;
    return (NULL);
}
void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    char * pcTaskName )
{
    ( void ) xTask;
    ( void ) pcTaskName;
    printf("Stack overflow: %s\n", pcTaskName);
    for( ;; );
}