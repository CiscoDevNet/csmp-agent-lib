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
#include <signal.h>
#include <limits.h>

#include <assert.h>
#include <errno.h>
#include <unistd.h>

#include <sys/syscall.h>
#include <arpa/inet.h>

#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

typedef struct sockaddr_in6 osal_sockaddr;
typedef sem_t osal_sem;
typedef socklen_t osal_socklen;
typedef pthread_t osal_task_t;
typedef uint64_t osal_time_t;
typedef long osal_basetype_t;
typedef ssize_t osal_ssize_t;
typedef int osal_socket_handle_t;
typedef void (*osal_sighandler_t)(int);
typedef fd_set osal_sd_set_t;

#define OSAL_AF_INET6 AF_INET6 
#define OSAL_SOCK_DGRAM SOCK_DGRAM

#endif
