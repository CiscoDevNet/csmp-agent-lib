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

#ifndef _CSMPTLV_H
#define _CSMPTLV_H

#include "protobuf-c.h"

size_t csmptlv_write(uint8_t *buf, size_t len, tlvid_t tlvid, const ProtobufCMessage *msg);
size_t csmptlv_read(const uint8_t *buf, size_t len, tlvid_t *ptlvid, ProtobufCMessage **msg, const ProtobufCMessageDescriptor *desc);
size_t csmptlv_readTL(const uint8_t *buf, size_t len, tlvid_t *ptlvid, uint32_t *ptlvlen);
size_t csmptlv_readV(const uint8_t *buf, size_t len, ProtobufCMessage **msg, const ProtobufCMessageDescriptor *desc);
void csmptlv_free(ProtobufCMessage *message);
const uint8_t *csmptlv_find(const uint8_t *buf, size_t len, tlvid_t tlvid, uint32_t *pmsglen);
int csmptlv_str2id(const char *str, tlvid_t *ptlvid);
int csmptlv_id2str(char *str, size_t str_size, const tlvid_t *ptlvid);

#endif
