#include "CsmpAgentLib_sample_tlvs.h"


/**
 * @brief csmp get TLV request
 *
 * @param tlvid the tlvid to handle
 * @param num returned amount of instances
 * @return void* pointer to the global variable containing the return data
 */
void* csmptlvs_get(tlvid_t tlvid, uint32_t *num) {
  if (tlvid.vendor == CSMP_NON_VENDOR_ID)
  {
    switch(tlvid.type) {
    case HARDWARE_DESC_ID:
      return hardware_desc_get(num);
    case INTERFACE_DESC_ID:
      return interface_desc_get(num);
    case IPADDRESS_ID:
      return ipaddress_get(num);
    case IPROUTE_ID:
      return iproute_get(num);
    case CURRENT_TIME_ID:
      return currenttime_get(num);
    case UPTIME_ID:
      return uptime_get(num);
    case INTERFACE_METRICS_ID:
      return interface_metrics_get(num);
    case IPROUTE_RPLMETRICS_ID:
      return iproute_rplmetrics_get(num);
    case WPANSTATUS_ID:
      return wpanstatus_get(num);
    case RPLINSTANCE_ID:
      return rplinstance_get(num);
    case FIRMWARE_IMAGE_INFO_ID:
      return firmware_image_info_get(num);
    case SIGNATURE_SETTINGS_ID:
      return signature_settings_get(num);

    default:
      break;
    }
  }
  else
  {
    return vendorspecificdata_get(tlvid, num);
  }
  return NULL;
}

/**
 * @brief csmp post TLV request
 *
 * @param tlvid the tlvid to handle
 * @param tlv the request data
 */
void csmptlvs_post(tlvid_t tlvid, void *tlv) {
  if (tlvid.vendor == CSMP_NON_VENDOR_ID)
  {
    switch(tlvid.type) {
    case CURRENT_TIME_ID:
      currenttime_post((Current_Time*)tlv);
      break;
    case SIGNATURE_SETTINGS_ID:
      signature_settings_post((Signature_Settings*)tlv);
      break;
    default:
      break;
    }
  }
  else
  {
    vendorspecificdata_post(tlvid, (Vendor_Specific*)tlv);
  }
}