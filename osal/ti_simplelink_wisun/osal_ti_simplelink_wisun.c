
#include "osal.h"
#include <string.h>
#include <stdlib.h>
#include "csmpservice.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"

#include <ti/drivers/dpl/ClockP.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/sys_ctrl.h)
#include <ti_drivers_config.h>   /* CONFIG_NVSEXTERNAL */
#include <ti/drivers/NVS.h>      /* Off-Chip OAD secondary slot via NVSSPI25X */
#include "socket_api.h"
#include "ns_address.h"
#include "ip6string.h"
#include "mesh_system.h"
#include "net_interface.h"

#include "nvocmp.h"
#include "nvintf.h"

struct trickle_timer
{
    uint32_t t0, tfire, icur, imin, imax;
    uint8_t is_running : 1;
    TimerHandle_t timer;
};

#define __ret_freertos2posix(r) ((r) == pdPASS ? OSAL_SUCCESS : OSAL_FAILURE)

extern uint8_t g_csmplib_eui64[8];
extern NVINTF_nvFuncts_t *pNV;

static struct trickle_timer timers[timer_num];
static trickle_timer_fired_t timer_fired[timer_num];
static int32_t m_remaining = ((1UL << 31) - 1) / 1000;
static bool m_timert_isrunning = false;

static void osal_update_timer(void);
static void osal_alarm_fired(TimerHandle_t xTimer);
static void osal_alarm_fired_pend_fnc(void *p1, uint32_t p2);

/* Time offset (int64_t to survive past 2038) */
int64_t g_ti_csmp_time_offset_sec = 0;

#define CSMP_TI_RX_BUF_LEN   1280u
#define CSMP_TI_CH_SERVER     0          /* CON/NON — server requests       */
#define CSMP_TI_CH_CLIENT     1          /* ACK/RST  — client responses     */
#define CSMP_TI_CH_COUNT      2
/* Socket handles: server uses the real nanostack fd; client uses fd+64 as a
 * synthetic handle so osal_recvfrom can distinguish them via sockd>=64. */
#define CSMP_TI_CLIENT_FD_OFFSET  64

static SemaphoreHandle_t g_rx_sem[CSMP_TI_CH_COUNT];
static int8_t            g_ns_sock = -1;
static uint8_t           g_rx_buf[CSMP_TI_CH_COUNT][CSMP_TI_RX_BUF_LEN];
static osal_sockaddr_t   g_rx_src[CSMP_TI_CH_COUNT];
static int32_t           g_rx_len[CSMP_TI_CH_COUNT];
/* Track how many times osal_socket() has been called to assign channels */
static int g_sock_alloc_count = 0;

/* Flash / NV constants are now in osal_platform_types.h */

/* ------------------------------------------------------------------ */
/* Initialize trickle timers and per-channel receive semaphores before the RTOS scheduler runs */
void osal_kernel_start(void)
{
    for (BaseType_t i = 0; i < (BaseType_t)timer_num; i++)
    {
        timers[i].is_running = false;
        timers[i].timer = xTimerCreate("csmp_tri",
                                       pdMS_TO_TICKS(m_remaining * 1000),
                                       pdTRUE, (void *)(uintptr_t)i,
                                       osal_alarm_fired);
        assert(timers[i].timer != NULL);
        xTimerStop(timers[i].timer, 0);
    }
    for (int ch = 0; ch < CSMP_TI_CH_COUNT; ch++)
    {
        g_rx_sem[ch] = xSemaphoreCreateCounting(8u, 0u);
        assert(g_rx_sem[ch] != NULL);
    }
}

/* Create a FreeRTOS task wrapping the CSMP service thread entry point */
osal_basetype_t osal_task_create(osal_task_t *thread, const char *name,
                                 uint32_t priority, size_t stacksize,
                                 osal_task_fnc_t entry, void *arg)
{
    /* 1536 words (6KB) base stack. */
    uint16_t words = (uint16_t)(1536u + stacksize / sizeof(StackType_t));
    osal_basetype_t r = xTaskCreate(entry, name, words, arg,
                                    (UBaseType_t)(tskIDLE_PRIORITY + 1u + priority),
                                    (TaskHandle_t *)thread);
    DPRINTF("task '%s' %s\n", name, r == pdPASS ? "ok" : "failed");
    return __ret_freertos2posix(r);
}

/* Delete a running CSMP task by handle */
osal_basetype_t osal_task_cancel(osal_task_t t)
{
    vTaskDelete((TaskHandle_t)t);
    return OSAL_SUCCESS;
}

/* No-op stub: FreeRTOS tasks do not support POSIX cancel types */
osal_basetype_t osal_task_setcanceltype(void)
{
    return OSAL_SUCCESS;
}

/* No-op stub: signal masks are not supported on this embedded target */
osal_basetype_t osal_task_sigmask(osal_basetype_t h, const osal_sigset_t *s,
                                  osal_sigset_t *o)
{
    (void)h;
    (void)s;
    (void)o;
    return OSAL_FAILURE;
}

/* Create a FreeRTOS counting semaphore with an initial value */
osal_basetype_t osal_sem_create(osal_sem_t *s, uint16_t v)
{
    if (!s)
    {
        return OSAL_FAILURE;
    }
    *s = xSemaphoreCreateCounting(0xFFFFu, (UBaseType_t)v);
    return *s ? OSAL_SUCCESS : OSAL_FAILURE;
}

/* Signal a semaphore from task context */
osal_basetype_t osal_sem_post(osal_sem_t *s)
{
    return s ? __ret_freertos2posix(xSemaphoreGive(*s)) : OSAL_FAILURE;
}

/* Block on a semaphore with a FreeRTOS tick timeout */
osal_basetype_t osal_sem_wait(osal_sem_t *s, osal_time_t t)
{
    return s ? __ret_freertos2posix(xSemaphoreTake(*s, (TickType_t)t)) : OSAL_FAILURE;
}

/* Delete a semaphore and release its memory */
osal_basetype_t osal_sem_destroy(osal_sem_t *s)
{
    if (s)
    {
        vSemaphoreDelete(*s);
    }
    return OSAL_SUCCESS;
}

/* Nanostack SOCKET_DATA callback: demux incoming CoAP packets to server or client channel */
static void csmp_ns_socket_cb(void *cb_ptr)
{
    socket_callback_t *cb = (socket_callback_t *)cb_ptr;
    ns_address_t src = {0};
    if (cb->event_type != SOCKET_DATA)
    {
        /* Log MAC-level TX feedback to diagnose frame size issues */
        switch (cb->event_type) {
            case SOCKET_TX_DONE:
                DPRINTF("csmp_sock: TX_DONE %u bytes OK\n", (unsigned)cb->d_len);
                break;
            case SOCKET_TX_FAIL:
                DPRINTF("csmp_sock: TX_FAIL - MAC dropped the packet!\n");
                break;
            case SOCKET_NO_ROUTE:
                DPRINTF("csmp_sock: NO_ROUTE - no route to destination\n");
                break;
            case SOCKET_NO_RAM:
                DPRINTF("csmp_sock: NO_RAM - insufficient memory\n");
                break;
            default:
                DPRINTF("csmp_sock: event=0x%x\n", (unsigned)cb->event_type);
                break;
        }
        return;
    }
    /* Read into a temporary buffer so we can inspect the CoAP type before
     * routing to the correct receive channel. */
    static uint8_t tmp[CSMP_TI_RX_BUF_LEN];
    int32_t rlen = socket_recvfrom(cb->socket_id, tmp, CSMP_TI_RX_BUF_LEN, 0, &src);
    if (rlen <= 0)
    {
        return;
    }
    /* CoAP type is in bits 4-5 of byte 0:
     *   0=CON, 1=NON  → server channel (inbound requests from FND)
     *   2=ACK, 3=RST  → client channel (responses to device's outgoing CON) */
    int ch = (rlen > 0 && ((tmp[0] >> 4) & 0x03) >= 2) ? CSMP_TI_CH_CLIENT : CSMP_TI_CH_SERVER;
    g_rx_len[ch] = rlen;
    memcpy(g_rx_buf[ch], tmp, (size_t)rlen);
    g_rx_src[ch].sin6_family = AF_INET6;
    g_rx_src[ch].sin6_port = htons(src.identifier);
    memcpy(g_rx_src[ch].sin6_addr.s6_addr, src.address, 16u);
    BaseType_t w = pdFALSE;
    xSemaphoreGiveFromISR(g_rx_sem[ch], &w);
    portYIELD_FROM_ISR(w);
}

/* Open (or return a synthetic handle for) the UDP socket used by CSMP CoAP */
osal_socket_handle_t osal_socket(osal_basetype_t d, osal_basetype_t t, osal_basetype_t p)
{
    (void)d;
    (void)t;
    (void)p;
    if (g_sock_alloc_count == 0)
    {
        /* First call: coapserver_listen() — open the real nanostack socket. */
        nanostack_lock();
        g_ns_sock = socket_open(SOCKET_UDP, 0, csmp_ns_socket_cb);
        nanostack_unlock();
        g_sock_alloc_count = 1;
        DPRINTF("osal_socket: server sock=%d\n", (int)g_ns_sock);
        return (osal_socket_handle_t)g_ns_sock;
    }
    else
    {
        g_sock_alloc_count++;
        DPRINTF("osal_socket: client sock=%d (synthetic)\n",
                (int)(g_ns_sock + CSMP_TI_CLIENT_FD_OFFSET));
        return (osal_socket_handle_t)(g_ns_sock + CSMP_TI_CLIENT_FD_OFFSET);
    }
}

/* Bind the nanostack UDP socket to the CoAP port */
osal_basetype_t osal_bind(osal_socket_handle_t sockd, osal_sockaddr_t *addr, osal_socklen_t addrlen)
{
    (void)addrlen;
    ns_address_t a = {0};
    a.type = ADDRESS_IPV6;
    a.identifier = ntohs(addr->sin6_port);
    nanostack_lock();
    int8_t r = socket_bind((int8_t)sockd, &a);
    nanostack_unlock();
    DPRINTF("osal_bind: sock=%d port=%u rv=%d (%s)\n",
            (int)sockd, (unsigned)ntohs(addr->sin6_port), (int)r, r == 0 ? "OK" : "FAIL");
    return r == 0 ? OSAL_SUCCESS : OSAL_FAILURE;
}

/* Block until a CoAP packet arrives on the correct channel and copy it to the caller's buffer */
osal_ssize_t osal_recvfrom(osal_socket_handle_t sockd, void *buf, size_t len,
                           osal_basetype_t flags, osal_sockaddr_t *src_addr,
                           osal_socklen_t *addrlen)
{
    (void)flags;
    /* Select channel: server handle (real fd) → channel 0 (CON/NON),
     * client handle (fd + CSMP_TI_CLIENT_FD_OFFSET) → channel 1 (ACK/RST). */
    int ch = ((int)sockd >= (int)(g_ns_sock + CSMP_TI_CLIENT_FD_OFFSET))
             ? CSMP_TI_CH_CLIENT : CSMP_TI_CH_SERVER;
    xSemaphoreTake(g_rx_sem[ch], portMAX_DELAY);
    int32_t n = (g_rx_len[ch] < (int32_t)len) ? g_rx_len[ch] : (int32_t)len;
    memcpy(buf, g_rx_buf[ch], (size_t)n);
    if (src_addr)
    {
        memcpy(src_addr, &g_rx_src[ch], sizeof(g_rx_src[ch]));
        if (addrlen)
        {
            *addrlen = (osal_socklen_t)sizeof(g_rx_src[ch]);
        }
    }
    return (osal_ssize_t)n;
}

/* Send a CoAP packet over the nanostack UDP socket, stripping the synthetic client offset */
osal_ssize_t osal_sendto(osal_socket_handle_t sockd, const void *buf, size_t len,
                         osal_basetype_t flags, const osal_sockaddr_t *dest,
                         osal_socklen_t addrlen)
{
    (void)flags;
    (void)addrlen;
    ns_address_t d = {0};
    d.type = ADDRESS_IPV6;
    d.identifier = ntohs(dest->sin6_port);
    memcpy(d.address, dest->sin6_addr.s6_addr, 16u);

    /* Strip the client channel offset — always send on the real nanostack socket. */
    int8_t real_fd = (int8_t)(((int)sockd >= (int)(g_ns_sock + CSMP_TI_CLIENT_FD_OFFSET))
                               ? g_ns_sock : (int8_t)sockd);
    DPRINTF("osal_sendto: PRE-DELAY sock=%d len=%u dst_port=%u\n",
            (int)real_fd, (unsigned)len, (unsigned)d.identifier);

    vTaskDelay(pdMS_TO_TICKS(10));
    DPRINTF("osal_sendto: POST-DELAY calling socket_sendto\n");

    int16_t r = socket_sendto(real_fd, &d, (void *)buf, (uint16_t)len);
    /* TI NSS socket_sendto returns 0 on success (not byte count like POSIX).
     * Return len on success so callers get the expected positive byte count. */
    if (r == 0)
    {
        DPRINTF("osal_sendto: sock=%d len=%u OK\n", (int)sockd, (unsigned)len);
        return (osal_ssize_t)len;
    }
    DPRINTF("osal_sendto: sock=%d len=%u FAILED r=%d\n", (int)sockd, (unsigned)len, (int)r);
    return (osal_ssize_t)r;
}

/* Scatter-gather send: forwards the first iov buffer via osal_sendto */
osal_ssize_t osal_sendmsg(osal_socket_handle_t sockd, const struct msghdr msg,
                          osal_basetype_t flags)
{
    if (!msg.msg_name || msg.msg_iovlen == 0)
    {
        return -1;
    }
    return osal_sendto(sockd, msg.msg_iov[0].iov_base, msg.msg_iov[0].iov_len,
                       flags, (const osal_sockaddr_t *)msg.msg_name,
                       (osal_socklen_t)msg.msg_namelen);
}

/* Convert a text IPv6 address string to a binary in6_addr using the Nanostack ip6string helper */
osal_basetype_t osal_inet_pton(osal_basetype_t af, const char *src, void *dst)
{
    (void)af;
    return stoip6(src, strlen(src), dst) ? 1 : 0;
}

/* No-op stub: socket readiness is always assumed available on this target */
osal_basetype_t osal_select(osal_basetype_t n, osal_sd_set_t *r,
                            osal_sd_set_t *w, osal_sd_set_t *e,
                            struct timeval *t)
{
    (void)n;
    (void)r;
    (void)w;
    (void)e;
    (void)t;
    return 1;
}

/* Zero-fill a sockaddr and set family and port, leaving the address as the wildcard */
void osal_update_sockaddr(osal_sockaddr_t *a, uint16_t p)
{
    memset(a, 0, sizeof(*a));
    a->sin6_family = AF_INET6;
    memset(&a->sin6_addr, 0, sizeof(a->sin6_addr));
    a->sin6_port = htons(p);
}

/* Print an IPv6 address and port in bracket notation via DPRINTF */
void osal_print_formatted_ip(const osal_sockaddr_t *s)
{
    DPRINTF("[%x:%x:%x:%x:%x:%x:%x:%x]:%hu\n",
            ((uint16_t)s->sin6_addr.s6_addr[0] << 8) | s->sin6_addr.s6_addr[1],
            ((uint16_t)s->sin6_addr.s6_addr[2] << 8) | s->sin6_addr.s6_addr[3],
            ((uint16_t)s->sin6_addr.s6_addr[4] << 8) | s->sin6_addr.s6_addr[5],
            ((uint16_t)s->sin6_addr.s6_addr[6] << 8) | s->sin6_addr.s6_addr[7],
            ((uint16_t)s->sin6_addr.s6_addr[8] << 8) | s->sin6_addr.s6_addr[9],
            ((uint16_t)s->sin6_addr.s6_addr[10] << 8) | s->sin6_addr.s6_addr[11],
            ((uint16_t)s->sin6_addr.s6_addr[12] << 8) | s->sin6_addr.s6_addr[13],
            ((uint16_t)s->sin6_addr.s6_addr[14] << 8) | s->sin6_addr.s6_addr[15],
            ntohs(s->sin6_port));
}

/* Return wall-clock time derived from ClockP ticks plus the stored epoch offset */
osal_basetype_t osal_gettime(struct timeval *tv, struct timezone *tz)
{
  (void)tz;
  if (!tv) return OSAL_FAILURE;
  uint32_t ticks  = ClockP_getSystemTicks();
  uint32_t us_pt  = ClockP_getSystemTickPeriod();
  int64_t  uptime = (int64_t)(((uint64_t)ticks * us_pt) / 1000000ULL);
  tv->tv_sec  = (time_t)(uptime + g_ti_csmp_time_offset_sec);
  tv->tv_usec = 0;
  return OSAL_SUCCESS;
}

/* Set the epoch offset so that osal_gettime matches the NMS-provided wall-clock time */
osal_basetype_t osal_settime(struct timeval *tv, struct timezone *tz)
{
  (void)tz;
  if (!tv) return OSAL_FAILURE;
  uint32_t ticks  = ClockP_getSystemTicks();
  uint32_t us_pt  = ClockP_getSystemTickPeriod();
  int64_t  uptime = (int64_t)(((uint64_t)ticks * us_pt) / 1000000ULL);
  g_ti_csmp_time_offset_sec = (int64_t)tv->tv_sec - uptime;
  DPRINTF("osal_settime: epoch=%lld offset=%lld\n",
          (long long)tv->tv_sec, (long long)g_ti_csmp_time_offset_sec);
  return OSAL_SUCCESS;
}

/* Signals — no-op on embedded */
/* No-op stub: signal delivery is not supported on this embedded target */
osal_sighandler_t osal_signal(osal_basetype_t n, osal_sighandler_t h)
{
    (void)n;
    (void)h;
    return NULL;
}

/* No-op stub: process signal masks are not applicable on FreeRTOS */
osal_basetype_t osal_sigprocmask(osal_basetype_t h, const osal_sigset_t *s, osal_sigset_t *o)
{
    (void)h;
    (void)s;
    (void)o;
    return OSAL_FAILURE;
}

/* No-op stub: signal sets are not used on this embedded target */
osal_basetype_t osal_sigemptyset(osal_sigset_t *s)
{
    (void)s;
    return OSAL_FAILURE;
}

/* No-op stub: signal sets are not used on this embedded target */
osal_basetype_t osal_sigaddset(osal_sigset_t *s, osal_basetype_t n)
{
    (void)s;
    (void)n;
    return OSAL_FAILURE;
}

/* Start a trickle timer with the given imin/imax interval bounds and fire callback */
void osal_trickle_timer_start(osal_timerid_t timerid, uint32_t imin, uint32_t imax,
                              trickle_timer_fired_t cb)
{
    uint32_t min;
    struct timeval tv = {0};
    /* Guard: imin=0 means "no periodic reporting" — rand() % 0 is UB. */
    if (imin == 0)
        return;
    if (!m_timert_isrunning)
    {
        m_timert_isrunning = true;
    }
    osal_gettime(&tv, NULL);
    srand((((uint16_t)g_csmplib_eui64[6] << 8) | g_csmplib_eui64[7]));
    timers[timerid].t0 = (uint32_t)tv.tv_sec + (uint32_t)(rand() % imin);
    timers[timerid].icur = imin;
    timers[timerid].imin = imin;
    timers[timerid].imax = imax;
    timers[timerid].is_running = true;
    timer_fired[timerid] = cb;
    min = timers[timerid].icur >> 1;
    timers[timerid].tfire = timers[timerid].t0 + min
                            + (uint32_t)(rand() % (timers[timerid].icur - min));
    xTimerStart(timers[timerid].timer, 0);
    osal_update_timer();
}

/* Stop a trickle timer and disable the global running flag if all timers are idle */
void osal_trickle_timer_stop(osal_timerid_t timerid)
{
    timers[timerid].is_running = false;
    for (uint8_t i = 0; i < (uint8_t)timer_num; i++)
        if (timers[i].is_running)
            return;
    m_timert_isrunning = false;
    xTimerStop(timers[timerid].timer, 0);
}

/* Allocate heap memory from the FreeRTOS heap */
void *osal_malloc(size_t n) { return pvPortMalloc(n); }

/* Free memory back to the FreeRTOS heap */
void osal_free(void *p) { vPortFree(p); }

/* Suspend the calling task for the given number of milliseconds */
void osal_sleep_ms(uint64_t ms) { vTaskDelay(pdMS_TO_TICKS((uint32_t)ms)); }

/* FreeRTOS stack-overflow hook: log the offending task name and halt */
void vApplicationStackOverflowHook(TaskHandle_t t, char *n)
{
    (void)t;
    DPRINTF("stack overflow: %s\n", n);
    for (;;)
    {
    }
}

/* Reboot the SoC via SysCtrl after suspending all tasks */
osal_basetype_t osal_system_reboot(struct in6_addr *NMSaddr)
{
  (void)NMSaddr;
  DPRINTF("osal_system_reboot\n");
  vTaskSuspendAll();
  SysCtrlSystemReset();
  return OSAL_SUCCESS;
}

/* Read an OTA slot header from NVOCMP persistent storage */
osal_basetype_t osal_read_slothdr(uint8_t slotid, Csmp_Slothdr *slot)
{
    if (!pNV || !pNV->readItem || !slot)
    {
        return OSAL_FAILURE;
    }

    NVINTF_itemID_t id = {
        .systemID = CSMP_TI_NV_SYSID,
        .itemID = CSMP_TI_NV_SLOTHDR_ITEM,
        .subID = slotid};

    uint8_t r = pNV->readItem(id, 0u, (uint16_t)sizeof(Csmp_Slothdr), slot);
    if (r != NVINTF_SUCCESS)
    {
        DPRINTF("osal_read_slothdr: slot %u err %u\n", (unsigned)slotid, (unsigned)r);
        return OSAL_FAILURE;
    }
    DPRINTF("osal_read_slothdr: slot %u ok\n", (unsigned)slotid);
    return OSAL_SUCCESS;
}

/* Write (or create) an OTA slot header in NVOCMP persistent storage */
osal_basetype_t osal_write_slothdr(uint8_t slotid, Csmp_Slothdr *slot)
{
    if (!pNV || !slot)
    {
        return OSAL_FAILURE;
    }

    NVINTF_itemID_t id = {
        .systemID = CSMP_TI_NV_SYSID,
        .itemID = CSMP_TI_NV_SLOTHDR_ITEM,
        .subID = slotid};

    uint8_t r = NVINTF_FAILURE;
    if (pNV->writeItem)
    {
        r = pNV->writeItem(id, (uint16_t)sizeof(Csmp_Slothdr), slot);
    }
    /* Item may not exist on first write — create it */
    if (r == NVINTF_NOTFOUND && pNV->createItem)
    {
        r = pNV->createItem(id, (uint16_t)sizeof(Csmp_Slothdr), slot);
    }

    if (r != NVINTF_SUCCESS)
    {
        DPRINTF("osal_write_slothdr: slot %u err %u\n", (unsigned)slotid, (unsigned)r);
        return OSAL_FAILURE;
    }
    DPRINTF("osal_write_slothdr: slot %u ok\n", (unsigned)slotid);
    return OSAL_SUCCESS;
}

/* Erase the external SPI flash secondary slot to prepare it for an OTA image upload */
osal_basetype_t ti_flash_erase_slot(uint8_t slotid)
{
    if (slotid != UPLOAD_IMAGE)
    {
        DPRINTF("ti_flash_erase_slot: only UPLOAD_IMAGE is in ext flash (slotid=%u)\n",
                (unsigned)slotid);
        return OSAL_FAILURE;
    }

    DPRINTF("OTA: erasing ext flash secondary slot (%lu bytes)...\n",
            (unsigned long)CSMP_TI_SLOT_MAX_SIZE);

    NVS_Handle h = NVS_open(CONFIG_NVSEXTERNAL, NULL);
    if (h == NULL)
    {
        DPRINTF("ti_flash_erase_slot: NVS_open failed\n");
        return OSAL_FAILURE;
    }
    int_fast16_t r = NVS_erase(h, 0, CSMP_TI_SLOT_MAX_SIZE);
    NVS_close(h);

    if (r != NVS_STATUS_SUCCESS)
    {
        DPRINTF("ti_flash_erase_slot: NVS_erase failed (%d)\n", (int)r);
        return OSAL_FAILURE;
    }
    DPRINTF("ti_flash_erase_slot: ext flash erased OK\n");
    return OSAL_SUCCESS;
}

/* Read firmware bytes from internal flash (RUN_IMAGE) or external SPI flash (UPLOAD_IMAGE) */
osal_basetype_t osal_read_firmware(uint8_t slotid, uint8_t *data, uint32_t size)
{
    if (data == NULL || size > CSMP_TI_SLOT_MAX_SIZE)
        return OSAL_FAILURE;

    if (slotid == RUN_IMAGE)
    {
        /* Running application is memory-mapped in internal flash */
        memcpy(data, (const void *)CSMP_TI_RUN_SLOT_ADDR, (size_t)size);
        return OSAL_SUCCESS;
    }
    if (slotid == UPLOAD_IMAGE)
    {
        NVS_Handle h = NVS_open(CONFIG_NVSEXTERNAL, NULL);
        if (h == NULL)
        {
            DPRINTF("osal_read_firmware: NVS_open failed\n");
            return OSAL_FAILURE;
        }
        int_fast16_t r = NVS_read(h, 0, data, (size_t)size);
        NVS_close(h);
        return (r == NVS_STATUS_SUCCESS) ? OSAL_SUCCESS : OSAL_FAILURE;
    }
    return OSAL_FAILURE;
}

/* Write a firmware chunk to the external SPI flash UPLOAD_IMAGE slot */
osal_basetype_t osal_write_firmware(uint8_t slotid, uint8_t *data, uint32_t size)
{
    if (slotid != UPLOAD_IMAGE || data == NULL || size > CSMP_TI_SLOT_MAX_SIZE)
        return OSAL_FAILURE;

    NVS_Handle h = NVS_open(CONFIG_NVSEXTERNAL, NULL);
    if (h == NULL)
    {
        DPRINTF("osal_write_firmware: NVS_open failed\n");
        return OSAL_FAILURE;
    }
    int_fast16_t r = NVS_write(h, 0, data, (size_t)size, NVS_WRITE_POST_VERIFY);
    NVS_close(h);
    return (r == NVS_STATUS_SUCCESS) ? OSAL_SUCCESS : OSAL_FAILURE;
}

/* No-op: MCUBoot handles firmware rollback natively; SetBackupRequest (TLV 70) is accepted but ignored */
osal_basetype_t osal_copy_firmware(uint8_t src_slotid, uint8_t dst_slotid,
                                   Csmp_Slothdr *slots)
{
    (void)src_slotid; (void)dst_slotid; (void)slots;
    DPRINTF("osal_copy_firmware: no-op — MCUBoot handles rollback natively\n");
    return OSAL_SUCCESS;
}

/* Internal trickle helpers — identical to osal_freertos.c */
/* Recalculate the nearest trickle fire time and reschedule the FreeRTOS timer accordingly */
static void osal_update_timer(void)
{
    struct timeval tv = {0};
    m_remaining = (int32_t)(((1UL << 31) - 1));
    osal_gettime(&tv, NULL);
    uint32_t now = (uint32_t)tv.tv_sec;
    for (uint8_t i = 0; i < (uint8_t)timer_num; i++)
    {
        if (!timers[i].is_running)
        {
            continue;
        }
        int32_t rem = (int32_t)(timers[i].tfire - now);
        if (rem < m_remaining)
        {
            m_remaining = rem;
            if (m_remaining <= 0)
            {
                xTimerPendFunctionCall(osal_alarm_fired_pend_fnc, NULL, 0, pdFALSE);
            }
            else
            {
                xTimerChangePeriod(timers[i].timer,
                                   pdMS_TO_TICKS((uint32_t)(m_remaining * 1000)), 0);
            }
        }
    }
}

/* FreeRTOS timer callback: fire expired trickle timers and advance their intervals */
static void osal_alarm_fired(TimerHandle_t t)
{
    (void)t;
    struct timeval tv = {0};
    osal_gettime(&tv, NULL);
    uint32_t now = (uint32_t)tv.tv_sec;
    for (uint8_t i = 0; i < (uint8_t)timer_num; i++)
    {
        if (!timers[i].is_running)
        {
            continue;
        }
        if ((int32_t)(timers[i].tfire - now) > 0)
        {
            continue;
        }
        timers[i].t0 += timers[i].icur;
        timers[i].icur <<= 1;
        if (timers[i].icur > timers[i].imax)
        {
            timers[i].icur = timers[i].imax;
        }
        timer_fired[i]();
        uint32_t min = timers[i].icur >> 1;
        timers[i].tfire = timers[i].t0 + min + (uint32_t)(rand() % (timers[i].icur - min));
    }
    osal_update_timer();
}

/* Timer daemon trampoline: invokes osal_alarm_fired from the FreeRTOS timer task context */
static void osal_alarm_fired_pend_fnc(void *p1, uint32_t p2)
{
    (void)p1;
    (void)p2;
    osal_alarm_fired(NULL);
}
