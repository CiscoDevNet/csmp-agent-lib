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

osal_task_t osal_task_create (
   const char * name,
   uint32_t priority,
   size_t stacksize,
   void *(*entry) (void * arg),
   void * arg)
{
    /* FreeRtos implementation */
}

int osal_task_cancel(osal_task_t thread)
{
    /* FreeRtos implementation */
}

int osal_task_setcanceltype(int type, int *oldtype)
{
    /* FreeRtos implementation */
}

int osal_task_sigmask(int how, const sigset_t *set, sigset_t *oldset)
{
    /* FreeRtos implementation */
}

//sem_init
int osal_sem_create (osal_sem * sem, uint16_t value)
{
    /* FreeRtos implementation */
}

//sem_post
int osal_sem_post (osal_sem * sem)
{
    /* FreeRtos implementation */
}

//sem_wait
int osal_sem_wait (osal_sem * sem, osal_time_t timeout)
{
    /* FreeRtos implementation */
}

//sem_destroy
int osal_sem_destroy(osal_sem *sem)
{
    /* FreeRtos implementation */
}
//Socket
osal_socket_handle_t osal_socket(osal_basetype_t domain, osal_basetype_t type, osal_basetype_t protocol)
{
    return(FreeRTOS_socket(domain, type, protocol));
}

//bind
osal_basetype_t osal_bind(osal_socket_handle_t osal_sockfd, osal_sockaddr *osal_addr, 
                            osal_socklen addrlen)
{
    return (FreeRTOS_bind(osal_sockfd, (struct freertos_sockaddr *)(osal_addr), addrlen));
}

//recvfrom
int osal_recvfrom(int sockfd, void *buf, size_t len, int flags,
                    osal_sockaddr *src_addr, osal_socklen *addrlen)
{
    /* FreeRtos implementation */

}
//sendmsg
ssize_t osal_sendmsg(int sockfd, const struct msghdr msg, int flags){
    /* FreeRtos implementation */
}

//sendto
ssize_t osal_sendto(int sockfd, const void *buf, size_t len, int flags,
                      const osal_sockaddr *dest_addr, osal_socklen addrlen)
{
    /* FreeRtos implementation */
}

//inet_pton

int osal_inet_pton(int af, const char *src, void *dst)
{
    /* FreeRtos implementation */
}

//select
int osal_select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout)
{
    /* FreeRtos implementation */
}

void osal_fd_zero(fd_set *set)
{
    /* FreeRtos implementation */
}

void osal_fd_set(int fd, fd_set *set)
{
    /* FreeRtos implementation */
}

int osal_fd_isset(int fd, fd_set *set)
{
    /* FreeRtos implementation */
}

//gettimeofday
int osal_gettimeofday(struct timeval *tv, struct timezone *tz)
{
    /* FreeRtos implementation */
}

//settime
int osal_settime(struct timeval *tv, struct timezone *tz)
{
    /* FreeRtos implementation */
}

osal_sighandler_t osal_signal(int signum, osal_sighandler_t handler)
{
    /* FreeRtos implementation */
}

int osal_sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    /* FreeRtos implementation */
}

int osal_sigemptyset(sigset_t *set)
{
    /* FreeRtos implementation */
}

int osal_sigaddset(sigset_t *set, int signum)
{
    /* FreeRtos implementation */
}

void *osal_malloc(size_t size)
{
    /* FreeRtos implementation */
}

void *osal_calloc(size_t num, size_t size)
{
    /* FreeRtos implementation */
}

void *osal_realloc(void *ptr, size_t size)
{
    /* FreeRtos implementation */
}

void osal_free(void *ptr)
{
    /* FreeRtos implementation */
}
