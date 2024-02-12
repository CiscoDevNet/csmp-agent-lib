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

#ifndef _CSMPSERVICE_H
#define _CSMPSERVICE_H

#include <netinet/in.h>
#include "csmp.h"

/*! \file
 *
 * CSMP service
 *
 * Definitions of the CSMP server.
 * includes the registration part, with sending back the metrics on a regular interval.
 */

/** tlvs enumeration
 */
typedef enum {
  HARDWARE_DESC_ID = 11,   /**< Hardware description */
  INTERFACE_DESC_ID = 12,  /**< Interface description */
  IPADDRESS_ID = 16,        /**< Ip address info */
  IPROUTE_ID = 17,        /**< ip route info */
  CURRENT_TIME_ID = 18,   /**< current time */
  UPTIME_ID = 22,         /**< up time */
  INTERFACE_METRICS_ID = 23,  /**< interface metrics info */
  IPROUTE_RPLMETRICS_ID = 25, /**< ip route rpl info */
  WPANSTATUS_ID = 35,       /**< wan status info */
  NEIGHBOR802154_G_ID = 52, /**< neighbor info */
  RPLINSTANCE_ID = 53,    /**< rpl instance info */
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
 * dev_config_t
 *
 * device configuration
 */
typedef struct dev_config {
  struct in6_addr NMSaddr;     /**< the NMS’s IPv6 address */
  struct {
    uint8_t data[8];
  } ieee_eui64;          /**< the device’s eui64, should be the same with the EID that imported on NMS */
  uint32_t reginterval_min;  /**< the minimum interval to send register message to NMS */
  uint32_t reginterval_max;  /**< the maximum interval to send register message to NMS */
  csmp_cfg_t csmp_sig_settings;/**< the csmp signature settings data*/
} dev_config_t;

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
 * csmp_handle_t
 *
 * handle
 */
typedef struct _csmp_handle {
  csmptlvs_get_t csmptlvs_get;
  csmptlvs_post_t csmptlvs_post;
  signature_verify_t signature_verify;
} csmp_handle_t;

/**
 * csmp_service_status_t
 *
 * service status
 */
typedef enum {
  SERVICE_NOT_START,     /**< not started */
  SERVICE_START_FAILURE,  /**< failed to start */
  REGISTRATION_IN_PROGRESS, /**< registration in progress */
  REGISTRATION_SUCCESS  /**< registration is successful */
} csmp_service_status_t;

/**
 * @brief service statistics
 *
 */
typedef struct {
  uint32_t reg_succeed;  /**< registration status */
  uint32_t reg_attempts; /**< number of registration attemps */
  uint32_t reg_fails; /**< registration failure */
  struct {
    uint32_t error_coap;/**< CoAP error */
    uint32_t error_signature;/**< signature check failure */
    uint32_t error_process;/**< overall error */
  } reg_fails_stats; /**< failure statistics */
  uint32_t metrics_reports;/**< metric reports */

  uint32_t nms_errors; /**< nms error */
  uint32_t sig_no_sync; /**< get time error*/

  uint32_t csmp_get_succeed;/**< CoAP GET successfull */
  uint32_t csmp_post_succeed;/**< CoAP POST successfull */

  uint32_t sig_ok; /**< signature status */
  uint32_t sig_no_signature; /**< no signature needed */
  uint32_t sig_bad_auth; /**< signature failure on authorisation */
  uint32_t sig_bad_validity; /**< signature failure on time check */
} csmp_service_stats_t;

/**
 * @brief start service
 *
 * @param devconfig the device configuration
 * @param csmp_handle the service handle
 * @return int
 */
int csmp_service_start(dev_config_t *devconfig, csmp_handle_t *csmp_handle);

/**
 * @brief the device configuration update
 *
 * @param devconfig device configuration
 * @return true
 * @return false
 */
bool csmp_devconfig_update(dev_config_t *devconfig);

/**
 * @brief retrieve service status
 *
 * @return csmp_service_status_t service status
 */
csmp_service_status_t csmp_service_status();

/**
 * @brief retrive service statistics
 *
 * @return csmp_service_stats_t* service statistics
 */
csmp_service_stats_t* csmp_service_stats();

/**
 * @brief stop service
 *
 * @return true
 * @return false
 */
bool csmp_service_stop();

/**
 * @brief externally defined stats
 *
 */
extern csmp_service_stats_t g_csmplib_stats;

/**
 * @brief externally defined get function
 *
 */
extern csmptlvs_get_t g_csmptlvs_get;

/**
 * @brief externally defined post function
 *
 */
extern csmptlvs_post_t g_csmptlvs_post;

/**
 * @brief externally signature verification function
 *
 */
extern signature_verify_t g_csmplib_signature_verify;

#endif
