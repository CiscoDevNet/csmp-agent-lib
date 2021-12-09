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
