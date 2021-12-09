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

#ifndef _CSMP_SERVICE_H
#define _CSMP_SERVICE_H

/*! \file
 *
 * CSMP service
 *
 * Generic CSMP server code
 * callbacks to be registered for GET and POST & signature verification functions.
 * function to set the device characteristics.
 */


#include <stdint.h>
#include <stdbool.h>
#include <netinet/in.h>

/** tlvs list
 *
 */
typedef enum {
  HARDWARE_DESC_ID = 11,  /**< Hardware description request */
  INTERFACE_DESC_ID = 12, /**< interface description request */
  IPADDRESS_ID = 16,  /**< ip address request */
  IPROUTE_ID = 17, /**< ip route request */
  CURRENT_TIME_ID = 18, /**< current time request */
  UPTIME_ID = 22, /**< uptime request */
  INTERFACE_METRICS_ID = 23, /**< interface metrics request */
  IPROUTE_RPLMETRICS_ID = 25, /**< rpl metrics request */
  WPANSTATUS_ID = 35, /**< wpan status request */
  RPLINSTANCE_ID = 53, /**< rpl instance info request */
  FIRMWARE_IMAGE_INFO_ID = 75 /**< firmware info request */
} tlv_type_t;

/**
 * @brief device configuration
 *
 */
typedef struct _dev_config {
  struct in6_addr NMSaddr;     /**< the NMS’s IPv6 address */
  struct {
    uint8_t data[8];  //**< bytes */
  } ieee_eui64;          /**< the device’s eui64, should be the same with the EID that imported on NMS*/
  uint32_t reginterval_min;  /**< the minimum interval to send register message to NMS*/
  uint32_t reginterval_max;  /**< the maximum interval to send register message to NMS*/
} dev_config_t;

/**
 * @brief tlvid
 *
 */
typedef struct _tlvid {
  uint32_t vendor; /**< vendor identification */
  uint32_t type;   /**< type identification */
} tlvid_t;

/**
 * @brief GET function definition
 *
 * @param tlvid the URL being called
 * @param num
 */
typedef void* (* csmptlvs_get_t)(tlvid_t tlvid, uint32_t *num);

/**
 * @brief POST function definition
 *
 * @param tlvid the URL being called
 * @param tlv
 */
typedef void (* csmptlvs_post_t)(tlvid_t tlvid, void *tlv);

/**
 * @brief signature verification function definition
 *
 * @param data the data to be checked
 * @param datalen the length of the data
 * @param sig the signature
 * @param siglen the signature length
 */
typedef bool (* signature_verify_t)(const void *data, size_t datalen, const void *sig, size_t siglen);

/**
 * @brief the handles for the installed functions
 *
 */
typedef struct _csmp_handle {
  csmptlvs_get_t csmptlvs_get;   /**< GET function */
  csmptlvs_post_t csmptlvs_post;  /**< POST function */
  signature_verify_t signature_verify; /**< signature check function */
} csmp_handle_t;

/**
 * @brief service status
 *
 */
typedef enum {
  SERVICE_NOT_START,    /**< not started */
  SERVICE_START_FAILURE, /**< start failure */
  REGISTRATION_IN_PROGRESS, /**< registering */
  REGISTRATION_SUCCESS    /**< registering success full, e.g. active */
} csmp_service_status_t;

/** csmp_service_stats_t
 */
typedef struct {
  uint32_t reg_succeed; /**< registration status */
  uint32_t reg_attempts; /**< number of registration attemps */
  uint32_t reg_fails;  /**< registration failure */
  struct {
    uint32_t error_coap;    /**< CoAP error */
    uint32_t error_signature; /**< signature check failure */
    uint32_t error_process; /**< overall error */
  } reg_fails_stats; /**< failure stats */
  uint32_t metrics_reports; /**< metric reports */

  uint32_t csmp_get_succeed; /**< CoAP GET successfull */
  uint32_t csmp_post_succeed;/**< CoAP POST successfull */

  uint32_t sig_ok; /**< signature status */
  uint32_t sig_no_signature; /**< no signature needed */
  uint32_t sig_bad_auth;  /**< signature failure on authorisation */
  uint32_t sig_bad_validity; /**< signature failure on time check */
} csmp_service_stats_t;

/**
 * @brief start the csmp server
 *
 * @param devconfig the device configuration
 * @param csmp_handle the handle
 * @return int 0 is success
 */
int csmp_service_start(dev_config_t *devconfig, csmp_handle_t *csmp_handle);

/**
 * @brief update the device configuration
 *
 * @param devconfig the device configuration
 * @return true
 * @return false
 */
bool csmp_devconfig_update(dev_config_t *devconfig);

/**
 * @brief retrieve the service status
 *
 * @return csmp_service_status_t
 */
csmp_service_status_t csmp_service_status();

/**
 * @brief retrieve the service statistics
 *
 * @return csmp_service_stats_t*
 */
csmp_service_stats_t* csmp_service_stats();

/**
 * @brief stop the csmp service
 *
 * @return true
 * @return false
 */
bool csmp_service_stop();

#endif
