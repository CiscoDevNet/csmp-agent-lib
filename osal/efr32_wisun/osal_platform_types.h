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

#ifndef __OSAL_PLATFORM_TYPES_H
#define __OSAL_PLATFORM_TYPES_H
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
#include "sl_memory_manager.h"
#include "sl_sleeptimer.h"
#include "sl_wisun_ntp_timesync_config.h"

typedef void (*osal_sighandler_t)(int);

typedef struct sockaddr_in6 osal_sockaddr_t;
typedef SemaphoreHandle_t osal_sem_t;
typedef int osal_sigset_t;
typedef socklen_t osal_socklen_t;
typedef TaskHandle_t osal_task_t;
typedef TaskFunction_t osal_task_fnc_t;
typedef ssize_t osal_ssize_t;
typedef uint64_t osal_time_t;
typedef BaseType_t osal_basetype_t;
typedef int osal_socket_handle_t;
typedef int osal_sd_set_t;

#define OSAL_AF_INET6 AF_INET6
#define OSAL_SOCK_DGRAM SOCK_DGRAM

#define s6_addr address


#define CSMP_IMAGE_HDR_SIZE 256

#endif
