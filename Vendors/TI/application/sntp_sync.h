/*
 * sntp_sync.h — Minimal SNTP client (RFC 4330) for TI Wi-SUN devices.
 *
 * Queries an NTP server over the Wi-SUN IPv6 network immediately after
 * joining, matching the same approach Silicon Labs uses with
 * sl_wisun_ntp_timesync() on EFR32 platforms.  This eliminates the
 * compile-time clock lag that causes LoadRequest timer calculations to
 * fire minutes late.
 */
#ifndef SNTP_SYNC_H
#define SNTP_SYNC_H

#include <stdint.h>

/* Query NTP server and sync the device clock; call after join, before csmp_service_start(). */
int sntp_sync(const char *ntp_server_ipv6);

#endif /* SNTP_SYNC_H */
