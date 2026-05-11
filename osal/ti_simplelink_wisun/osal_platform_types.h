#ifndef __OSAL_PLATFORM_TYPES_H
#define __OSAL_PLATFORM_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

/* struct timeval — from TI POSIX shim */
#include <sys/time.h>

/* -----------------------------------------------------------------------
 * FreeRTOS — from TI SimpleLink SDK.
 * --------------------------------------------------------------------- */
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
#define DPRINTF(...) ns_trace_printf(TRACE_LEVEL_DEBUG, "csmp", __VA_ARGS__)
#endif

/* -----------------------------------------------------------------------
 * TI POSIX shim only provides __socklen_t; define directly.
 * --------------------------------------------------------------------- */
typedef uint32_t socklen_t;
typedef int32_t  ssize_t;

/* -----------------------------------------------------------------------
 * Socket constants and macros
 * --------------------------------------------------------------------- */
#ifndef AF_INET6
#define AF_INET6        28
#endif
#define OSAL_AF_INET6   AF_INET6
#define OSAL_SOCK_DGRAM 2

#define htons(x)  ((uint16_t)(__builtin_bswap16((uint16_t)(x))))
#define ntohs(x)  ((uint16_t)(__builtin_bswap16((uint16_t)(x))))

/* -----------------------------------------------------------------------
 * POSIX socket structs — not provided by TI SDK, defined here.
 * Nanostack uses ns_address_t internally; these exist only at the
 * OSAL boundary that csmp-agent-lib sees.
 * --------------------------------------------------------------------- */
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

/* -----------------------------------------------------------------------
 * OSAL type definitions
 * --------------------------------------------------------------------- */
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

/* -----------------------------------------------------------------------
 * TI CC13xx flash layout for CSMP OTA firmware management.
 * These are shared by osal_ti_simplelink_wisun.c and ti_simplelink_wisun_tlvs.c.
 * Defaults can be overridden at compile time with -D flags.
 * ----------------------------------------------------------------------- */
#ifndef CSMP_TI_RUN_SLOT_ADDR
#define CSMP_TI_RUN_SLOT_ADDR       0x00000000UL
#endif
#ifndef CSMP_TI_UPLOAD_SLOT_ADDR
#define CSMP_TI_UPLOAD_SLOT_ADDR    0x0005E000UL
#endif
#ifndef CSMP_TI_BACKUP_SLOT_ADDR
#define CSMP_TI_BACKUP_SLOT_ADDR    0x00088000UL
#endif
#ifndef CSMP_TI_SLOT_MAX_SIZE
#define CSMP_TI_SLOT_MAX_SIZE       (328UL * 1024UL)
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

/* -----------------------------------------------------------------------
 * External SPI flash (Off-Chip OAD secondary slot).
 *
 * TI's MCUBoot uses VERSION COMPARISON (ih_ver field) to decide whether to
 * install the secondary slot — no trailer magic bytes are needed or written.
 * Simply reboot after the download is complete; MCUBoot checks the version
 * on every boot and installs if secondary.ih_ver > primary.ih_ver.
 *
 * The secondary slot lives entirely in external SPI flash (e.g. W25Q32FV).
 * CSMP_TI_EXT_FLASH_SECTOR_SIZE: minimum erase granularity for W25Q32 = 4 KB.
 * ----------------------------------------------------------------------- */
#define CSMP_TI_EXT_FLASH_SECTOR_SIZE   0x1000UL    /* 4 KB erase unit on MX25R8035F */

/* Forward declaration — defined in osal_ti_simplelink_wisun.c, called from ti_simplelink_wisun_tlvs.c */
osal_basetype_t ti_flash_erase_slot(uint8_t slotid);

#endif /* __OSAL_PLATFORM_TYPES_H */
