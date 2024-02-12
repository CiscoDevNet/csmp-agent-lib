/*
 *  Copyright 2021 Cisco Systems, Inc.
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

#ifndef __CSMPFUNCTION_H
#define __CSMPFUNCTION_H

#include <sys/types.h>
#include <netinet/in.h>

int csmp_get_tlvindex(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_deviceid(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_sessionID(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_groupAssign(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_groupInfo(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_reportSubscribe(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_hardwareDesc(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_interfaceDesc(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_ipAddress(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_ipRoute(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_currenttime(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_uptime(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_interfaceMetrics(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_ipRouteRplMetrics(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_wpanStatus(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_cgmsNotification(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_cgmsStats(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_rplInstance(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_firmwareImageInfo(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_signatureValidity(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_signature(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_signatureSettings(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);
int csmp_get_vendorTlv(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);

int csmp_put_currenttime(tlvid_t tlvid, const uint8_t *buf, size_t len,
                         uint8_t *out_buf, size_t out_size, size_t *out_len,
                         int32_t tlvindex);
int csmp_put_sessionID(tlvid_t tlvid, const uint8_t *buf, size_t len,
                         uint8_t *out_buf, size_t out_size, size_t *out_len,
                         int32_t tlvindex);
int csmp_put_signature(tlvid_t tlvid, const uint8_t *buf, size_t len,
                         uint8_t *out_buf, size_t out_size, size_t *out_len,
                         int32_t tlvindex);
int csmp_put_signatureValidity(tlvid_t tlvid, const uint8_t *buf, size_t len,
                         uint8_t *out_buf, size_t out_size, size_t *out_len,
                         int32_t tlvindex);
int csmp_put_signatureSettings(tlvid_t tlvid, const uint8_t *buf, size_t len,
                         uint8_t *out_buf, size_t out_size, size_t *out_len,
                         int32_t tlvindex);
int csmp_put_groupAssign(tlvid_t tlvid, const uint8_t *buf, size_t len,
                         uint8_t *out_buf, size_t out_size, size_t *out_len,
                         int32_t tlvindex);
int csmp_put_groupMatch(tlvid_t tlvid, const uint8_t *buf, size_t len,
                         uint8_t *out_buf, size_t out_size, size_t *out_len,
                         int32_t tlvindex);
int csmp_put_reportSubscribe(tlvid_t tlvid, const uint8_t *buf, size_t len,
                         uint8_t *out_buf, size_t out_size, size_t *out_len,
                         int32_t tlvindex);
int csmp_put_vendorTlv(tlvid_t tlvid, const uint8_t *buf, size_t len,
                         uint8_t *out_buf, size_t out_size, size_t *out_len,
                         int32_t tlvindex);

#endif
