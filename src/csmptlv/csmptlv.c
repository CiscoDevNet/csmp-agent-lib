/*
 *  Copyright 2021-2024 Cisco Systems, Inc.
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
#include <inttypes.h>

#include "protobuf-c.h"
#include "ProtobufVarint.h"

#include "csmp.h"
#include "csmptlv.h"

size_t csmptlv_write(uint8_t *buf, size_t len, tlvid_t tlvid, const ProtobufCMessage *msg) {
  uint8_t *p_cur, *p_tlvlen;
  uint32_t used = 0, rv;
  uint32_t packsize = 0;
  if ((buf == NULL) || (msg == NULL)) {
    DPRINTF("csmptlv_write error: (buf == NULL) || (msg == NULL)\n");
    return 0;
  }

  p_cur = buf;

  if (tlvid.vendor != 0) {
    rv = ProtobufVarint_encodeUINT32(p_cur,len - used,CSMP_TYPE_VENDOR);
    p_cur += rv; used += rv;
    if ((rv == 0) || (used > len)) {
      DPRINTF("csmptlv_write error: (rv == 0) || (used=%d > len=%ld)\n", used, len);
      return 0;
    }
    rv = ProtobufVarint_encodeUINT32(p_cur,len - used,tlvid.vendor);
    p_cur += rv; used += rv;
    if ((rv == 0) || (used > len)) {
      DPRINTF("csmptlv_write error: (rv == 0) || (used=%d > len=%ld)\n", used, len);
      return 0;
    }
  }

  rv = ProtobufVarint_encodeUINT32(p_cur,len - used,tlvid.type);
  p_cur += rv; used += rv;
  if ((rv == 0) || (used > len)) {
    DPRINTF("csmptlv_write error: (rv == 0) || (used=%d > len=%ld)\n", used, len);
    return 0;
  }

  p_tlvlen = p_cur;
  p_cur += CSMP_LEN_SKIP; used += CSMP_LEN_SKIP;

  packsize = protobuf_c_message_get_packed_size(msg);
  DPRINTF("csmptlv_write: len=%ld used=%d packsize=%d\n", len, used, packsize);
  if ((len-used) < packsize) {
    DPRINTF("csmptlv_write error: (len-used)=%ld < packsize=%d)\n", len-used, packsize);
    return 0;
  }
  else {
    protobuf_c_message_pack(msg, p_cur);
    rv = packsize;
  }

  p_cur += rv; used += rv;
  if ((rv == 0) || (used > len)) {
    DPRINTF("csmptlv_write error: (rv == 0) || (used=%d > len=%ld)\n", used, len);
    return 0;
  }

  // Now go back and write the length
  rv = ProtobufVarint_encodeUINT32(p_tlvlen,CSMP_LEN_SKIP,rv);
  if (rv == 0) {
    DPRINTF("csmptlv_write error: (rv == 0)\n");
    return 0;
  }

  while (rv < CSMP_LEN_SKIP) {
    p_tlvlen[(rv - 1)] |= 0x80;
    p_tlvlen[(rv)] = 0;
    rv++;
  }
  return used;
}

size_t csmptlv_readTL(const uint8_t *buf, size_t len, tlvid_t *ptlvid, uint32_t *ptlvlen) {
  const uint8_t *p_cur;
  uint32_t used = 0, rv;

  if ((buf == NULL) || (ptlvid == NULL) || (ptlvlen == NULL)) {
    return 0;
  }

  p_cur = buf;

  rv = ProtobufVarint_decodeUINT32(p_cur,len,&ptlvid->type);
  p_cur += rv; used += rv;
  if (rv == 0)
    return 0;

  if (ptlvid->type == CSMP_TYPE_VENDOR) {
    rv = ProtobufVarint_decodeUINT32(p_cur,len - used,&ptlvid->vendor);
    p_cur += rv; used += rv;
    if (rv == 0)
      return 0;

    rv = ProtobufVarint_decodeUINT32(p_cur,len - used,&ptlvid->type);
    p_cur += rv; used += rv;
    if (rv == 0)
      return 0;
  }
  else {
    ptlvid->vendor = 0;
  }

  rv = ProtobufVarint_decodeUINT32(p_cur,len - used,ptlvlen);
  p_cur += rv; used += rv;
  if (rv == 0)
    return 0;

  return used;
}

size_t csmptlv_readV(const uint8_t *buf, size_t len,
    ProtobufCMessage **msg, const ProtobufCMessageDescriptor *desc) {
  uint32_t rv;

  *msg = protobuf_c_message_unpack(desc, NULL, len, buf);
  if (!(*msg)) {
    DPRINTF("ProtobufMsg_unpack error!\n");
    rv = 0;
  }
  else {
    rv = protobuf_c_message_get_packed_size(*msg);
  }

  return rv;
}

size_t csmptlv_read(const uint8_t *buf, size_t len, tlvid_t *ptlvid,
    ProtobufCMessage **msg, const ProtobufCMessageDescriptor *desc) {
  const uint8_t *p_cur;
  uint32_t in_tlvlen;
  uint32_t used = 0, rv;

  p_cur = buf;

  rv = csmptlv_readTL(p_cur,len,ptlvid,&in_tlvlen);
  p_cur += rv; used += rv;
  if (rv == 0)
    return 0;

  if (in_tlvlen > (len-used))
    return 0;

  rv = csmptlv_readV(p_cur,in_tlvlen, msg, desc);
  if (rv == 0)
    return 0;

  used += rv;

  return used;
}

const uint8_t *csmptlv_find(const uint8_t *buf, size_t len, tlvid_t qtlvid, uint32_t *pmsglen) {
  tlvid_t tlvid = {0,0};
  uint32_t tlvlen = 0;
  const uint8_t *pbuf = buf;
  uint32_t used = 0;
  uint32_t rv;

  while (used < len) {
    rv = csmptlv_readTL(pbuf,len - used ,&tlvid, &tlvlen);
    if (rv == 0)
      return NULL;
    if ((tlvid.type == qtlvid.type) && (tlvid.vendor == qtlvid.vendor)) {
      if (pmsglen)
        *pmsglen = rv + tlvlen;
      return pbuf;
    }
    pbuf += rv + tlvlen; used += rv + tlvlen;
  }

  return NULL;
}

int csmptlv_str2id(const char *str, tlvid_t *ptlvid) {
  int rv;
  rv = sscanf(str,"e%" SCNu32 ".%" SCNu32, &ptlvid->vendor,&ptlvid->type);
  if (rv == 0) {
    ptlvid->vendor = 0;
    rv = sscanf(str,"%" SCNu32 ,&ptlvid->type);
  }
  return rv;
}

int csmptlv_id2str(char *str, size_t str_size, const tlvid_t *ptlvid) {
  int rv;
  if (ptlvid->vendor != 0) {
    rv = snprintf(str,str_size,"e%" PRIuLEAST32 ".%" PRIuLEAST32 ,ptlvid->vendor,ptlvid->type);
  }
  else {
    rv = snprintf(str,str_size,"%" PRIuLEAST32 ,ptlvid->type);
  }
  return rv;
}

void csmptlv_free(ProtobufCMessage *message)
{
  protobuf_c_message_free_unpacked(message, NULL);
}
