/*
 * Cisco CSMP Reference Implementation Source Code License Agreement v1.0
 *
 * These terms govern your use of Cisco Systems, Inc.’s CSMP Reference
 * Implementation Source Code and associated documentation (“Licensed
 * Materials”). By accessing, downloading, copying, distributing, calling, or
 * otherwise making any use of the Licensed Materials, you accept and agree to
 * be bound by the following terms and conditions (the “License”). If you are
 * accepting the License on behalf of an entity, you represent that you have the
 * authority to do so (either you or the entity, “you”). All use of the Licensed
 * Materials must be in accordance with this License and all rights not
 * expressly granted herein are reserved.

 * 1.	LICENSE GRANT.
 * Subject to Your compliance with the terms of this License, Cisco grants you
 * a limited, non-exclusive, perpetual, revocable, non-sublicensable,
 * royalty-free, worldwide, and non-transferable license to: (a) use the
 * Licensed Materials for the purpose of internal development, testing,
 * maintenance and support of your products to interoperate with Cisco 802.15.4
 * border routers -- including without limitation the  Cisco 1000 Series
 * Connected Grid Routers – and Cisco IoT Field Network Director and any
 * successor products (collectively the “Cisco Products”), and for no other
 * purpose; (b) use and reproduce a reasonable number of copies of the Licensed
 * Materials solely for the internal development, testing, back-up, maintenance
 * and support of your products as permitted under this License; (c) make
 * corrections and bug fixes to the Licensed Materials only if such corrections
 * and bug fixes do not interfere with or inhibit the interoperability of your
 * products with the Cisco Products; and (d) distribute the CSMP Reference
 * Implementation Source Code solely in compiled, binary format with your
 * products to your customers solely to enable your products to interoperate
 * with the Cisco Products.

 * 2.	CONDITIONS.
 * You shall not use the Licensed Materials to: (a) substantially replicate and
 * compete with the Cisco Products; (b) collect, store, or transmit confidential
 * , sensitive, or protected data or information without authorization; (c)
 * disable, disrupt, circumvent, interfere with, or otherwise violate the
 * security of any Cisco product, service, or user;  (d) access any application,
 * system, service, computer, data, account, or network without authorization;
 * (e) develop features or functions in stand-alone applications that do not
 * interoperate with the Cisco Products;  or (f) sell or offer to sell the
 * Licensed Products on a standalone basis. The Cisco Products are licensed
 * under their own separate terms and you shall not use the Licensed Materials
 * in any way that violates or is inconsistent with those terms (for more
 * information, please visit: www.cisco.com/go/terms). Failure to comply with
 * any part of this section or section 2 constitutes a material breach of this
 * License, terminates the license granted in Section 1 above, and may result in
 * further repercussions and actions against you, at Cisco’s sole discretion.

 * 3.	OWNERSHIP.
 * The Licensed Materials, in whole or in part, in all forms, and any
 * corrections or bug fixes thereto that you make are the sole and exclusive
 * property of Cisco.   You irrevocably transfer, convey, and assign to Cisco
 * all of your right, title, and interest in any corrections or bug fixes you
 * make to the Licensed Materials.  You shall send the corrections or bug fixes
 * to ask-crmesh-dev@cisco.com.  This License does not grant you permission to
 * use any trade names, trademarks, service marks, or product names of Cisco.
 * If you provide any feedback to Cisco regarding the Licensed Materials, you
 * agree that Cisco, its suppliers, and its customers shall be free to use and
 * incorporate such feedback into the Licensed Materials and Cisco products and
 * services without restriction, payment, or additional consideration of any
 * kind. If you initiate or participate in any litigation against Cisco, its
 * partners, or its customers (including cross-claims and counter-claims)
 * alleging that the Licensed Materials or its use infringe any patent,
 * copyright, or other intellectual property right, then all rights granted to
 * you under this License shall terminate immediately.

 * 4.	CONFIDENTIAL INFORMATION.
 * The Licensed Materials, whether in whole or in part, contain confidential
 * information and proprietary property of Cisco.  Licensee acknowledges that
 * the Licensed Materials contain trade secrets and other confidential
 * information which are the property of Cisco.  You shall keep the Licensed
 * Materials in strict confidence and take all reasonable steps to prevent: (a)
 * unauthorized persons from having access to the Licensed Materials; (b)
 * disclosure of the Licensed Materials to any third party; and (c) unauthorized
 * copying of the Licensed Materials.

 * 5.	PROPRIETARY NOTICES.
 * You agree to maintain and reproduce all copyright, trademarks and other
 * proprietary notices on all copies of the Licensed Materials, in any form, in
 * the same form and manner that such copyright, trademarks, and other
 * proprietary notices are included on the Licensed Materials.

 * 6.	NO SUPPORT.
 * Nothing in this License shall require Cisco to provide any support, services,
 * updates, upgrades, bug fixes or any other modifications to the Licensed
 * Materials.  You shall be solely responsible for providing support to your
 * customers for any problems or issues that they encounter by using the CSMP
 * Reference Implementation Source Code in binary form that you distributed.
 * Cisco may, at its own sole discretion, update, extend, enhance, or otherwise
 * modify the Licensed Materials without notice.  If Cisco provides updates to
 * you and no separate agreement accompanies such updates, then such updates are
 * subject to this License.

 * 7.	LIMITATION OF LIABILITY.
 * CISCO SHALL HAVE NO LIABILITY IN CONNECTION WITH OR RELATING TO THIS LICENSE
 * OR USE OF THE LICENSED MATERIALS, FOR DAMAGES OF ANY KIND, INCLUDING BUT NOT
 * LIMITED TO DIRECT, INCIDENTAL, AND CONSEQUENTIAL DAMAGES, OR FOR ANY LOSS OF
 * USE, DATA, INFORMATION, PROFITS, BUSINESS, OR GOODWILL, HOWEVER CAUSED, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

 * 8.	DISCLAIMER OF WARRANTY.
 * CISCO PROVIDES THE LICENSED MATERIALS “AS IS” WITH ALL FAULTS, WITHOUT
 * WARRANTY OF ANY KIND. TO THE MAXIMUM EXTENT PERMITTED BY LAW, ALL EXPRESS AND
 * IMPLIED CONDITIONS, REPRESENTATIONS, AND WARRANTIES INCLUDING, WITHOUT
 * LIMITATION, ANY IMPLIED WARRANTY OR CONDITION OF MERCHANTABILITY, FITNESS FOR
 * A PARTICULAR PURPOSE, NON-INFRINGEMENT, SATISFACTORY QUALITY,
 * NON-INTERFERENCE, AND ACCURACY, ARE HEREBY EXCLUDED AND EXPRESSLY DISCLAIMED
 * BY CISCO. CISCO DOES NOT WARRANT THAT THE LICENSED MATERIALS ARE SUITABLE FOR
 * YOUR USE, WILL OPERATE PROPERLY WITH YOUR APPLICATION, ARE ACCURATE OR
 * COMPLETE, OR ARE WITHOUT ERROR OR DEFECT.

 * 9.	GENERAL.
 * This License shall be governed by and interpreted in accordance with the laws
 * of the State of California, excluding its conflict of law’s provisions. You
 * agree to comply with all applicable United States export laws, rules, and
 * regulations. If any provision of this License is judged illegal, invalid, or
 * otherwise unenforceable, that provision shall be severed, and the rest of the
 * License shall remain in full force and effect. No failure by Cisco to enforce
 * any of its rights related to the Licensed Materials or to a breach of this
 * License in a particular situation will act as a waiver of such rights. In the
 * event of any inconsistencies with any other terms that may apply to the
 * Licensed Materials, this License shall take precedence.
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

uint32_t ProtobufVarint_encodeUINT32(uint8_t *buf, uint32_t len, uint32_t val) {
  return uint32_pack(val, buf);
}

uint32_t ProtobufVarint_encodeINT32(uint8_t *buf, uint32_t len, int32_t val) {
  return int32_pack(val,buf);
}

uint32_t ProtobufVarint_encodeSINT32(uint8_t *buf, uint32_t len, int32_t val) {
  return sint32_pack(val,buf);
}

uint32_t ProtobufVarint_encodeUINT64(uint8_t *buf, uint32_t len, uint64_t val) {
  return uint64_pack(val, buf);
}

uint32_t ProtobufVarint_encodeINT64(uint8_t *buf, uint32_t len, int64_t val) {
  return uint64_pack((uint64_t)val,buf);
}

uint32_t ProtobufVarint_encodeSINT64(uint8_t *buf, uint32_t len, int64_t val) {
  return sint64_pack(val,buf);
}

uint32_t ProtobufVarint_decodeUINT32(const uint8_t *buf, uint32_t len, uint32_t *val) {
  return parse_uint32(val,buf,len);
}

uint32_t ProtobufVarint_decodeINT32(const uint8_t *buf, uint32_t len, int32_t *val) {
  return parse_int32(val,buf,len);
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
