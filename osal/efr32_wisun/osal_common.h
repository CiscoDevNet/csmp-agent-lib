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
#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "socket/socket.h"
#include "sl_malloc.h"
#include "sl_sleeptimer.h"

typedef void (*osal_sighandler_t)(int);

typedef struct sockaddr_in6 osal_sockaddr;

typedef SemaphoreHandle_t osal_sem;
typedef socklen_t osal_socklen;
typedef TaskHandle_t osal_task_t;
typedef TaskFunction_t osal_task_fnc_t;
typedef ssize_t osal_ssize_t;
typedef uint64_t osal_time_t;
typedef BaseType_t osal_basetype_t;
typedef ssize_t osal_ssize_t;
typedef int osal_socket_handle_t;
#define OSAL_AF_INET6 AF_INET6
#define OSAL_SOCK_DGRAM SOCK_DGRAM

void osal_kernel_start(void);

#define s6_addr address

osal_basetype_t osal_task_create(
   osal_task_t * thread,
   const char * name,
   uint32_t priority,
   size_t stacksize,
   osal_task_fnc_t entry,
   void * arg);
osal_basetype_t osal_task_cancel(osal_task_t thread);
osal_basetype_t osal_task_setcanceltype(void);
osal_basetype_t osal_sem_create(osal_sem * sem, uint16_t value);
osal_basetype_t osal_sem_post(osal_sem * sem);
osal_basetype_t osal_sem_wait(osal_sem * sem, osal_time_t timeout);
osal_basetype_t osal_sem_destroy(osal_sem *sem);
osal_socket_handle_t osal_socket(osal_basetype_t domain, osal_basetype_t type, osal_basetype_t protocol);
osal_ssize_t osal_recvfrom(int sockfd, void *buf, size_t len, int flags,
                  osal_sockaddr *src_addr, osal_socklen *addrlen);
ssize_t osal_sendmsg(int sockfd, const struct msghdr msg, int flags);
osal_basetype_t osal_bind(int osal_sockfd, osal_sockaddr *osal_addr, 
	                       osal_socklen addrlen);

ssize_t osal_sendto(int sockfd, const void *buf, size_t len, int flags,
                      const osal_sockaddr *dest_addr, osal_socklen addrlen);
osal_basetype_t osal_inet_pton(int af, const char *src, void *dst);
#if 1

typedef struct {
   uint32_t fd_bits[1];
} fd_set;

osal_basetype_t osal_select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
void osal_fd_zero(fd_set *set);
void osal_fd_set(int fd, fd_set *set);
osal_basetype_t osal_fd_isset(int fd, fd_set *set);
#endif

osal_basetype_t osal_gettimeofday(struct timeval *tv, struct timezone *tz);
osal_basetype_t osal_settime(struct timeval *tv, struct timezone *tz);

typedef enum {
 reg_timer = 0,  /**< register timer */
 rpt_timer = 1,  /**< reporting timer */
 timer_num = 2   /**< max amount of timers */
}timerid_t;

typedef void (*trickle_timer_fired_t) ();
void osal_trickle_timer_start(timerid_t timerid, uint32_t imin, uint32_t imax, trickle_timer_fired_t trickle_time_fired);
void osal_trickle_timer_stop(timerid_t timerid);

void osal_print_formatted_ip(const osal_sockaddr *sockAdd);
void osal_update_sockaddr(osal_sockaddr *listen_addr, uint16_t sport);

void *osal_malloc(size_t size);
void *osal_calloc(size_t num, size_t size);
void *osal_realloc(void *ptr, size_t size);
void osal_free(void *ptr); 

void osal_sleep_ms(uint64_t ms);
#endif
