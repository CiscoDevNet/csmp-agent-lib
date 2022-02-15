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

#ifndef _COAP_H_
#define _COAP_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "debug.h"

/*! \file
 *
 * This is a C implementation of a CoAP stack.
 *
 * This file defines the public API for the `coap` support library.
 */


/**
 *  payload marker
 */
enum {
  COAP_PAYLOAD_MARKER = 0xff
};

/**
 *  max TKL
 */
enum {
  COAP_MAX_TKL = 8
};

/**
 *  coap headers
 */
typedef struct {
  uint8_t control;  /**< control identification */
  uint8_t code;     /**< code */
  uint16_t message_id; /**< message id */
}__attribute__((packed)) coap_header_t;

/**
  * coap_methods_t
  * The coap methods as defined by the RFC
  */
typedef enum {
  COAP_GET = 1,   /**< GET*/
  COAP_POST = 2,  /**< POST*/
  COAP_PUT = 3,   /**< PUT*/
  COAP_DELETE = 4 /**< DELETE */
} coap_method_t;

/**
  * coap_option_t
  * The coap options as defined by the RFC
  */
typedef enum {
  COAP_IF_MATCH = 1,  /**< If-Match*/
  COAP_URI_HOST = 3,  /**< URI host */
  COAP_ETAG = 4,      /**< etag */
  COAP_IF_NONE_MATCH = 5,  /**< if none match */
  COAP_URI_PORT = 7,   /**< URI port */
  COAP_LOCATION_PATH = 8, /**< location path */
  COAP_URI_PATH = 11,  /**< URI path */
  COAP_CONTENT_FORMAT = 12, /**< content format */
  COAP_MAX_AGE = 14,   /**< max age */
  COAP_URI_QUERY = 15, /**< URI query */
  COAP_ACCEPT = 17,  /**< accept */
  COAP_LOCATION_QUERY = 20, /**< location query */
  COAP_BLOCK2 = 23,  /**< block 2 */
  COAP_BLOCK1 = 27,  /**< block 1 */
  COAP_SIZE2 = 28,   /**< size 2 */
  COAP_PROXY_URI = 35,  /**< proxy URI */
  COAP_PROXY_SCHEME = 39, /**< proxy scheme */
  COAP_SIZEL = 60  /**< size L */
} coap_option_t;

/**
  * coap_transaction_type_t
  * The coap transaction types
  */
typedef enum {
  COAP_CON = 0, /**< CON - confirmable request*/
  COAP_NON = 1, /**< NON - non confirmable request */
  COAP_ACK = 2, /**< ACK - acknowledge */
  COAP_RST = 3  /**< RST - reset */
} coap_transaction_type_t;


/**
  * coap_uri_seg_t
  * CoAP URL option
  */
typedef struct {
  uint32_t len; /**< length of the value */
  uint8_t *val; /**< value (url) */
} coap_uri_seg_t;

/**
  * coap_code_t
  * CoAP status codes
  */
typedef enum {
  COAP_CODE_OK = 200,  /**< 2.00 OK */
  COAP_CODE_CREATED = 201,  /**< 2.01 Created */
  COAP_CODE_DELETED = 202,  /**< 2.00 deleted */
  COAP_CODE_VALID = 203,  /**< 2.03 Valid */
  COAP_CODE_CHANDED = 204,  /**< 2.04 Changed */
  COAP_CODE_CONTENT = 205,  /**< 2.05 Content */
  COAP_CODE_BAD_REQ = 400,  /**< 4.00 Bad Request */
  COAP_CODE_UNAUTHORIZED = 401,  /**< 4.01 Unauthorized */
  COAP_CODE_FORBIDDEN = 403,  /**< 4.03 Forbidden */
  COAP_CODE_NOT_FOUND = 404,  /**< 4.04 Not Found */
  COAP_CODE_METHOD_NOT_ALLOWED = 405,  /**< 4.05 Method Not Allowed */
  COAP_CODE_INTERNAL_SERVER_ERROR = 500,  /**< 5.00 Internal Server Error */
  COAP_CODE_NOT_IMPLEMENTED = 501,  /**< 5.01 Not Implemented */
  COAP_CODE_SERVICE_UNAVAILABLE = 503,  /**< 5.03 Service Unavailable */
  COAP_CODE_GATEWAY_TIMEOUT = 504,  /**< 5.04 Gateway Timeout */
} coap_code_t;

/**
 * response codes are encoded to base 32, i.e.the three upper bits determine
 * the response class while the remaining five fine-grained information specific to that class.
 */
#define COAP_RESPONSE_CODE(N) (((N)/100 << 5) | (N)%100)

/**
 * Determines the class of response code C
 */
#define COAP_RESPONSE_CLASS(C) (((C) >> 5) & 0xFF)

#endif
