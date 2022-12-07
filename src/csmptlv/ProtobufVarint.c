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

#include <stdio.h>
#include <string.h>
#include "protobuf-c.h"

/* =========== pack() ============ */
/* Pack an unsigned 32-bit integer in base-128 encoding, and return the number of bytes needed:
   this will be 5 or less. */
  static inline size_t
uint32_pack (uint32_t value, uint8_t *out)
{
  unsigned rv = 0;
  if (value >= 0x80)
  {
    out[rv++] = value | 0x80;
    value >>= 7;
    if (value >= 0x80)
    {
      out[rv++] = value | 0x80;
      value >>= 7;
      if (value >= 0x80)
      {
        out[rv++] = value | 0x80;
        value >>= 7;
        if (value >= 0x80)
        {
          out[rv++] = value | 0x80;
          value >>= 7;
        }
      }
    }
  }
  /* assert: value<128 */
  out[rv++] = value;
  return rv;
}

/* Pack a 32-bit signed integer, returning the number of bytes needed.
   Negative numbers are packed as twos-complement 64-bit integers. */
  static inline size_t
int32_pack (int32_t value, uint8_t *out)
{
  if (value < 0)
  {
    out[0] = value | 0x80;
    out[1] = (value>>7) | 0x80;
    out[2] = (value>>14) | 0x80;
    out[3] = (value>>21) | 0x80;
    out[4] = (value>>28) | 0x80;
    out[5] = out[6] = out[7] = out[8] = 0xff;
    out[9] = 0x01;
    return 10;
  }
  else
    return uint32_pack (value, out);
}

/* return the zigzag-encoded 32-bit unsigned int from a 32-bit signed int */
  static inline uint32_t
zigzag32 (int32_t v)
{
  if (v < 0)
    return ((uint32_t)(-v)) * 2 - 1;
  else
    return v * 2;
}

/* Pack a 32-bit integer in zigwag encoding. */
  static inline size_t
sint32_pack (int32_t value, uint8_t *out)
{
  return uint32_pack (zigzag32 (value), out);
}

/* Pack a 64-bit unsigned integer that fits in a 64-bit uint,
   using base-128 encoding. */
  static size_t
uint64_pack (uint64_t value, uint8_t *out)
{
  uint32_t hi = (uint32_t )(value>>32);
  uint32_t lo = (uint32_t )value;
  unsigned rv;
  if (hi == 0)
    return uint32_pack ((uint32_t)lo, out);
  out[0] = (lo) | 0x80;
  out[1] = (lo>>7) | 0x80;
  out[2] = (lo>>14) | 0x80;
  out[3] = (lo>>21) | 0x80;
  if (hi < 8)
  {
    out[4] = (hi<<4) | (lo>>28);
    return 5;
  }
  else
  {
    out[4] = ((hi&7)<<4) | (lo>>28) | 0x80;
    hi >>= 3;
  }
  rv = 5;
  while (hi >= 128)
  {
    out[rv++] = hi | 0x80;
    hi >>= 7;
  }
  out[rv++] = hi;
  return rv;
}

/* return the zigzag-encoded 64-bit unsigned int from a 64-bit signed int */
  static inline uint64_t
zigzag64 (int64_t v)
{
  if (v < 0)
    return ((uint64_t)(-v)) * 2 - 1;
  else
    return v * 2;
}

/* Pack a 64-bit signed integer in zigzan encoding,
   return the size of the packed output.
   (Max returned value is 10) */
  static inline size_t
sint64_pack (int64_t value, uint8_t *out)
{
  return uint64_pack (zigzag64 (value), out);
}

  static inline size_t
parse_uint32(uint32_t *member, const uint8_t *data, uint32_t len)
{
  size_t used = 0;
  const uint8_t *prem;
  uint32_t rv = data[0] & 0x7f; used++;
  if ((len > 1) && (data[0] & 0x80))
  {
    rv |= ((data[1] & 0x7f) << 7); used++;
    if ((len > 2) && (data[1] & 0x80))
    {
      rv |= ((data[2] & 0x7f) << 14); used++;
      if ((len > 3) && (data[2] & 0x80))
      {
        rv |= ((data[3] & 0x7f) << 21); used++;
        if ((len > 4) && (data[3] & 0x80))
        {
          rv |= (data[4] << 28); used++;
          prem = &data[4];
          while ((*prem++ & 0x80) && (used < 10))
            used++;
          //if (data[4] & 0x80)
          //  return 0;
        }
      }
    }
  }
  *member = rv;
  return used;
}

  static inline size_t
parse_int32(uint32_t *member, const uint8_t *data, uint32_t len)
{
  return parse_uint32(member,data,len);
}

  static inline int32_t
unzigzag32 (uint32_t v)
{
  if (v&1)
    return -(v>>1) - 1;
  else
    return v>>1;
}

  static inline size_t
parse_sint32(int32_t *member, const uint8_t *data, uint32_t len)
{
  uint32_t v;
  size_t rv = parse_uint32(&v,data,len);
  *member = unzigzag32(v);
  return rv;
}

  static inline size_t
parse_uint64 (uint64_t *member, const uint8_t *data, uint32_t len)
{
  uint64_t rv = 0;
  uint32_t i = 0, shift = 0;

  while ((i < len) && (i < 10)) {
    rv |= ((data[i] & 0x7F) << shift);
    if (!(data[i++] & 0x80))
      break;
    shift +=7;
  }
  *member = rv;
  return i;
}

  static inline int64_t
unzigzag64 (uint64_t v)
{
  if (v&1)
    return -(v>>1) - 1;
  else
    return v>>1;
}

  static inline size_t
parse_sint64(int64_t *member, const uint8_t *data, uint32_t len)
{
  uint64_t v;
  size_t rv = parse_uint64(&v,data,len);
  *member = unzigzag64(v);
  return rv;
}

uint32_t ProtobufVarint_encodeUINT32(uint8_t *buf, uint32_t val) {
  return uint32_pack(val, buf);
}

uint32_t ProtobufVarint_encodeINT32(uint8_t *buf, int32_t val) {
  return int32_pack(val,buf);
}

uint32_t ProtobufVarint_encodeSINT32(uint8_t *buf, int32_t val) {
  return sint32_pack(val,buf);
}

uint32_t ProtobufVarint_encodeUINT64(uint8_t *buf, uint64_t val) {
  return uint64_pack(val, buf);
}

uint32_t ProtobufVarint_encodeINT64(uint8_t *buf, int64_t val) {
  return uint64_pack((uint64_t)val,buf);
}

uint32_t ProtobufVarint_encodeSINT64(uint8_t *buf, int64_t val) {
  return sint64_pack(val,buf);
}

uint32_t ProtobufVarint_decodeUINT32(const uint8_t *buf, uint32_t len, uint32_t *val) {
  return parse_uint32(val,buf,len);
}

uint32_t ProtobufVarint_decodeINT32(const uint8_t *buf, uint32_t len, int32_t *val) {
  return parse_int32((uint32_t *)val,buf,len);
}

uint32_t ProtobufVarint_decodeSINT32(const uint8_t *buf, uint32_t len, int32_t *val) {
  return parse_sint32(val,buf,len);
}

uint32_t ProtobufVarint_decodeUINT64(const uint8_t *buf, uint32_t len, uint64_t *val) {
  return parse_uint64(val,buf,len);
}

uint32_t ProtobufVarint_decodeINT64(const uint8_t *buf, uint32_t len, int64_t *val) {
  return parse_uint64((uint64_t *)val,buf,len);
}

uint32_t ProtobufVarint_decodeSINT64(const uint8_t *buf, uint32_t len, int64_t *val) {
  return parse_sint64(val,buf,len);
}
