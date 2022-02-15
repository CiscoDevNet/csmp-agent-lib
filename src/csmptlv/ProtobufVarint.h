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

#ifndef __PROTOBUFVARINT_H
#define __PROTOBUFVARINT_H

uint32_t ProtobufVarint_encodeUINT32(uint8_t *buf, uint32_t len, uint32_t val);
uint32_t ProtobufVarint_encodeINT32(uint8_t *buf, uint32_t len, int32_t val);
uint32_t ProtobufVarint_encodeSINT32(uint8_t *buf, uint32_t len, int32_t val);
uint32_t ProtobufVarint_encodeUINT64(uint8_t *buf, uint32_t len, uint64_t val);
uint32_t ProtobufVarint_encodeINT64(uint8_t *buf, uint32_t len, int64_t val);
uint32_t ProtobufVarint_encodeSINT64(uint8_t *buf, uint32_t len, int64_t val);

uint32_t ProtobufVarint_decodeUINT32(const uint8_t *buf, uint32_t len, uint32_t *val);
uint32_t ProtobufVarint_decodeINT32(const uint8_t *buf, uint32_t len, int32_t *val);
uint32_t ProtobufVarint_decodeSINT32(const uint8_t *buf, uint32_t len, int32_t *val);
uint32_t ProtobufVarint_decodeUINT64(const uint8_t *buf, uint32_t len, uint64_t *val);
uint32_t ProtobufVarint_decodeINT64(const uint8_t *buf, uint32_t len, int64_t *val);
uint32_t ProtobufVarint_decodeSINT64(const uint8_t *buf, uint32_t len, int64_t *val);

#endif
