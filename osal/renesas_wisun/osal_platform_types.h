/*
 *  Copyright 2026 Renesas Electronics Corporation
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

#include <assert.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "r_byte_swap.h"
#include "r_header_utils.h"
#if defined(__i386) || defined(__x86_64)

/* The Wi-SUN FAN simulator requires Unix sockets for basic functionality -> avoid redefinition */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#else

/* Supported address families. */
#define AF_INET6 10  /* IP version 6 */

/* Type to represent a port.  */
typedef uint16_t in_port_t;

typedef uint16_t sa_family_t;

typedef uint32_t socklen_t;

struct in6_addr
{
	unsigned char s6_addr[16];  /* IPv6 address */
};

/* Structure describing a generic socket address.  */
struct sockaddr
{
	sa_family_t sa_family;   /* Common data: address family and length.  */
	char        sa_data[14]; /* Address data.  */
};

struct sockaddr_in6
{
	sa_family_t     sin6_family;   /* AF_INET6 */
	in_port_t       sin6_port;     /* port number */
	uint32_t        sin6_flowinfo; /* IPv6 flow information */
	struct in6_addr sin6_addr;     /* IPv6 address */
	uint32_t        sin6_scope_id; /* Scope ID (new in 2.4) */
};

/* Structure for scatter/gather I/O.  */
struct iovec
{
	void*  iov_base; /* Pointer to data.  */
	size_t iov_len;  /* Length of data.  */
};

/* Structure describing messages sent by
   `sendmsg' and received by `recvmsg'.  */
struct msghdr
{
	void*         msg_name;       /* Address to send to/receive from.  */
	socklen_t     msg_namelen;    /* Length of address data.  */

	struct iovec* msg_iov;        /* Vector of data to send/receive into.  */
	size_t        msg_iovlen;     /* Number of elements in the vector.  */

	void*         msg_control;    /* Ancillary data (eg BSD filedesc passing). */
	size_t        msg_controllen; /* Ancillary data buffer length.
				   !! The type should be socklen_t but the
				   definition of the kernel is incompatible
				   with this.  */

	int msg_flags;                /* Flags on received message.  */
};

#endif

typedef void (*osal_sighandler_t)(int);

typedef struct sockaddr_in6 osal_sockaddr_t;
typedef SemaphoreHandle_t osal_sem_t;
typedef int osal_sigset_t;
typedef socklen_t osal_socklen_t;
typedef TaskHandle_t osal_task_t;
typedef TaskFunction_t osal_task_fnc_t;
typedef int32_t osal_ssize_t;
typedef uint64_t osal_time_t;
typedef BaseType_t osal_basetype_t;
typedef int osal_socket_handle_t;
typedef int osal_sd_set_t;

#define OSAL_AF_INET6 AF_INET6
#define OSAL_SOCK_DGRAM 0

#define OSAL_ATTR_PACKED R_HEADER_UTILS_ATTR_PACKED

#define CSMP_MAX_SOCKETS 2

#if !defined(__i386) && !defined(__x86_64)
struct timeval
{
    uint32_t tv_sec;  /* Seconds.  */
    uint32_t tv_usec; /* Microseconds.  */
};
#endif

struct timezone {
    int tz_minuteswest; /* minutes west of Greenwich */
    int tz_dsttime;     /* type of DST correction */
};

#endif