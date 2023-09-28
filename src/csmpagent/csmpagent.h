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

#ifndef __CSMPAGENT_H
#define __CSMPAGENT_H

#include <sys/types.h>
#include <netinet/in.h>
#include "csmpservice.h"

/*! \file
 *
 * CSMP agent
 *
 * code to interact with the NMS
 */

/**
 * @brief CoAP GET handler, based on tlvid as ULR
 *
 * @param tlvid tvlid to be retrieved
 * @param buf  request buffer
 * @param len  request buffer length
 * @param tlvindex   tlv url that is used
 * @return int 0 is success
 */
int csmpagent_get(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex);

/**
 * @brief coap POST handler, based on tlvid as URL
 *
 * @param tlvid tvlid to be retrieved
 * @param buf request buffer
 * @param len buffer size
 * @param out_buf response buffer
 * @param out_size response buffer size
 * @param out_len actual used length of the response buffer
 * @param tlvindex index
 * @return int  0 is success
 */
int csmpagent_post(tlvid_t tlvid, const uint8_t *buf, size_t len, uint8_t *out_buf, size_t out_size, size_t *out_len, int32_t tlvindex);

/**
 * @brief check signature
 *
 * @param buf request buffer
 * @param len request buffer size
 * @param agent request or response
 * @return int 0 is success
 */
int checkSignature(const uint8_t *buf, uint32_t len, bool agent);

/**
 * @brief check group
 *
 * @param buf request buffer
 * @param len request buffer length
 * @return true
 * @return false
 */
bool checkGroup(const uint8_t *buf, uint32_t len);

#endif
