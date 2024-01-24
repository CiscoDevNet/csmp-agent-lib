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

#ifndef __COAPCLIENT_H
#define __COAPCLIENT_H

#include "coap.h"
#include "osal_common.h"

/*! \file
 *
 * This file is the Client side of a CoAP stack.
 *
 *
 * This file defines the public API for the `coap` client part of the CoAP support library.
 */


/**
 * @brief response handler definition
 *
 * @param from return address
 * @param status status of the response
 * @param body body of the response
 * @param body_len length of the respons
 *
 */
typedef void (*response_handler_t)(struct sockaddr_in6 *from,
		       uint16_t status,
		       const void *body, uint16_t body_len);

/**
 * @brief function to install the respons handler for the request
 *
 * @param response_handler
 * @return int The return value is 0 on success and -1 on failure.
 */
int coapclient_open(response_handler_t response_handler);

int coapclient_stop();

/**
 * @brief make a request
 *
 * @param to address to send the request to
 * @param tx_type connection type
 * @param method CoAP method
 * @param token_length  the token length
 * @param token the token
 * @param url url segments to be used
 * @param url_cnt url segment count
 * @param query query segments to be used
 * @param query_cnt query segments count
 * @param body request body
 * @param body_len request body length
 * @return int The return value is 0 on success and -1 on failure.
 */
int coapclient_request(const osal_sockaddr *to,
		coap_transaction_type_t tx_type,
		coap_method_t method,
		uint8_t token_length, uint8_t *token,
		const coap_uri_seg_t *url, uint32_t url_cnt,
		const coap_uri_seg_t *query, uint32_t query_cnt,
		const void *body, uint16_t body_len);

#endif
