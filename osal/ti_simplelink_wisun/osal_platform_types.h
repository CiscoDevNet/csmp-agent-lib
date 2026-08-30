#ifndef __OSAL_PLATFORM_TYPES_H
#define __OSAL_PLATFORM_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

/* struct timeval — from TI POSIX shim */
#include <sys/time.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"

/* Include debug.h first so its DPRINTF definition comes before any override */
#include "../../src/lib/debug.h"

#if defined(CSMP_USE_MBED_TRACE)
#ifndef TRACE_LEVEL_DEBUG
#define TRACE_LEVEL_DEBUG  4
#endif
void ns_trace_printf(uint8_t dlevel, const char *grp, const char *fmt, ...);
#undef  DPRINTF
/* Route CSMP debug output through mbed-trace when CSMP_USE_MBED_TRACE is set */
#define DPRINTF(...) ns_trace_printf(TRACE_LEVEL_DEBUG, "csmp", __VA_ARGS__)
#endif

extern int close(int fd);

typedef uint32_t socklen_t;
typedef int32_t  ssize_t;

#ifndef AF_INET6
#define AF_INET6        28
#endif
#define OSAL_AF_INET6   AF_INET6
#define OSAL_SOCK_DGRAM 2

/* Convert 16-bit value from host to network byte order */
#define htons(x)  ((uint16_t)(__builtin_bswap16((uint16_t)(x))))
/* Convert 16-bit value from network to host byte order */
#define ntohs(x)  ((uint16_t)(__builtin_bswap16((uint16_t)(x))))

struct iovec {
    void  *iov_base;
    size_t iov_len;
};

struct msghdr {
    void          *msg_name;
    socklen_t      msg_namelen;
    struct iovec  *msg_iov;
    int            msg_iovlen;
    void          *msg_control;
    socklen_t      msg_controllen;
    int            msg_flags;
};

struct in6_addr {
    uint8_t s6_addr[16];
};

struct sockaddr_in6 {
    uint16_t        sin6_family;
    uint16_t        sin6_port;
    uint32_t        sin6_flowinfo;
    struct in6_addr sin6_addr;
    uint32_t        sin6_scope_id;
};

/* struct timezone — not provided by TI POSIX shim */
#ifndef _STRUCT_TIMEZONE
#define _STRUCT_TIMEZONE
struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};
#endif

typedef void                  (*osal_sighandler_t)(int);
typedef struct sockaddr_in6     osal_sockaddr_t;
typedef SemaphoreHandle_t       osal_sem_t;
typedef int                     osal_sigset_t;
typedef socklen_t               osal_socklen_t;
typedef TaskHandle_t            osal_task_t;
typedef TaskFunction_t          osal_task_fnc_t;
typedef ssize_t                 osal_ssize_t;
typedef uint64_t                osal_time_t;
typedef BaseType_t              osal_basetype_t;
typedef int                     osal_socket_handle_t;
typedef struct { int _u; }      osal_sd_set_t;

#ifndef CSMP_TI_RUN_SLOT_ADDR
#define CSMP_TI_RUN_SLOT_ADDR       0x00000000UL
#endif
#ifndef CSMP_TI_UPLOAD_SLOT_ADDR
#define CSMP_TI_UPLOAD_SLOT_ADDR    0x00000000UL
#endif
#ifndef CSMP_TI_BACKUP_SLOT_ADDR
#define CSMP_TI_BACKUP_SLOT_ADDR    0x00000000UL
#endif
#ifndef CSMP_TI_SLOT_MAX_SIZE
#define CSMP_TI_SLOT_MAX_SIZE       0x000A8000UL
#endif
#ifndef CSMP_TI_FLASH_SECTOR_SIZE
#define CSMP_TI_FLASH_SECTOR_SIZE   0x1000UL
#endif
#ifndef CSMP_TI_NV_SYSID
#define CSMP_TI_NV_SYSID            7u
#endif
#ifndef CSMP_TI_NV_SLOTHDR_ITEM
#define CSMP_TI_NV_SLOTHDR_ITEM     0x01u
#endif

#define CSMP_TI_EXT_FLASH_SECTOR_SIZE   0x1000UL    /* 4 KB erase unit on MX25R8035F */

/* Forward declaration — defined in osal_ti_simplelink_wisun.c, called from ti_simplelink_wisun_tlvs.c */
osal_basetype_t ti_flash_erase_slot(uint8_t slotid);

#endif /* __OSAL_PLATFORM_TYPES_H */
