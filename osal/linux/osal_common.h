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

#ifndef __OSAL_H
#define __OSAL_H
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <stdbool.h>
#if defined(__APPLE__)
   #include <dispatch/dispatch.h>
#else
   #include <semaphore.h>
#endif
#include "../../src/lib/debug.h"

typedef void (*osal_sighandler_t)(int);

typedef struct sockaddr_in6 osal_sockaddr;
#if defined(__APPLE__)
typedef dispatch_semaphore_t osal_sem;
#else
typedef sem_t osal_sem;
#endif
typedef socklen_t osal_socklen;
typedef pthread_t osal_task_t;
typedef uint64_t osal_time_t;
typedef ssize_t osal_ssize_t;
typedef int osal_basetype_t;
typedef int osal_socket_handle_t;
#define OSAL_AF_INET6 AF_INET6 
#define OSAL_SOCK_DGRAM SOCK_DGRAM 
void osal_print_formatted_ip(const osal_sockaddr *sockAdd);
osal_task_t osal_task_create(
   const char * name,
   uint32_t priority,
   size_t stacksize,
   void* (*entry) (void * arg),
   void * arg);
int osal_task_cancel(osal_task_t thread);
int osal_task_setcanceltype(int type, int *oldtype);
int osal_task_sigmask(int how, const sigset_t *set, sigset_t *oldset);
int osal_sem_create (osal_sem * sem, uint16_t value);
int osal_sem_post (osal_sem * sem);
int osal_sem_wait (osal_sem * sem, osal_time_t timeout);
int osal_sem_destroy(osal_sem *sem);
osal_socket_handle_t osal_socket(osal_basetype_t domain, osal_basetype_t type, osal_basetype_t protocol);
osal_basetype_t osal_recvfrom(osal_socket_handle_t sockfd, void *buf, size_t len, osal_basetype_t flags,
                        osal_sockaddr *src_addr, osal_socklen *addrlen);
osal_ssize_t osal_sendmsg(int sockfd, const struct msghdr msg, int flags);
int osal_bind(int osal_sockfd, osal_sockaddr *osal_addr, 
	osal_socklen addrlen);

osal_ssize_t osal_sendto(osal_basetype_t sockfd, const void *buf, size_t len, int flags,
                      const osal_sockaddr *dest_addr, osal_socklen addrlen);
int osal_inet_pton(int af, const char *src, void *dst);
int osal_select(int nfds, fd_set *readfds, fd_set *writefds,
              fd_set *exceptfds, struct timeval *timeout);
void osal_fd_zero(fd_set *set);
void osal_fd_set(int fd, fd_set *set);
int osal_fd_isset(int fd, fd_set *set);

void osal_update_sockaddr(osal_sockaddr *listen_addr, uint16_t sport);

int osal_gettimeofday(struct timeval *tv, struct timezone *tz);
int osal_settime(struct timeval *tv, struct timezone *tz);
osal_sighandler_t osal_signal(int signum, osal_sighandler_t handler);
int osal_sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int osal_sigemptyset(sigset_t *set);
int osal_sigaddset(sigset_t *set, int signum);

/* 
 * trickle_timer defines
 */
typedef enum {
 reg_timer = 0,  /**< register timer */
 rpt_timer = 1,  /**< reporting timer */
 timer_num = 2   /**< max amount of timers */
}timerid_t;

typedef void (*trickle_timer_fired_t) ();
void osal_trickle_timer_start(timerid_t timerid, uint32_t imin, uint32_t imax, trickle_timer_fired_t trickle_time_fired);
void osal_trickle_timer_stop(timerid_t timerid);

#endif
