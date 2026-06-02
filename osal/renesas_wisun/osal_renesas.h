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

/**
 * @file  osal_renesas.h
 * @brief Configuration and API of the CSMP module running within the Renesas Wi-SUN FAN sample application.
 */

#ifndef OSAL_RENESAS_H
#define OSAL_RENESAS_H

#include <stdint.h>
#include "r_nwk_api.h"

/** the major version of the CSMP module */
#define R_CSMP_MAJOR_VERSION    "0"

/** the major version of the CSMP module */
#define R_CSMP_MINOR_VERSION    "1"

/** the build version of the CSMP module */
#define R_CSMP_BUILD_VERSION    "0"

/** string to identify this version of the CSMP module (in format "major.minor.build") */
#define R_CSMP_VERSION_STRING   R_CSMP_MAJOR_VERSION "." R_CSMP_MINOR_VERSION "." R_CSMP_BUILD_VERSION

/** string to identify the running firmware */
#define R_CSMP_FILE_NAME "Wi-SUN-FAN-SampleApp"

/** string to identify this hardware platform as used by FND */
#define R_CSMP_HARDWARE_ID "renesastest"

/** max number of ip addresses */
#define R_TLV_IPADDRESS_MAX_NUM 3

/** max number of routes */
#define R_TLV_ROUTES_MAX_NUM    2

/**
 * Start the CSMP module. Several periodic timers will be started.
 * @param serverAddr The IPv6 address of the server (Cisco IoT Field Network Director).
 * @param localEui64 The EUI-64 of this device.
 * @param regImin The minimum registration interval of this device.
 * @param regImax The maximum registration interval of this device.
 * @return 0 if the CSMP module was successfully started.
 */
int R_CSMP_Start(const r_ipv6addr_t* serverAddr, const uint8_t* localEui64, uint32_t regImin, uint32_t regImax);

/**
 * Stop the CSMP module and related timers.
 * @return 0 if the CSMP module was successfully stopped.
 */
int R_CSMP_Stop();
#endif /* OSAL_RENESAS_H */
