#ifndef _SAMPLE_UTIL_H_
#define _SAMPLE_UTIL_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "csmp_service.h"

extern dev_config_t g_devconfig;
extern csmp_handle_t g_csmp_handle;
extern uint32_t g_init_time;

int str2addr(char *str, uint8_t *addr);

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


#endif // _SAMPLE_UTIL_H_