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

#define PUBLIC_KEY_LEN  65

/*! \file
 *
 * This file contains the declarations to verify the signatures for the incoming post request.
 */

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
