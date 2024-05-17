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

osal_basetype_t osal_task_create (
   osal_task_t * thread,
   const char * name,
   uint32_t priority,
   size_t stacksize,
   void *(*entry) (void * arg),
   void * arg)
{
    /* FreeRtos implementation */
}

osal_basetype_t osal_task_cancel(osal_task_t thread)
{
    /* FreeRtos implementation */
}

osal_basetype_t osal_task_setcanceltype(int type, int *oldtype)
{
    /* FreeRtos implementation */
}

osal_basetype_t osal_task_sigmask(int how, const osal_sigset_t *set, osal_sigset_t *oldset)
{
    /* FreeRtos implementation */
}

//sem_init
osal_basetype_t osal_sem_create(osal_sem_t * sem, uint16_t value)
{
    /* FreeRtos implementation */
}

//sem_post
osal_basetype_t osal_sem_post(osal_sem_t * sem)
{
    /* FreeRtos implementation */
}

//sem_wait
osal_basetype_t osal_sem_wait(osal_sem_t * sem, osal_time_t timeout)
{
    /* FreeRtos implementation */
}

//sem_destroy
osal_basetype_t osal_sem_destroy(osal_sem_t *sem)
{
    /* FreeRtos implementation */
}
//Socket
osal_socket_handle_t osal_socket(osal_basetype_t domain, osal_basetype_t type, osal_basetype_t protocol)
{
    return(FreeRTOS_socket(domain, type, protocol));
}

//bind
osal_basetype_t osal_bind(osal_socket_handle_t sockd, osal_sockaddr_t *addr, 
                            osal_socklen_t addrlen)
{
    return (FreeRTOS_bind(sockd, (struct freertos_sockaddr *)(addr), addrlen));
}

//recvfrom
osal_ssize_t osal_recvfrom(int sockd, void *buf, size_t len, int flags,
                    osal_sockaddr_t *src_addr, osal_socklen_t *addrlen)
{
    /* FreeRtos implementation */

}
//sendmsg
ssize_t osal_sendmsg(int sockd, const struct msghdr msg, int flags){
    /* FreeRtos implementation */
}

//sendto
ssize_t osal_sendto(int sockd, const void *buf, size_t len, int flags,
                      const osal_sockaddr_t *dest_addr, osal_socklen_t addrlen)
{
    /* FreeRtos implementation */
}

//inet_pton

osal_basetype_t osal_inet_pton(int af, const char *src, void *dst)
{
    /* FreeRtos implementation */
}

//select
osal_basetype_t osal_select(int nsds, osal_sd_set_t *readsds, osal_sd_set_t *writesds,
                  osal_sd_set_t *exceptsds, struct timeval *timeout)
{
    /* FreeRtos implementation */
}

void osal_sd_zero(osal_sd_set_t *set)
{
    /* FreeRtos implementation */
}

void osal_sd_set(int sd, osal_sd_set_t *set)
{
    /* FreeRtos implementation */
}

osal_basetype_t osal_sd_isset(int sd, osal_sd_set_t *set)
{
    /* FreeRtos implementation */
}

//gettimeofday
osal_basetype_t osal_gettime(struct timeval *tv, struct timezone *tz)
{
    /* FreeRtos implementation */
}

//settime
osal_basetype_t osal_settime(struct timeval *tv, struct timezone *tz)
{
    /* FreeRtos implementation */
}

osal_sighandler_t osal_signal(int signum, osal_sighandler_t handler)
{
    /* FreeRtos implementation */
}

osal_basetype_t osal_sigprocmask(int how, const osal_sigset_t *set, osal_sigset_t *oldset)
{
    /* FreeRtos implementation */
}

osal_basetype_t osal_sigemptyset(osal_sigset_t *set)
{
    /* FreeRtos implementation */
}

osal_basetype_t osal_sigaddset(osal_sigset_t *set, int signum)
{
    /* FreeRtos implementation */
}

void *osal_malloc(size_t size)
{
    /* FreeRtos implementation */
}

void osal_free(void *ptr)
{
    /* FreeRtos implementation */
}
