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

#ifndef __COAPSERVER_H
#define __COAPSERVER_H

/*! \file
 *
 * This is a Server side implementation of a CoAP stack.
 *
 * This file defines the public API for the `coap` server support library.
 *
 * \page overview CoAP server
 * The CoAP server implements https://tools.ietf.org/html/rfc7252
 * The server is started on server port as input argument of the coapserver_listen() function
 * The incoming request is send to the registered callback.
 * Replies to a POST function can be send via the coapserver_response() function.
 * The server can be stopped by calling the coapserver_stop() function.
 *
 * The CoAP server Implements:
 * - Confirmable messages
 * - Non confirmable messages
 * - option headers
 * - tokens
 *
 */

#include "coap.h"
#include "osal.h"

/**
 * @brief callback handler for receiving CoAP commands
 *
 * @param from The address to send the CoAP message
 * @param tx_id The CoAP identifier
 * @param token_length The length of the CoAP identifier
 * @param token  The CoAP token
 * @param method The CoAP method of the incoming data
 * @param url The CoAP option URL
 * @param url_cnt Number of URL segments
 * @param query The CoAP option query of the incoming data
 * @param url_cnt Number of query segments
 * @param body The body of the message
 * @param body_len The length of the body message
 */
typedef void (*recv_handler_t)(struct sockaddr_in6 *from,
                   coap_transaction_type_t tx_type,
                   uint16_t tx_id,
                   uint8_t token_length,
                   uint8_t *token,
                   coap_method_t method,
                   const coap_uri_seg_t *url,
                   uint32_t url_cnt,
                   const coap_uri_seg_t *query,
                   uint32_t query_cnt,
                   const void *body,
                   uint16_t body_len );

/**
 * @brief starts listening to the UDP server port
 *
 * @param sport The server port
 * @param recv_handler The handler to handle incoming traffic.
 * @return int The return value is 0 on success and -1 on failure.
 */
int coapserver_listen(uint16_t sport, recv_handler_t recv_handler);

/**
 * @brief stops the CoAP server
 *
 * @return int The return value is 0 on success and -1 on failure.
 */
int coapserver_stop();

/**
 * @brief send a CoAP message
 *
 * @param to The address to send the CoAP message
 * @param tx_type Ehe CoAP message type
 * @param tx_id The CoAP identifier
 * @param token_length The length of the CoAP identifier
 * @param token  The CoAP token
 * @param status The (return) status
 * @param body The body of the message
 * @param body_len The length of the body message
 * @return int The return value is 0 on success and -1 on failure.
 */
int coapserver_response(const struct sockaddr_in6 *to,
    coap_transaction_type_t tx_type,
    uint16_t tx_id,
    uint8_t token_length, uint8_t *token,
    uint16_t status,
    const void* body, uint16_t body_len);

#endif
