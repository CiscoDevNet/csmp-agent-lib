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

#ifndef _SIGNATURE_H
#define _SIGNATURE_H

/*! \file
 *
 * This file contains the declarations to verify the signatures for the incoming post request.
 */

/* public key */
static const unsigned char pubkey[65] = {
  0x04, 0xB9, 0xE9, 0xE9, 0x09, 0x29, 0xF4, 0xFD, 0x40, 0xF8,
  0x38, 0x85, 0x03, 0x6D, 0x22, 0xEF, 0x8D, 0x7C, 0xDB, 0xA8,
  0xBA, 0x62, 0x2C, 0xBA, 0x1D, 0x1B, 0xB7, 0x4C, 0x55, 0x3A,
  0x4D, 0xA4, 0x07, 0xB4, 0xA0, 0xD0, 0x35, 0xB5, 0x85, 0x9F,
  0x15, 0x0A, 0x6B, 0x00, 0x9C, 0xC5, 0x1C, 0xAC, 0xE3, 0xB5,
  0x5B, 0x9C, 0x38, 0x91, 0x91, 0x11, 0x33, 0x1D, 0xEF, 0x7A,
  0x98, 0xC7, 0xE9, 0xD3, 0x6C
};

/**
 * @brief function to verify the incoming post data
 * 
 * the signature is the last TLV of the incoming data.
 * hence this data will be removed from the data
 * @param data incoming request (on POST) data
 * @param datalen size of the request data
 * @param sig   the signature to check
 * @param siglen the size of the signature
 * @return true 
 * @return false 
 */
bool signature_verify(const void *data, size_t datalen, const void *sig, size_t siglen);


#endif
