/*
 * sntp_sync.c — Minimal SNTP client for TI Wi-SUN
 *
 * Uses nanostack's callback-based UDP socket API (the same layer as the CSMP
 * agent).  A FreeRTOS binary semaphore synchronises the callback with the
 * calling application task so the function blocks cleanly for up to
 * SNTP_TIMEOUT_MS without polling.
 *
 * SNTP protocol (RFC 4330):
 *   Request:  48 bytes UDP to port 123 — byte 0 = 0x1B, rest zeros.
 *   Response: bytes 40-43 = transmit timestamp (big-endian, seconds since
 *             1900-01-01).  Subtract NTP_UNIX_DELTA (70 years) for Unix epoch.
 */

#include "sntp_sync.h"

#include <string.h>
#include <unistd.h>    /* usleep */

/* nanostack socket layer — same API used by CSMP agent */
#include "socket_api.h"
#include "net_interface.h"   /* SOCKET_DATA and other socket event defines */
#include "ns_address.h"
#include "ip6string.h"

/* FreeRTOS — binary semaphore for blocking wait */
#include "FreeRTOS.h"
#include "semphr.h"

#include "ns_trace.h"
#include "osal.h"

#define TRACE_GROUP     "sntp"
#define NTP_PORT        123u
#define SNTP_PKT_SIZE   48u
/* 70 years of seconds between 1900-01-01 (NTP epoch) and 1970-01-01 (Unix) */
#define NTP_UNIX_DELTA  2208988800UL
#define SNTP_TIMEOUT_MS 5000u

/* ---------- module-level state (safe: only one sntp_sync() call at a time) */
static SemaphoreHandle_t  s_sem      = NULL;
static uint8_t            s_resp[SNTP_PKT_SIZE];
static volatile int       s_resp_len = 0;

/* Nanostack UDP socket callback: receives SNTP response and signals the waiting task */
static void sntp_socket_cb(void *cb_arg)
{
    socket_callback_t *cb = (socket_callback_t *)cb_arg;
    if (cb->event_type != SOCKET_DATA || cb->d_len < SNTP_PKT_SIZE)
        return;

    ns_address_t from;
    int32_t len = socket_recvfrom(cb->socket_id,
                                  s_resp, sizeof(s_resp), 0, &from);
    if (len == (int32_t)SNTP_PKT_SIZE)
    {
        s_resp_len = (int)len;
        xSemaphoreGive(s_sem);
    }
}

/* Sends an SNTP request to the given IPv6 server and sets the system clock on success */
int sntp_sync(const char *ntp_server_ipv6)
{
    int rc = -1;

    if (!ntp_server_ipv6)
    {
        tr_error("sntp: NULL server");
        return -1;
    }

    /* Create one-shot binary semaphore for response notification */
    s_sem = xSemaphoreCreateBinary();
    if (!s_sem)
    {
        tr_error("sntp: semaphore alloc failed");
        return -1;
    }
    s_resp_len = 0;

    /* Open ephemeral UDP socket with our callback */
    int8_t sock = socket_open(SOCKET_UDP, 0, sntp_socket_cb);
    if (sock < 0)
    {
        tr_error("sntp: socket_open failed %d", sock);
        goto cleanup;
    }

    /* Build 48-byte NTP client request — byte 0 = LI=0, VN=3, Mode=3 */
    uint8_t pkt[SNTP_PKT_SIZE];
    memset(pkt, 0, sizeof(pkt));
    pkt[0] = 0x1Bu;

    /* Target: NTP server port 123 */
    ns_address_t server;
    memset(&server, 0, sizeof(server));
    server.type       = ADDRESS_IPV6;
    server.identifier = NTP_PORT;
    stoip6(ntp_server_ipv6, strlen(ntp_server_ipv6), server.address);

    int32_t sent = socket_sendto(sock, &server, pkt, SNTP_PKT_SIZE);
    if (sent < 0)
    {
        tr_error("sntp: sendto failed (err=%ld)", (long)sent);
        goto close;
    }
    tr_info("sntp: request sent to [%s]:123", ntp_server_ipv6);

    /* Wait up to SNTP_TIMEOUT_MS for the callback to signal the semaphore */
    if (xSemaphoreTake(s_sem, pdMS_TO_TICKS(SNTP_TIMEOUT_MS)) != pdTRUE)
    {
        tr_error("sntp: timeout (%u ms) — no response from [%s]:123",
                 SNTP_TIMEOUT_MS, ntp_server_ipv6);
        goto close;
    }

    if (s_resp_len != (int)SNTP_PKT_SIZE)
    {
        tr_error("sntp: short response %d bytes", s_resp_len);
        goto close;
    }

    /* Extract transmit timestamp: bytes 40-43, big-endian, seconds since 1900 */
    uint32_t ntp_sec = ((uint32_t)s_resp[40] << 24u) |
                       ((uint32_t)s_resp[41] << 16u) |
                       ((uint32_t)s_resp[42] <<  8u) |
                       ((uint32_t)s_resp[43]);

    if (ntp_sec < NTP_UNIX_DELTA)
    {
        tr_error("sntp: implausible timestamp %lu", (unsigned long)ntp_sec);
        goto close;
    }

    uint32_t unix_sec = ntp_sec - (uint32_t)NTP_UNIX_DELTA;

    struct timeval tv = { (time_t)unix_sec, 0 };
    osal_settime(&tv, NULL);

    tr_info("sntp: clock set to Unix=%lu (NTP=%lu)", (unsigned long)unix_sec,
            (unsigned long)ntp_sec);
    rc = 0;

close:
    socket_close(sock);
cleanup:
    vSemaphoreDelete(s_sem);
    s_sem = NULL;
    return rc;
}
