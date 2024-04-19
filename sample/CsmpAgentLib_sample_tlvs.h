#ifndef _SAMPLE_TLVS_H_
#define _SAMPLE_TLVS_H_
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "csmp_service.h"
#include "csmp_info.h"
#include "CsmpAgentLib_sample_util.h"

#define CSMP_NON_VENDOR_ID 0
#define VENDOR_DATA_LEN 64

extern dev_config_t g_devconfig;
extern csmp_handle_t g_csmp_handle;
extern uint32_t g_init_time;

/**
 * @brief pubkey_get (portable)
 *
 * @param key pointer to pubkey
 */
void pubkey_get(char *key);

/**
 * @brief hardware description function (portable)
 *
 * @param num amount of instances (array index) of hardware Descriptions
 * @return void* pointer to global variable g_hardwareDesc
 */
void* hardware_desc_get(uint32_t *num);

/**
 * @brief interface description function (portable)
 *
 * @param num  amount of instances (array index) of interface Descriptions
 * @return void* pointer to global variable g_interfaceDesc
 */
void* interface_desc_get(uint32_t *num);

/**
 * @brief function to set the ip Addresses. (portable)
 *
 * @param num amount of instances (array index) of ipAddresses
 * @return void* pointer to global variable g_ipAddress
 */
void* ipaddress_get(uint32_t *num);

/**
 * @brief set up ip Route information (portable)
 *
 * @param num amount of instances of g_ipRoute
 * @return void* pointer to g_ipRoute
 */
void* iproute_get(uint32_t *num);

/**
 * @brief set up the current time (portable)
 *
 * @param num amount of instances
 * @return void* pointer to g_currentTime
 */
void* currenttime_get(uint32_t *num);


/**
 * @brief set the current time (portable)
 *
 * @param tlv
 */
void currenttime_post(Current_Time *tlv);

/**
 * @brief get the vendor specific data (portable)
 *
 * @param num amount of instances
 * @return void* pointer to g_VendorData
 */
void* vendorspecificdata_get(tlvid_t tlvid, uint32_t *num);


/**
 * @brief set the vendor specific data (portable)
 *
 * @param tlv
 */
void vendorspecificdata_post(tlvid_t tlvid, Vendor_Specific *tlv);

/**
 * @brief Get the uptime
 *
 * @param num amount of instances in g_uptime (portable)
 * @return void* point to g_uptime
 */
void* uptime_get(uint32_t *num);

/**
 * @brief interface metrics information setup (portable)
 *
 * @param num num mount of instances of g_interfaceMetrics
 * @return void* pointer to global g_interfaceMetrics
 */
void* interface_metrics_get(uint32_t *num);

/**
 * @brief iproute RPL information setup (portable)
 *
 * @param num num mount of instances of g_iprouteRplmetrics
 * @return void* pointer to global g_iprouteRplmetrics
 */
void* iproute_rplmetrics_get(uint32_t *num);


/**
 * @brief wpan information setup (portable)
 *
 * @param num num mount of instances of g_wpanStatus
 * @return void* pointer to global g_wpanStatus
 */
void* wpanstatus_get(uint32_t *num);


/**
 * @brief RPL instance information (portable)
 *
 * @param num mount of instances of g_rplInstance
 * @return void* pointer to global g_rplInstance
 */
void* rplinstance_get(uint32_t *num);

/**
 * @brief firmware information (portable)
 *
 * @param num amount of instances of g_firmwareImageInfo
 * @return void* pointer to global g_firmwareImageInfo
 */
void* firmware_image_info_get(uint32_t *num);


/**
 * @brief get up the signature settings (portable)
 *
 * @param num amount of instances
 * @return void* pointer to global g_SignatureSettings
 */
void* signature_settings_get(uint32_t *num);

/**
 * @brief set the signature settings (portable)
 *
 * @param tlv
 */
void signature_settings_post(Signature_Settings *tlv);


/**
 * @brief csmp get TLV request
 *
 * @param tlvid the tlvid to handle
 * @param num returned amount of instances
 * @return void* pointer to the global variable containing the return data
 */
void* csmptlvs_get(tlvid_t tlvid, uint32_t *num);

/**
 * @brief csmp post TLV request
 *
 * @param tlvid the tlvid to handle
 * @param tlv the request data
 */
void csmptlvs_post(tlvid_t tlvid, void *tlv);

#endif // _SAMPLE_TLVS_H_