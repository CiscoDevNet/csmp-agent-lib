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

/** maximum CSMP cert length */
#define MAX_SIGNATURE_CERT_LENGTH 512

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
  FIRMWARE_IMAGE_INFO_ID = 75, /**< firmware info request */
  SIGNATURE_SETTINGS_ID = 79, /**< signature settings request */
  VENDOR_TLV_ID = 127 /**< vendor tlv */
} tlv_type_t;

/**
 * csmp_cert
 *
 * csmp certificate
 */
typedef struct csmp_sig_cert{
    size_t len;
    uint8_t data[MAX_SIGNATURE_CERT_LENGTH];
}csmp_cert;

/**
 * csmp_cfg_t
 *
 * csmp signature settings configuration
 */
typedef struct csmp_sig_cfg{
    bool reqsignedpost;
    bool reqvalidcheckpost;
    bool reqtimesyncpost;
    bool reqseclocalpost;
    bool reqsignedresp;
    bool reqvalidcheckresp;
    bool reqtimesyncresp;
    bool reqseclocalresp;
    csmp_cert cert;
} csmp_cfg_t;

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
  csmp_cfg_t csmp_sig_settings;/**< the csmp signature settings data*/
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

  uint32_t nms_errors; /**< nms error */
  uint32_t sig_no_sync; /**< get time error*/

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
