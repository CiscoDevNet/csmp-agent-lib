/*
 *  Copyright 2026 Renesas Electronics Corporation
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

/**
 * @file  renesas_wisun_tlvs.c
 * @brief Renesas implementation of the individual CSMP get and post TLV requests.
 *
 * @note This file maps Renesas Wi-SUN FAN stack and firmware update library data into CSMP TLVs. See
 * Vendors/Renesas/Readme.md for the Renesas library requirements and function usage summary.
 */


#include <stdio.h>
#include <string.h>

#include "r_nwk_api.h"
#include "r_impl_utils.h"
#include "r_mem_tools.h"
#include "r_app_config.h"

#include "osal_renesas.h"

#include "csmp_service.h"
#include "csmp_info.h"

#include "signature_verify.h"
#if R_FWUP_SUPPORT
#include "r_flash_wrapper.h"
#include "r_fwup_wrapper.h"
#if !defined(__i386) && !defined(__x86_64)
#include "r_fwup_private.h"
#include "r_fwup_wrap_flash.h"
#include "r_fwup_config.h"
#endif
#endif

// R_LOG_PREFIX is processed by RLog code generator
#define R_LOG_PREFIX CSMP
#include "r_log_internal.h"
#if R_LOG_THRESHOLD > R_LOG_SEVERITY_OFF
#include "r_loggen_renesas_wisun_tlvs.h"
#endif

/** The hardware information */
Hardware_Desc g_tlv_hardwareDesc = HARDWARE_DESC_INIT;

/** The interface information */
Interface_Desc g_tlv_interfaceDesc = INTERFACE_DESC_INIT;

/** The ipaddress information */
IP_Address g_tlv_ipAddress[R_TLV_IPADDRESS_MAX_NUM] = {IPADDRESS_INIT};

/** The ip route information */
IP_Route g_tlv_ipRoute[R_TLV_ROUTES_MAX_NUM] = {IPROUTE_INIT};

/** The current time information */
Current_Time g_tlv_currentTime = CURRENT_TIME_INIT;

/** The up time information */
Up_Time g_tlv_upTime = UPTIME_INIT;

/** The interface metrics data */
Interface_Metrics g_tlv_interfaceMetrics = INTERFACE_METRICS_INIT;

/** The ip route rpl metrics data */
IPRoute_RPLMetrics g_tlv_iprouteRplmetrics[R_TLV_ROUTES_MAX_NUM] = {IPROUTE_RPLMETRICS_INIT};

/** The wpan status data */
WPAN_Status g_tlv_wpanStatus = WPANSTATUS_INIT;

/** The rpl data */
RPL_Instance g_tlv_rplInstance = RPLINSTANCE_INIT;

/** The signature settings data */
Signature_Settings g_SignatureSettings = SIGNATURE_SETTINGS_INIT;

/** The transfer request data */
Transfer_Request g_transferRequest = TRANSFER_REQUEST_INIT;

/** The image block data */
Image_Block g_imageBlock = IMAGE_BLOCK_INIT;

/** The load request data */
Load_Request g_loadRequest = LOAD_REQUEST_INIT;

/** The cancel load request data */
Cancel_Load_Request g_cancelLoadRequest = CANCEL_LOAD_REQUEST_INIT;

/** The firmware info data */
Firmware_Image_Info g_firmwareImageInfo[CSMP_FWMGMT_ACTIVE_SLOTS - 1] = {FIRMWARE_IMAGE_INFO_INIT};

bool g_downloadbusy  = false;  // Track ongoing download

#if R_FWUP_SUPPORT
static const char baseHwId[] = R_CSMP_HARDWARE_ID;
static const char baseFileName[] = R_CSMP_FILE_NAME;
static const char baseCsmpVersion[] = R_CSMP_VERSION_STRING;
static const char baseLibraryVersion[] = R_NWK_BASE_VERSION_STRING;
static bool initxfer        = false;  // Track transfer request
static bool initload        = false;  // Track load request
static uint32_t curloadtime = 0;      // Track current loadtime

static r_boolean_t uploadSlotValid = R_FALSE;
extern r_fwup_state_t fwupState;
/** The information about the firmware image being uploaded */
static Csmp_Slothdr uploadSlotHdr = CSMP_SLOTHDR_INIT;

/** The file hash is either read from non-volatile memory or from the running firmware header */
static uint8_t runImageFileHash[SHA256_HASH_SIZE] = { 0 };
/** The file name is either read from non-volatile memory or computed from the app name and the file hash */
static char runImageFileName[FILE_NAME_SIZE] = "";
/**
 * The file size is read from non-volatile memory. It is valid only after switching to an image transferred by FND;
 * otherwise it should be 0.
 */
static uint32_t runImageFileSize = 0;
#endif

extern const uint8_t* AppIpGetMacAddr(void);

typedef union
{
    r_nwk_nd_cache_t nd_cache;
    uint8_t          placeholder[sizeof(r_nwk_nd_cache_t) + R_NWK_MAX_NEIGHBORS_PER_CONFIRM * sizeof(r_nwk_nd_cache_entry_t)];
} nd_cache_buffer_t;

/** public key */
static const char pubkey[PUBLIC_KEY_LEN] = {
  0x04, 0x23, 0xD2, 0x83, 0x45, 0xE8, 0xD5, 0xDF, 0x86, 0x9D,
  0x6E, 0xE7, 0x58, 0x0D, 0xC1, 0x8F, 0x35, 0x9D, 0x57, 0xB1,
  0x3D, 0x50, 0x4A, 0x16, 0x01, 0x15, 0xC4, 0x81, 0x19, 0xB0,
  0xE6, 0x60, 0xB8, 0x64, 0x14, 0x01, 0x5D, 0x56, 0x83, 0xBE,
  0xE1, 0x85, 0x98, 0xCB, 0x90, 0xE1, 0xF7, 0x9B, 0xF4, 0x33,
  0x5A, 0x4B, 0x29, 0xAD, 0x35, 0x69, 0x9B, 0x4F, 0xDC, 0x42,
  0x7F, 0xEB, 0xC2, 0x99, 0xA5
};

void pubkey_get(const char** key, size_t len)
{
    if (key && (len == sizeof(pubkey)))
    {
        *key = pubkey;
    }
}

void* hardware_desc_get(uint32_t* num)
{
    *num = 1;
    MEMZERO_S(&g_tlv_hardwareDesc);

    g_tlv_hardwareDesc.has_entphysicalindex = true;
    g_tlv_hardwareDesc.has_entphysicaldescr = true;
    g_tlv_hardwareDesc.has_entphysicalclass = true;
    g_tlv_hardwareDesc.has_entphysicalname = true;
    g_tlv_hardwareDesc.has_entphysicalhardwarerev = true;
    g_tlv_hardwareDesc.has_entphysicalfirmwarerev = true;
    g_tlv_hardwareDesc.has_entphysicalsoftwarerev = true;
    g_tlv_hardwareDesc.has_entphysicalserialnum = true;
    g_tlv_hardwareDesc.has_entphysicalmfgname = true;
    g_tlv_hardwareDesc.has_entphysicalmodelname = true;
    g_tlv_hardwareDesc.has_entphysicalmfgdate = true;
    g_tlv_hardwareDesc.has_entphysicalfunction = true;
    g_tlv_hardwareDesc.has_entphysicaloui = true;

    g_tlv_hardwareDesc.entphysicalindex = 1;
    sprintf(g_tlv_hardwareDesc.entphysicaldescr, "Renesas Wi-SUN FAN CSMP Agent");
    g_tlv_hardwareDesc.entphysicalclass = CLASS_MODULE;
    sprintf(g_tlv_hardwareDesc.entphysicalname, "lowpan");  // Other values ("Wi-SUN FAN Node") seem to conflict with the server
    sprintf(g_tlv_hardwareDesc.entphysicalhardwarerev, "1.0");
    sprintf(g_tlv_hardwareDesc.entphysicalfirmwarerev, R_CSMP_VERSION_STRING);
    const char* appName = R_APP_GetAppName();
    strncpy(g_tlv_hardwareDesc.entphysicalsoftwarerev, appName, sizeof(g_tlv_hardwareDesc.entphysicalsoftwarerev));
    g_tlv_hardwareDesc.entphysicalsoftwarerev[sizeof(g_tlv_hardwareDesc.entphysicalsoftwarerev) - 1] = '\0';
    const uint8_t* p_macAddr = AppIpGetMacAddr();
    snprintf(g_tlv_hardwareDesc.entphysicalserialnum, sizeof(g_tlv_hardwareDesc.entphysicalserialnum),
             "%02X%02X%02X%02X%02X%02X%02X%02X",
             p_macAddr[0], p_macAddr[1], p_macAddr[2], p_macAddr[3],
             p_macAddr[4], p_macAddr[5], p_macAddr[6], p_macAddr[7]);
    snprintf(g_tlv_hardwareDesc.entphysicaloui, sizeof(g_tlv_hardwareDesc.entphysicaloui),
             "%02X%02X%02X",
             p_macAddr[0], p_macAddr[1], p_macAddr[2]);
    sprintf(g_tlv_hardwareDesc.entphysicalmfgname, "Renesas Electronics");
    const char* hardwareName = R_APP_GetHardwareName();
    strncpy(g_tlv_hardwareDesc.entphysicalmodelname, hardwareName, sizeof(g_tlv_hardwareDesc.entphysicalmodelname));
    g_tlv_hardwareDesc.entphysicalmodelname[sizeof(g_tlv_hardwareDesc.entphysicalmodelname) - 1] = '\0';
    g_tlv_hardwareDesc.entphysicalmfgdate = R_APP_RELEASE_DATE;
    g_tlv_hardwareDesc.entphysicalfunction = 1;

    return &g_tlv_hardwareDesc;
}

void* interface_desc_get(uint32_t* num)
{
    *num = 1;
    MEMZERO_S(&g_tlv_interfaceDesc);

    g_tlv_interfaceDesc.has_ifindex = true;
    g_tlv_interfaceDesc.has_ifname = true;
    g_tlv_interfaceDesc.has_ifdescr = true;
    g_tlv_interfaceDesc.has_iftype = true;
    g_tlv_interfaceDesc.has_ifmtu = true;
    g_tlv_interfaceDesc.has_ifphysaddress = true;

    g_tlv_interfaceDesc.ifindex = 1;
    sprintf(g_tlv_interfaceDesc.ifname, "lowpan");
    sprintf(g_tlv_interfaceDesc.ifdescr, "ieee802154");
    g_tlv_interfaceDesc.iftype = 259;
    g_tlv_interfaceDesc.ifmtu = R_MAX_MTU_SIZE;
    const uint8_t* p_macAddr = AppIpGetMacAddr();
    g_tlv_interfaceDesc.ifphysaddress.len = 8;
    MEMCPY_A(g_tlv_interfaceDesc.ifphysaddress.data, p_macAddr);

    return &g_tlv_interfaceDesc;
}

void* ipaddress_get(uint32_t* num)
{
    uint32_t i = 0;
    uint8_t ipv6Addr[R_IPV6_ADDRESS_LENGTH];
    const uint8_t* p_macAddr = AppIpGetMacAddr();
    MEMZERO_A(g_tlv_ipAddress);

    g_tlv_ipAddress[i].ipaddressaddrtype = IPV6;
    g_tlv_ipAddress[i].ipaddressaddr.len = R_IPV6_ADDRESS_LENGTH;
    g_tlv_ipAddress[i].ipaddressaddr.data[0] = 0xfe;
    g_tlv_ipAddress[i].ipaddressaddr.data[1] = 0x80;
    R_memcpy(g_tlv_ipAddress[i].ipaddressaddr.data + 8, p_macAddr, 8);
    g_tlv_ipAddress[i].ipaddressaddr.data[8] ^= 0x02;
    g_tlv_ipAddress[i].ipaddressifindex = 1;
    g_tlv_ipAddress[i].ipaddresstype = UNICAST;
    g_tlv_ipAddress[i].ipaddressorigin = LINKLAYER;
    g_tlv_ipAddress[i].ipaddresspfxlen = R_IPV6_PREFIX_LEN * 8u;
    g_tlv_ipAddress[i].ipaddressindex = (int32_t)i + 1;
    i++;

    if (R_NWK_GetRequest(R_NWK_nwkIpv6Address, ipv6Addr, sizeof(ipv6Addr)) == R_RESULT_SUCCESS
        && MEMISNOTZERO_S(&ipv6Addr))
    {
        g_tlv_ipAddress[i].ipaddressaddrtype = IPV6;
        g_tlv_ipAddress[i].ipaddressaddr.len = R_IPV6_ADDRESS_LENGTH;
        MEMCPY_A(g_tlv_ipAddress[i].ipaddressaddr.data, ipv6Addr);
        g_tlv_ipAddress[i].ipaddressifindex = 1;
        g_tlv_ipAddress[i].ipaddresstype = UNICAST;
        g_tlv_ipAddress[i].ipaddressorigin = DHCP;
        g_tlv_ipAddress[i].ipaddresspfxlen = R_IPV6_PREFIX_LEN * 8u;
        g_tlv_ipAddress[i].ipaddressindex = (int32_t)i + 1;
        i++;
    }
    *num = i;
    for (i = 0; i < *num; i++)
    {
        g_tlv_ipAddress[i].has_ipaddressindex = true;
        g_tlv_ipAddress[i].has_ipaddressaddrtype = true;
        g_tlv_ipAddress[i].has_ipaddressaddr = true;
        g_tlv_ipAddress[i].has_ipaddressifindex = true;
        g_tlv_ipAddress[i].has_ipaddresstype = true;
        g_tlv_ipAddress[i].has_ipaddressorigin = true;
        g_tlv_ipAddress[i].has_ipaddresspfxlen = true;
    }

    if (*num == 0)
    {
        return NULL;
    }

    return &g_tlv_ipAddress;
}

void* iproute_get(uint32_t* num)
{
    uint32_t i = 0;
    r_ipv6addr_t apAddr;
    MEMZERO_A(g_tlv_ipRoute);

#if R_BORDER_ROUTER_ENABLED
    uint8_t deviceType;
    if (R_NWK_GetRequest(R_NWK_deviceType, &deviceType, sizeof(deviceType)) == R_RESULT_SUCCESS &&
        deviceType != R_BORDERROUTER)
#endif
    {
        if (R_NWK_GetRequest(R_NWK_preferredParentAddress, &apAddr, sizeof(apAddr)) == R_RESULT_SUCCESS
            && MEMISNOTZERO_S(&apAddr))
        {
            g_tlv_ipRoute[i].inetcidrroutedesttype = IPV6;
            g_tlv_ipRoute[i].inetcidrroutedest.len = R_IPV6_ADDRESS_LENGTH;
            MEMCPY_A(g_tlv_ipRoute[i].inetcidrroutedest.data, apAddr.bytes);
            g_tlv_ipRoute[i].inetcidrroutepfxlen = R_IPV6_PREFIX_LEN * 8u;
            g_tlv_ipRoute[i].inetcidrroutenexthoptype = IPV6;
            g_tlv_ipRoute[i].inetcidrroutenexthop.len = R_IPV6_ADDRESS_LENGTH;
            MEMCPY_A(g_tlv_ipRoute[i].inetcidrroutenexthop.data, apAddr.bytes);
            g_tlv_ipRoute[i].inetcidrrouteifindex = 1;
            g_tlv_ipRoute[i].inetcidrrouteindex = (int32_t)i + 1;
            i++;
        }
        if (R_NWK_GetRequest(R_NWK_alternateParentAddress, &apAddr, sizeof(apAddr)) == R_RESULT_SUCCESS
            && MEMISNOTZERO_S(&apAddr))
        {
            g_tlv_ipRoute[i].inetcidrroutedesttype = IPV6;
            g_tlv_ipRoute[i].inetcidrroutedest.len = R_IPV6_ADDRESS_LENGTH;
            MEMCPY_A(g_tlv_ipRoute[i].inetcidrroutedest.data, apAddr.bytes);
            g_tlv_ipRoute[i].inetcidrroutepfxlen = R_IPV6_PREFIX_LEN * 8u;
            g_tlv_ipRoute[i].inetcidrroutenexthoptype = IPV6;
            g_tlv_ipRoute[i].inetcidrroutenexthop.len = R_IPV6_ADDRESS_LENGTH;
            MEMCPY_A(g_tlv_ipRoute[i].inetcidrroutenexthop.data, apAddr.bytes);
            g_tlv_ipRoute[i].inetcidrrouteifindex = 1;
            g_tlv_ipRoute[i].inetcidrrouteindex = (int32_t)i + 1;
            i++;
        }
    }

    *num = i;
    for (i = 0; i < *num; i++)
    {
        g_tlv_ipRoute[i].has_inetcidrrouteindex = true;
        g_tlv_ipRoute[i].has_inetcidrroutedesttype = true;
        g_tlv_ipRoute[i].has_inetcidrroutedest = true;
        g_tlv_ipRoute[i].has_inetcidrroutepfxlen = true;
        g_tlv_ipRoute[i].has_inetcidrroutenexthoptype = true;
        g_tlv_ipRoute[i].has_inetcidrroutenexthop = true;
        g_tlv_ipRoute[i].has_inetcidrrouteifindex = true;
    }

    if (*num == 0)
    {
        return NULL;
    }

    return &g_tlv_ipRoute;
}

void* currenttime_get(uint32_t* num)
{
    struct timeval tv = {0};

    *num = 1;
    MEMZERO_S(&g_tlv_currentTime);

    g_tlv_currentTime.has_posix = true;
    g_tlv_currentTime.has_source = true;

    osal_gettime(&tv, NULL);
    g_tlv_currentTime.posix = tv.tv_sec;
    g_tlv_currentTime.source = 1;  // Local

    return &g_tlv_currentTime;
}

/**
 * @brief Parse exactly @p n decimal digits from a character buffer.
 *
 * @param s Pointer to the input character buffer containing at least @p n characters.
 * @param n Number of digits to parse.
 * @param parsed The parsed numeric value.
 *
 * @return r_result_t R_RESULT_SUCCESS on success, R_RESULT_FAILED on parse error.
 */
static r_result_t parse_ndigits(const char *s, uint8_t n, uint32_t *parsed)
{
    *parsed = 0;
    for (int i = 0; i < n; ++i)
    {
        unsigned char c = (unsigned char)s[i];
        if (c < '0' || c > '9')
        {
            return R_RESULT_FAILED;
        }
        *parsed = *parsed * 10 + (c - '0');
    }
    return R_RESULT_SUCCESS;
}

/**
 * @brief Determine whether a given year is a leap year.
 *
 * @param year Year to evaluate.
 *
 * @return r_boolean_t TRUE if @p year is a leap year, FALSE otherwise.
 */
static r_boolean_t is_leap(uint16_t year)
{
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

/**
 * @brief Get the number of days in a given month of a specific year.
 *
 * @param year  Year used to determine leap-year behavior.
 * @param month Month index (1–12) for which the day count is requested.
 *
 * @return uint8_t Number of days in the specified month.
 */
static uint8_t days_in_month(const uint16_t year, const uint8_t month)
{
    static const uint8_t base_days[12] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };
    uint8_t d = base_days[month - 1];
    if (month == 2 && is_leap(year))
    {
        d = 29;
    }
    return d;
}

/**
 * @brief Compute the number of days since the Unix epoch (1970-01-01).
 *
 * @param year             Year of the input date.
 * @param month            Month (1–12) of the input date.
 * @param day              Day (1–31) of the input date.
 * @param days_since_epoch Pointer receiving the computed day count.
 *
 * @return r_result_t R_RESULT_SUCCESS on success, R_RESULT_FAILED on validation error.
 */
static r_result_t days_since_epoch(const uint16_t year, const uint8_t month, const uint8_t day, uint32_t *days_since_epoch)
{
    *days_since_epoch = 0;

    if (year >= 1970)
    {
        uint16_t y;
        for (y = 1970; y < year; ++y)
        {
            *days_since_epoch += 365 + is_leap(y);
        }
    }
    else
    {
        return R_RESULT_FAILED;
    }

    for (uint8_t m = 1; m < month; ++m)
    {
        *days_since_epoch += days_in_month(year, m);
    }

    *days_since_epoch += (day - 1); // 1970-01-01 -> day index 0

    return R_RESULT_SUCCESS;
}

/**
 * @brief Parse an ISO 8601 timestamp string and convert it to a Unix timestamp.
 *
 * Supported formats:
 *   - "YYYY-MM-DDTHH:MM:SSZ"
 *   - "YYYY-MM-DDTHH:MM:SS+HH:MM"
 *   - "YYYY-MM-DDTHH:MM:SS-HH:MM"
 *   - "YYYY-MM-DD HH:MM:SSZ" (space instead of 'T')
 *
 * @param from Pointer to a zero-terminated ISO 8601 input string.
 * @param to   Pointer receiving the computed Unix timestamp in seconds.
 *
 * @return r_result_t R_RESULT_SUCCESS on success, R_RESULT_FAILED on parse or validation error.
 */
r_result_t iso8601_to_unix(const char *from, uint32_t *to)
{
    if (from == NULL)
    {
        return R_RESULT_FAILED;
    }

    const size_t len = strlen(from);
    if (len < 19)
    {
        // Minimal length: "YYYY-MM-DDTHH:MM:SS"
        return R_RESULT_FAILED;
    }

    // Parse date/time components
    uint32_t year, month, day, hour, min, sec;
    r_result_t result = parse_ndigits(from + 0, 4, &year);
    result |= parse_ndigits(from + 5, 2, &month);
    result |= parse_ndigits(from + 8, 2, &day);
    const char sep1 = from[4];  // '-'
    const char sep2 = from[7];  // '-'
    const char sep3 = from[10]; // 'T' or ' '
    result |= parse_ndigits(from + 11, 2, &hour);
    const char sep4 = from[13]; // ':'
    result |= parse_ndigits(from + 14, 2, &min);
    const char sep5 = from[16]; // ':'
    result |= parse_ndigits(from + 17, 2, &sec);

    if (result != R_RESULT_SUCCESS)
    {
        return result;
    }

    if (month < 1 || month > 12 || day < 1 || day > 31 ||
        hour > 23 || min > 59 || sec > 60)
    {
        return R_RESULT_FAILED;
    }

    if (sep1 != '-' || sep2 != '-' ||
        (sep3 != 'T' && sep3 != ' ') ||
        sep4 != ':' || sep5 != ':')
    {
        return R_RESULT_FAILED;
    }

    // Validate day against month/year
    if (day > days_in_month(year, month))
    {
        return R_RESULT_FAILED;
    }

    // Timezone offset in seconds (relative to UTC)
    int32_t offset_seconds = 0;

    // Position after "YYYY-MM-DDTHH:MM:SS" (19 chars)
    const uint8_t pos = 19;
    if (pos < len)
    {
        const char c = from[pos];
        if (c == 'Z' || c == 'z')
        {
            // UTC, offset 0
            offset_seconds = 0;
        }
        else if (c == '+' || c == '-')
        {
            // +HH:MM or -HH:MM
            if ((uint16_t) len < pos + 6)
            {
                return R_RESULT_FAILED; // not enough room for "+HH:MM"
            }
            const int32_t sign = (c == '+') ? 1 : -1;

            uint32_t off_hour, off_min;
            result = parse_ndigits(from + pos + 1, 2, &off_hour);
            const char colon   = from[pos + 3];
            result |= parse_ndigits(from + pos + 4, 2, &off_min);

            if (result != R_RESULT_SUCCESS ||
                off_hour > 23 || off_min > 59 ||
                colon != ':')
            {
                return R_RESULT_FAILED;
            }

            offset_seconds = sign * (int32_t) (off_hour * 3600 + off_min * 60);
        }
        else
        {
            // Unexpected trailing character
            return R_RESULT_FAILED;
        }
    }

    // Compute days since epoch and then total seconds
    uint32_t days;
    result = days_since_epoch(year, month, day, &days);

    if (result != R_RESULT_SUCCESS)
    {
        return result;
    }

    *to = days * 86400 + hour * 3600 + min * 60 + sec;

    /*
     * Interpretation of timezone: "2025-11-15T10:30:00+02:00"
     * means local_time = UTC + 2h  =>  UTC = local_time - 2h
     *
     * We have parsed the *local* time (10:30).
     * So we must subtract the offset to get UTC.
     */
    *to -= offset_seconds;

    return R_RESULT_SUCCESS;
}

void currenttime_post(Current_Time* tlv)
{
    struct timeval tv = {0};

    if (tlv->has_posix)
    {
        tv.tv_sec = tlv->posix;
        osal_settime(&tv, NULL);
    }
    else if (tlv->has_iso8601)
    {
        uint32_t posix_seconds = 0;
        iso8601_to_unix(tlv->iso8601, &posix_seconds);
        tv.tv_sec = posix_seconds;
        osal_settime(&tv, NULL);
    }
}

void* uptime_get(uint32_t* num)
{
    *num = 1;
    MEMZERO_S(&g_tlv_upTime);

    g_tlv_upTime.has_sysuptime = true;

    g_tlv_upTime.sysuptime = clock_seconds();

    return &g_tlv_upTime;
}

void* interface_metrics_get(uint32_t* num)
{
    uint16_t phy_DataRate;
    *num = 1;
    MEMZERO_S(&g_tlv_interfaceMetrics);

    g_tlv_interfaceMetrics.has_ifindex = true;
    g_tlv_interfaceMetrics.has_ifinspeed = true;
    g_tlv_interfaceMetrics.has_ifoutspeed = true;
    g_tlv_interfaceMetrics.has_ifadminstatus = true;
    g_tlv_interfaceMetrics.has_ifoperstatus = true;
    g_tlv_interfaceMetrics.has_iflastchange = true;
    g_tlv_interfaceMetrics.has_ifinoctets = true;
    g_tlv_interfaceMetrics.has_ifoutoctets = true;
    g_tlv_interfaceMetrics.has_ifindiscards = true;
    g_tlv_interfaceMetrics.has_ifinerrors = true;
    g_tlv_interfaceMetrics.has_ifoutdiscards = true;
    g_tlv_interfaceMetrics.has_ifouterrors = true;

    g_tlv_interfaceMetrics.ifindex = 1;
    if (R_NWK_GetRequest(R_NWK_phyDataRate, &phy_DataRate, sizeof(phy_DataRate)) == R_RESULT_SUCCESS)
    {
        g_tlv_interfaceMetrics.ifinspeed = phy_DataRate;
        g_tlv_interfaceMetrics.ifoutspeed = phy_DataRate;
    }
    else
    {
        g_tlv_interfaceMetrics.has_ifinspeed = false;
        g_tlv_interfaceMetrics.has_ifoutspeed = false;
    }
    g_tlv_interfaceMetrics.ifadminstatus = IF_ADMIN_STATUS_UP;
    g_tlv_interfaceMetrics.ifoperstatus = IF_OPER_STATUS_UP;
    g_tlv_interfaceMetrics.iflastchange = clock_seconds();
    r_nwk_if_mib_t mgmt;
    if (R_NWK_GetRequest(R_NWK_fanInterfaceMgmtInfo, &mgmt, sizeof(mgmt)) == R_RESULT_SUCCESS)
    {
        g_tlv_interfaceMetrics.ifinoctets = mgmt.stats.ifinoctets;
        g_tlv_interfaceMetrics.ifoutoctets = mgmt.stats.ifoutoctets;
        g_tlv_interfaceMetrics.ifindiscards = mgmt.stats.ifindiscards + mgmt.stats.ifinunknownprotos;
        g_tlv_interfaceMetrics.ifinerrors = mgmt.stats.ifinerrors;
        g_tlv_interfaceMetrics.ifoutdiscards = mgmt.stats.ifoutdiscards;
        g_tlv_interfaceMetrics.ifouterrors = mgmt.stats.ifouterrors;
    }
    else
    {
        g_tlv_interfaceMetrics.has_ifinoctets = false;
        g_tlv_interfaceMetrics.has_ifoutoctets = false;
        g_tlv_interfaceMetrics.has_ifindiscards = false;
        g_tlv_interfaceMetrics.has_ifinerrors = false;
        g_tlv_interfaceMetrics.has_ifoutdiscards = false;
        g_tlv_interfaceMetrics.has_ifouterrors = false;
    }

    return &g_tlv_interfaceMetrics;
}

/**
 * Search in the neighbor cache for a given address and fills the RPL metric information
 * @param index The index of the metric information in the reported TLV
 * @param apAddr The address for which the metrics needs to be retrieved
 * @param metric The metric structure to be filled
 * @return R_RESULT_SUCCESS if the address was found, R_RESULT_FAILED otherwise
 */
r_result_t fillIpRouteRplMetric(uint32_t index, r_ipv6addr_t* apAddr, IPRoute_RPLMetrics* metric)
{
    r_result_t res;
    r_boolean_t isContinuation = R_FALSE;
    do
    {
        nd_cache_buffer_t cfm;
        res = R_NWK_GetRequestMultipart(R_NWK_ndCache, &cfm, sizeof(cfm), isContinuation);
        if (res == R_RESULT_SUCCESS || res == R_RESULT_SUCCESS_ADDITIONAL_DATA)
        {
            isContinuation = R_TRUE;

            for (uint16_t i = 0; i < cfm.nd_cache.count; i++)
            {
                r_boolean_t check = R_TRUE;
                for (uint16_t j = 0; j < sizeof(cfm.nd_cache.entries[i].ipAddress.bytes); j++)
                {
                    if (cfm.nd_cache.entries[i].ipAddress.bytes[j] != apAddr->bytes[j])
                    {
                        check = R_FALSE;
                        break;
                    }
                }
                if (check == R_TRUE)
                {
                    metric->inetcidrrouteindex = (int32_t)index + 1;
                    metric->instanceindex = 1;
                    metric->rank = cfm.nd_cache.entries[i].rank;
                    metric->pathetx = cfm.nd_cache.entries[i].routingCost;
                    metric->linketx = cfm.nd_cache.entries[i].linkMetric;
                    metric->lqiforward = cfm.nd_cache.entries[i].nodeToNeighRsl;
                    metric->lqireverse = cfm.nd_cache.entries[i].neighToNodeRsl;
                    metric->rssiforward = -174 + cfm.nd_cache.entries[i].nodeToNeighRsl;
                    metric->rssireverse = -174 + cfm.nd_cache.entries[i].neighToNodeRsl;
                    metric->dagsize = cfm.nd_cache.entries[i].panSize;
                    return R_RESULT_SUCCESS;
                }
            }
        }
        else
        {
            return R_RESULT_FAILED;
        }
    }
    while (res == R_RESULT_SUCCESS_ADDITIONAL_DATA);
    return R_RESULT_FAILED;
}

void* iproute_rplmetrics_get(uint32_t* num)
{
    uint32_t index = 0;
    r_ipv6addr_t apAddr;
    MEMZERO_A(g_tlv_iprouteRplmetrics);

#if R_BORDER_ROUTER_ENABLED
    uint8_t deviceType;
    if (R_NWK_GetRequest(R_NWK_deviceType, &deviceType, sizeof(deviceType)) == R_RESULT_SUCCESS &&
        deviceType != R_BORDERROUTER)
#endif
    {
        if (R_NWK_GetRequest(R_NWK_preferredParentAddress, &apAddr, sizeof(apAddr)) == R_RESULT_SUCCESS
            && MEMISNOTZERO_S(&apAddr))
        {
            if (fillIpRouteRplMetric(index, &apAddr, &g_tlv_iprouteRplmetrics[index]) == R_RESULT_SUCCESS)
            {
                index++;
            }
        }

        if (R_NWK_GetRequest(R_NWK_alternateParentAddress, &apAddr, sizeof(apAddr)) == R_RESULT_SUCCESS
            && MEMISNOTZERO_S(&apAddr))
        {
            if (fillIpRouteRplMetric(index, &apAddr, &g_tlv_iprouteRplmetrics[index]) == R_RESULT_SUCCESS)
            {
                index++;
            }
        }
    }

    *num = index;
    for (uint32_t i = 0; i < *num; i++)
    {
        g_tlv_iprouteRplmetrics[i].has_inetcidrrouteindex = true;
        g_tlv_iprouteRplmetrics[i].has_instanceindex = true;
        g_tlv_iprouteRplmetrics[i].has_rank = true;
        g_tlv_iprouteRplmetrics[i].has_pathetx = true;
        g_tlv_iprouteRplmetrics[i].has_linketx = true;
        g_tlv_iprouteRplmetrics[i].has_rssiforward = true;
        g_tlv_iprouteRplmetrics[i].has_rssireverse = true;
        g_tlv_iprouteRplmetrics[i].has_lqiforward = true;
        g_tlv_iprouteRplmetrics[i].has_lqireverse = true;
        g_tlv_iprouteRplmetrics[i].has_dagsize = true;
    }

    if (*num == 0)
    {
        return NULL;
    }

    return &g_tlv_iprouteRplmetrics;
}

void* wpanstatus_get(uint32_t* num)
{
    *num = 1;
    MEMZERO_S(&g_tlv_wpanStatus);

    g_tlv_wpanStatus.has_ifindex = true;
    g_tlv_wpanStatus.has_ssid = true;
    g_tlv_wpanStatus.has_panid = true;
    g_tlv_wpanStatus.has_dot1xenabled = true;
    g_tlv_wpanStatus.has_securitylevel = true;
    g_tlv_wpanStatus.has_rank = true;
    g_tlv_wpanStatus.has_beaconvalid = true;
    g_tlv_wpanStatus.has_beaconversion = true;
    g_tlv_wpanStatus.has_beaconage = true;
    g_tlv_wpanStatus.has_txpower = true;
    g_tlv_wpanStatus.has_dagsize = true;
    g_tlv_wpanStatus.has_metric = true;
    g_tlv_wpanStatus.has_lastchanged = true;
    g_tlv_wpanStatus.has_lastchangedreason = true;
    g_tlv_wpanStatus.has_demomodeenabled = false;

    g_tlv_wpanStatus.ifindex = 1;
    const char* netName = R_APP_GetNetworkName();
    strncpy((char *) g_tlv_wpanStatus.ssid.data, netName, sizeof(g_tlv_wpanStatus.ssid.data));
    g_tlv_wpanStatus.ssid.data[sizeof(g_tlv_wpanStatus.ssid.data) - 1] = '\0';
    g_tlv_wpanStatus.ssid.len = strlen(netName) + 1;
    uint16_t panId;
    if (R_NWK_GetRequest(R_NWK_macPANId, &panId, sizeof(panId)) == R_RESULT_SUCCESS)
    {
        g_tlv_wpanStatus.panid = panId;
    }
    else
    {
        g_tlv_wpanStatus.has_panid = false;
    }
    g_tlv_wpanStatus.dot1xenabled = true;
    g_tlv_wpanStatus.securitylevel = IEEE154_SEC_MIC_32;
#if !R_LEAF_NODE_ENABLED
    r_nwk_rpl_info_t rplInfo;
    if (R_NWK_GetRequest(R_NWK_rplInfo, &rplInfo, sizeof(rplInfo)) == R_RESULT_SUCCESS)
    {
        g_tlv_wpanStatus.rank = rplInfo.rank;
    }
    else
#endif
    {
        g_tlv_wpanStatus.has_rank = false;
    }
#if !R_LEAF_NODE_ENABLED
    uint16_t panVersion;
    if (R_NWK_GetRequest(R_NWK_panVersion, &panVersion, sizeof(panVersion)) == R_RESULT_SUCCESS)
    {
        g_tlv_wpanStatus.beaconversion = panVersion;
    }
    else
#endif
    {
        g_tlv_wpanStatus.has_beaconversion = false;
    }
    g_tlv_wpanStatus.beaconvalid = true;
    clock_time_t beaconAge;
    if (R_NWK_GetRequest(R_NWK_lastPanConfigRecvTime, &beaconAge, sizeof(beaconAge)) == R_RESULT_SUCCESS)
    {
        g_tlv_wpanStatus.beaconage = clock_seconds() - beaconAge;
    }
    else
    {
        g_tlv_wpanStatus.has_beaconage = false;
    }

    int32_t csmpTxPower = INT32_MAX;
#if R_PHY_TYPE_CWX_M
    int8_t cwxTxPower;
    if (R_NWK_GetRequest(R_NWK_phyFskTransmitPower, &cwxTxPower, sizeof(cwxTxPower)) == R_RESULT_SUCCESS)
    {
        /* Convert CWX value to actual TX power output at antenna connector in dBm */
        csmpTxPower = cwxTxPower / 2;   // CWX value is in units of 0.5 dBm
        csmpTxPower = csmpTxPower + 22; // 20-24 dB FEM gain depending on CWX board type -> Add avg FEM gain
    }
#else
    uint8_t trgTxPower;
    if (R_NWK_GetRequest(R_NWK_phyTransmitPower, &trgTxPower, sizeof(trgTxPower)) == R_RESULT_SUCCESS)
    {
        csmpTxPower = trgTxPower;  // TODO convert TRG value to dBm (requires mapping table depending on region)
    }
#endif
    if (csmpTxPower != INT32_MAX)
    {
        g_tlv_wpanStatus.txpower = csmpTxPower;
    }
    else
    {
        g_tlv_wpanStatus.has_txpower = false;
    }
    uint16_t panSize = 0;
    if (R_NWK_GetRequest(R_NWK_panSize, &panSize, sizeof(panSize)))
    {
        g_tlv_wpanStatus.dagsize = panSize;
    }
    else
    {
        g_tlv_wpanStatus.has_dagsize = false;
    }

#if R_BORDER_ROUTER_ENABLED
    uint8_t deviceType;
    if (R_NWK_GetRequest(R_NWK_deviceType, &deviceType, sizeof(deviceType)) == R_RESULT_SUCCESS &&
        deviceType == R_BORDERROUTER)
    {
        g_tlv_wpanStatus.has_metric = false;
    }
    else
#endif
    {
        r_result_t res;
        r_ipv6addr_t addrPreferred;
        r_boolean_t foundPreferred = R_FALSE;
        if (R_NWK_GetRequest(R_NWK_preferredParentAddress, &addrPreferred, sizeof(addrPreferred)) == R_RESULT_SUCCESS
            && MEMISNOTZERO_S(&addrPreferred))
        {
            r_boolean_t isContinuation = R_FALSE;
            do
            {
                nd_cache_buffer_t cfm;
                res = R_NWK_GetRequestMultipart(R_NWK_ndCache, &cfm, sizeof(cfm), isContinuation);
                if (res == R_RESULT_SUCCESS || res == R_RESULT_SUCCESS_ADDITIONAL_DATA)
                {
                    if (!isContinuation)
                    {
                        isContinuation = R_TRUE;
                    }
                    for (uint16_t i = 0; i < cfm.nd_cache.count; i++)
                    {
                        r_boolean_t check = R_TRUE;
                        for (uint16_t j = 0; j < sizeof(cfm.nd_cache.entries[i].ipAddress.bytes); j++)
                        {
                            if (cfm.nd_cache.entries[i].ipAddress.bytes[j] != addrPreferred.bytes[j])
                            {
                                check = R_FALSE;
                                break;
                            }
                        }
                        if (check == R_TRUE)
                        {
                            g_tlv_wpanStatus.metric = cfm.nd_cache.entries[i].routingCost + cfm.nd_cache.entries[i].linkMetric;
                            foundPreferred = R_TRUE;
                            break;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
            while (res == R_RESULT_SUCCESS_ADDITIONAL_DATA);
            if (!foundPreferred)
            {
                g_tlv_wpanStatus.has_metric = false;
            }
        }
        else
        {
            g_tlv_wpanStatus.has_metric = false;
        }
    }

    g_tlv_wpanStatus.lastchanged = 0;
    g_tlv_wpanStatus.lastchangedreason = 0;

    return &g_tlv_wpanStatus;
}

void* rplinstance_get(uint32_t* num)
{
    *num = 1;
    MEMZERO_S(&g_tlv_rplInstance);

    g_tlv_rplInstance.has_instanceindex = true;
    g_tlv_rplInstance.has_instanceid = true;
    g_tlv_rplInstance.has_dodagid = true;
    g_tlv_rplInstance.has_dodagversionnumber = true;
    g_tlv_rplInstance.has_rank = true;
    g_tlv_rplInstance.has_parentcount = true;
    g_tlv_rplInstance.has_dagsize = true;

    g_tlv_rplInstance.instanceindex = 1;
#if !R_LEAF_NODE_ENABLED
    r_nwk_rpl_info_t rplInfo;
    if (R_NWK_GetRequest(R_NWK_rplInfo, &rplInfo, sizeof(rplInfo)) == R_RESULT_SUCCESS)
    {
        MEMCPY_A(g_tlv_rplInstance.dodagid.data, rplInfo.dodag_id.bytes);
        g_tlv_rplInstance.dodagid.len = 16;
        g_tlv_rplInstance.dodagversionnumber = rplInfo.dodag_version;
        g_tlv_rplInstance.rank = rplInfo.rank;
        g_tlv_rplInstance.instanceid = rplInfo.instance_id;
    }
    else
#endif
    {
        g_tlv_rplInstance.has_dodagid = false;
        g_tlv_rplInstance.has_dodagversionnumber = false;
        g_tlv_rplInstance.has_rank = false;
        g_tlv_rplInstance.has_instanceid = false;
    }

#if R_BORDER_ROUTER_ENABLED
    uint8_t deviceType;
    if (R_NWK_GetRequest(R_NWK_deviceType, &deviceType, sizeof(deviceType)) == R_RESULT_SUCCESS &&
        deviceType == R_BORDERROUTER)
    {
        g_tlv_rplInstance.parentcount = 0;
    }
    else
#endif
    {
        g_tlv_rplInstance.parentcount = 1;  // We must have a Preferred Parent (otherwise we could not communicate with NMS)
        r_ipv6addr_t apAddr;
        if (R_NWK_GetRequest(R_NWK_alternateParentAddress, &apAddr, sizeof(apAddr)) == R_RESULT_SUCCESS
            && MEMISNOTZERO_S(&apAddr))
        {
            g_tlv_rplInstance.parentcount++;  // We also have an Alternate Parent -> Increment counter
        }
    }

    uint16_t nwkPanSize = 0;
    if (R_NWK_GetRequest(R_NWK_panSize, &nwkPanSize, sizeof(nwkPanSize)) == R_RESULT_SUCCESS)
    {
        g_tlv_rplInstance.dagsize = nwkPanSize;
    }
    else
    {
        g_tlv_rplInstance.has_dagsize = false;
    }

    return &g_tlv_rplInstance;
}

void* signature_settings_get(uint32_t* num)
{
    *num = 1;

    return &g_SignatureSettings;
}

void signature_settings_post(Signature_Settings* tlv)
{
    g_SignatureSettings.has_reqsignedpost = true;
    g_SignatureSettings.reqsignedpost = tlv->reqsignedpost;

    g_SignatureSettings.has_reqvalidcheckpost = true;
    g_SignatureSettings.reqvalidcheckpost = tlv->reqvalidcheckpost;

    g_SignatureSettings.has_reqtimesyncpost = true;
    g_SignatureSettings.reqtimesyncpost = tlv->reqtimesyncpost;

    g_SignatureSettings.has_reqseclocalpost = true;
    g_SignatureSettings.reqseclocalpost = tlv->reqseclocalpost;

    g_SignatureSettings.has_reqsignedresp = true;
    g_SignatureSettings.reqsignedresp = tlv->reqsignedresp;

    g_SignatureSettings.has_reqvalidcheckresp = true;
    g_SignatureSettings.reqvalidcheckresp = tlv->reqvalidcheckresp;

    g_SignatureSettings.has_reqtimesyncresp = true;
    g_SignatureSettings.reqtimesyncresp = tlv->reqtimesyncresp;

    g_SignatureSettings.has_reqseclocalresp = true;
    g_SignatureSettings.reqseclocalresp = tlv->reqseclocalresp;

    g_SignatureSettings.has_cert = true;
    g_SignatureSettings.cert.len = tlv->cert.len;
    MEMCPY_A(g_SignatureSettings.cert.data, tlv->cert.data);
}

void* vendorTlv_get(tlvid_t tlvid, uint32_t *num)
{
    return NULL;
}

void vendorTlv_post(tlvid_t tlvid, Vendor_Tlv *tlv)
{
    return;
}

void* transferRequest_get(tlvid_t tlvid, uint32_t* num)
{
#if R_FWUP_SUPPORT
    (void)tlvid;
    *num = 1;
    R_LOG_DBG("## transferRequest_get: GET for TLV %{u32}", tlvid.type);

    // Check upload slot download status
    if (uploadSlotValid && uploadSlotHdr.status != (uint32_t) FWHDR_STATUS_DOWNLOAD)
    {
        g_transferRequest.status = uploadSlotHdr.status;
        R_LOG_DBG("transferRequest_get: Transfer request download status = 0x%{hex32}", g_transferRequest.status);
        return &g_transferRequest;
    }
    // Update g_transferRequest fields
    MEMCPY_A(g_transferRequest.filehash.data, uploadSlotHdr.filehash);
    strncpy(g_transferRequest.filename, uploadSlotHdr.filename, sizeof(g_transferRequest.filename));
    g_transferRequest.filename[sizeof(g_transferRequest.filename) - 1] = '\0';
    strncpy(g_transferRequest.version, uploadSlotHdr.version, sizeof(g_transferRequest.version));
    g_transferRequest.version[sizeof(g_transferRequest.version) -1] = '\0';
    strncpy(g_transferRequest.hwinfo.hwid, uploadSlotHdr.hwid, sizeof(g_transferRequest.hwinfo.hwid));
    g_transferRequest.hwinfo.hwid[sizeof(g_transferRequest.hwinfo.hwid) -1] = '\0';
    g_transferRequest.filesize = uploadSlotHdr.filesize;
    g_transferRequest.blocksize = uploadSlotHdr.blocksize;
    g_transferRequest.report_int_min = uploadSlotHdr.reportintervalmin;
    g_transferRequest.report_int_max = uploadSlotHdr.reportintervalmax;
    g_transferRequest.status = uploadSlotHdr.status;

    R_LOG_DBG("## transferRequest_get: GET for TLV %{u32} done.", tlvid.type);
    return &g_transferRequest;
#else
    *num = 0;
    return NULL;
#endif
}

void transferRequest_post(tlvid_t tlvid, Transfer_Request* tlv)
{
#if R_FWUP_SUPPORT
    (void)tlvid;
    R_LOG_DBG("## transferRequest_post: POST for TLV %{u32}.", tlvid.type);

    if (!tlv)
    {
        R_LOG_DBG("## transferRequest_post: Transfer request tlv context is NULL");
        return;
    }

    tlv->has_response = true;
    uint32_t tmin = tlv->report_int_min * 1000;
    uint32_t tmax = tlv->report_int_max * 1000;

    // Check hardware id
    if (!(tlv->hwinfo.has_hwid))
    {
        tlv->response = RESPONSE_INCOMPATIBLE_HW;
        R_LOG_DBG("## transferRequest_post: Invalid hardware id: %s", tlv->hwinfo.hwid);
        return;
    }
    // Check filehash len
    if (tlv->filehash.len != SHA256_HASH_SIZE)
    {
        tlv->response = RESPONSE_INVALID_REQ;
        R_LOG_DBG("## transferRequest_post: Invalid filehash size: %{u16}", tlv->filehash.len);
        return;
    }
    // Check filesize
    if (tlv->filesize == 0 ||
        tlv->filesize > CSMP_FWMGMT_SLOTIMG_SIZE)
    {
        tlv->response = RESPONSE_FILE_SIZE_TOO_BIG;
        R_LOG_DBG("## transferRequest_post: Invalid file size: %{u32}", tlv->filesize);
        return;
    }
    // blocksize should be smaller than csmp's MTU (1024)
    // blocksize should be larger than filesize/1024 since there is only 1024 bitmaps
    if (tlv->blocksize == 0 ||
        tlv->blocksize > BLOCK_SIZE ||
        tlv->blocksize < tlv->filesize / (CSMP_FWMGMT_BLKMAP_CNT * 32))
    {
        tlv->response = RESPONSE_INVALID_BLOCK_SIZE;
        R_LOG_DBG("## transferRequest_post: Invalid block size: %{u32}", tlv->blocksize);
        return;
    }
    // Check pending reboot
    if (initload)
    {
        tlv->response = RESPONSE_PENDING_REBOOT;
        R_LOG_DBG("## transferRequest_post: Pending reboot for upload image");
        return;
    }
    // Check duplicate request on Upload slot
    if ((memcmp(tlv->filehash.data, uploadSlotHdr.filehash, tlv->filehash.len)) == 0)
    {
        tlv->response = RESPONSE_DUP_XFER;
        R_LOG_DBG("## transferRequest_post: Duplicate transfer request");
        return;
    }
    // Check duplicate request on Run slot
    if ((memcmp(tlv->filehash.data, runImageFileHash, tlv->filehash.len)) == 0)
    {
        tlv->response = RESPONSE_MATCH_RUN_XFER;
        R_LOG_DBG("## transferRequest_post: Transfer request matches Run image");
        return;
    }

    // Initiliase new transfer - start
    tlv->response = RESPONSE_OK;
    MEMCPY_S(&g_transferRequest, tlv);
    if (!initxfer)
    {
        initxfer = true;
    }

    // Init report intervals
    tmin = (tmin >= MIN_REPORT_MIN) ? tmin : MIN_REPORT_MIN;
    tmax = (tmax >= MAX_REPORT_MIN) ? tmax : MAX_REPORT_MIN;
    tmax = (tmax < tmin) ? tmin : tmax;

    // Erase upload slot
    memset(&uploadSlotHdr, 0xFF, sizeof(uploadSlotHdr));

    // Init upload slot from tlv context
    MEMCPY_A(uploadSlotHdr.filehash, tlv->filehash.data);
    strncpy(uploadSlotHdr.filename, tlv->filename, sizeof(uploadSlotHdr.filename));
    uploadSlotHdr.filename[sizeof(uploadSlotHdr.filename) - 1] = '\0';
    strncpy(uploadSlotHdr.version, tlv->version, sizeof(uploadSlotHdr.version));
    uploadSlotHdr.version[sizeof(uploadSlotHdr.version) - 1] = '\0';
    strncpy(uploadSlotHdr.hwid, tlv->hwinfo.hwid, sizeof(uploadSlotHdr.hwid));
    uploadSlotHdr.hwid[sizeof(uploadSlotHdr.hwid) - 1] = '\0';
    uploadSlotHdr.filesize = tlv->filesize;
    uploadSlotHdr.blocksize = tlv->blocksize;
    uploadSlotHdr.filesizelastblk = uploadSlotHdr.filesize %
        uploadSlotHdr.blocksize;
    if (uploadSlotHdr.filesizelastblk == 0)
    {
        uploadSlotHdr.filesizelastblk = uploadSlotHdr.blocksize;
    }
    uploadSlotHdr.blockcnt = (uploadSlotHdr.filesize + (uploadSlotHdr.blocksize - 1)) /
        uploadSlotHdr.blocksize;
    uploadSlotHdr.reportintervalmin = tmin;
    uploadSlotHdr.reportintervalmax = tmax;
    uploadSlotHdr.status = (uint32_t) FWHDR_STATUS_DOWNLOAD;

    r_fwup_result_t result = R_APP_FWUP_TransferRequest(uploadSlotHdr.filehash, uploadSlotHdr.filename, uploadSlotHdr.version,
        uploadSlotHdr.filesize, uploadSlotHdr.blocksize);

    if (result == R_FWUP_RESULT_OK)
    {
        // Initiliase new transfer - done
        initxfer = false;
        uploadSlotValid = R_TRUE;
        R_LOG_DBG("## transferRequest_post: POST for TLV %{u32} done.", tlvid.type);
    }
    else
    {
        tlv->response = RESPONSE_INVALID_REQ;
        R_LOG_ERR("## transferRequest_post: R_APP_FWUP_TransferRequest failed: 0x%{hex8}", result);
    }
#endif
}

void imageBlock_post(tlvid_t tlvid, Image_Block* tlv)
{
#if R_FWUP_SUPPORT
    (void)tlvid;
    R_LOG_DBG("## imageBlock_post: POST for TLV %{u32}.", tlvid.type);

    if (!tlv)
    {
        R_LOG_ERR("## imageBlock_post: Image block tlv context is NULL");
        return;
    }

    /* Update g_imageBlock structure from tlv context */
    MEMCPY_S(&g_imageBlock, tlv);

    /* Check file hash length, file hash data, block data length with upload slot (as from transfer request) */
    if ((g_imageBlock.filehash.len >= MIN_HASH_COMPARE_LEN) && (g_imageBlock.filehash.len <= sizeof(uploadSlotHdr.filehash)) &&
        (memcmp(g_imageBlock.filehash.data, uploadSlotHdr.filehash, g_imageBlock.filehash.len) == 0) &&
        (g_imageBlock.blockdata.len <= uploadSlotHdr.blocksize))
    {
        tlv->retval = true;
        R_LOG_DBG("## imageBlock_post: Writing image block %{u32} to upload slot", g_imageBlock.blocknum);
        /* Check the initialization of the transfer request */
        if (initxfer)
        {
            tlv->retval = false;
            R_LOG_DBG("## imageBlock_post: Transfer still initializing ");
            g_downloadbusy = false;
            return;
        }

        /* Note: FND sends only a small Hash while the FWUP library expects the full filehash */
        r_fwup_result_t result = R_APP_FWUP_BlockRequest(uploadSlotHdr.filehash, g_imageBlock.blocknum,
                g_imageBlock.blockdata.len, g_imageBlock.blockdata.data);
        if (result != R_FWUP_RESULT_OK)
        {
            R_LOG_ERR("## imageBlock_post: R_APP_FWUP_BlockRequest failed: 0x%{hex8}", result);
            tlv->retval = false;
            g_downloadbusy = false;
            return;
        }
        R_LOG_DBG("## imageBlock_post: Successful write of image block %{u32} (len=%{u32})", g_imageBlock.blocknum, g_imageBlock.blockdata.len);
        if (g_imageBlock.blocknum == uploadSlotHdr.filesize / uploadSlotHdr.blocksize)
        {
            R_LOG_DBG("## imageBlock_post: Image transfer completed!");
            uploadSlotHdr.status = (uint32_t) FWHDR_STATUS_COMPLETE;
        }
    }
    else
    {
        tlv->retval = false;
        R_LOG_DBG("## imageBlock_post: Image block POST failed!");
    }

    g_downloadbusy = false;
    R_LOG_DBG("## imageBlock_post: POST for TLV %{u32} done.", tlvid.type);
#endif
}

void* loadRequest_get(tlvid_t tlvid, uint32_t* num)
{
#if R_FWUP_SUPPORT
    (void)tlvid;
    *num = 1;
    R_LOG_DBG("## loadRequest_get: GET for TLV %{u32}.", tlvid.type);

    // Check for pending active load requests
    if (!initload)
    {
        R_LOG_DBG("## loadRequest_get: No active load requests pending");
        return NULL;
    }

    MEMCPY_A(g_loadRequest.filehash.data, uploadSlotHdr.filehash);
    g_loadRequest.filehash.len = SHA256_HASH_SIZE;
    g_loadRequest.loadtime = curloadtime;

    R_LOG_DBG("## loadRequest_get: GET for TLV %{u32} done.", tlvid.type);
    return &g_loadRequest;
#else
    *num = 0;
    return NULL;
#endif
}

void loadRequest_post(tlvid_t tlvid, Load_Request* tlv)
{
#if R_FWUP_SUPPORT
    (void)tlvid;
    R_LOG_DBG("## loadRequest_post: POST for TLV %{u32}.", tlvid.type);

    if (!tlv)
    {
        R_LOG_ERR("## loadRequest_post: Load request tlv context is NULL");
        return;
    }

    // Reference g_loadRequest via tlv context
    g_loadRequest = *tlv;

    if (memcmp(g_loadRequest.filehash.data, uploadSlotHdr.filehash,
               g_loadRequest.filehash.len) == 0)
    {
        // Filehash matches UPLOAD image
        if (uploadSlotHdr.status == (uint32_t) FWHDR_STATUS_COMPLETE)
        {
            R_LOG_DBG("## loadRequest_post: Load request ok for upload slot image");
            tlv->response = RESPONSE_OK;
        }
        else if (uploadSlotHdr.status == (uint32_t) FWHDR_STATUS_BADIMAGE ||
            uploadSlotHdr.status == (uint32_t) FWHDR_STATUS_BADHASH)
        {
            R_LOG_DBG("## loadRequest_post: Load request on bad upload slot image");
            tlv->response = RESPONSE_SIGNATURE_FAILED;
        }
        else
        {
            R_LOG_DBG("## loadRequest_post: Load request incomplete");
            tlv->response = RESPONSE_INCOMPLETE;
        }
        if (tlv->response != RESPONSE_OK)
        {
            R_LOG_ERR("## loadRequest_post: Load request failed");
            return;
        }
    }
    else if (memcmp(g_loadRequest.filehash.data, runImageFileHash,
                    g_loadRequest.filehash.len) == 0)
    {
        // Filehash matches RUN image
        R_LOG_DBG("## loadRequest_post: Load request on running image");
        tlv->response = RESPONSE_IMAGE_RUNNING;
        return;
    }
    else
    {
        // Load request for unknown filehash
        R_LOG_ERR("## loadRequest_post: Load request with unknown filehash");
        tlv->response = RESPONSE_UNKNOWN_HASH;
        return;
    }
    // Check for redundant load request
    if (initload && (g_loadRequest.loadtime == curloadtime))
    {
        R_LOG_DBG("## loadRequest_post: Redundant load request");
        return;
    }

    curloadtime = 0;
    initload = true;

    r_fwup_result_t result = R_APP_FWUP_LoadRequest(g_loadRequest.filehash.data, g_loadRequest.loadtime);
    if (result != R_FWUP_RESULT_OK)
    {
        initload = false;
        R_LOG_ERR("## loadRequest_post: R_APP_FWUP_LoadRequest failed: 0x%{hex8}", result);
        return;
    }

    // Upon success update g_loadRequest structure from tlv context
    MEMCPY_S(&g_loadRequest, tlv);
    // Save new load time
    curloadtime = g_loadRequest.loadtime;

    R_LOG_DBG("## loadRequest_post: POST for TLV 0x%{hex32} done.", tlvid.type);
#endif
}

void cancelLoadRequest_post(tlvid_t tlvid, Cancel_Load_Request* tlv)
{
#if R_FWUP_SUPPORT
    (void)tlvid;
    R_LOG_DBG("## cancelLoadRequest_post: POST for TLV 0x%{hex32}.", tlvid.type);
    if (!tlv)
    {
        R_LOG_DBG("## cancelLoadRequest_post: Cancel load request tlv context is NULL");
        return;
    }

    if (memcmp(tlv->filehash.data, uploadSlotHdr.filehash, SHA256_HASH_SIZE) == 0)
    {
        R_LOG_DBG("## cancelLoadRequest_post: Cancel load request valid, cancelling current load request");

        // Cancel current load request
        r_fwup_result_t  result = R_APP_FWUP_CancelRequest(g_loadRequest.filehash.data);
        if (result != R_FWUP_RESULT_OK)
        {
            R_LOG_ERR("## cancelLoadRequest_post: R_APP_FWUP_CancelRequest failed: 0x%{hex8}", result);
        }
        else
        {
            initload = false;
            curloadtime = 0;
        }
    }

    R_LOG_DBG("## cancelLoadRequest_post: POST for TLV 0x%{hex32} done.", tlvid.type);
#endif
}

void setBackupRequest_post(tlvid_t tlvid, Set_Backup_Request *tlv)
{
#if R_FWUP_SUPPORT
    (void)tlvid;

    if (tlv)
    {
        tlv->has_response = true;
        tlv->response = RESPONSE_NO_VOLUME;
    }

    R_LOG_ERR("## setBackupRequest_post: Backup image slot not supported");
    return;
#endif
}

#if R_FWUP_SUPPORT
static uint8_t reverse_bits8(uint8_t x)
{
    x = (uint8_t)((x >> 4) | (x << 4)); // swap nibbles
    x = (uint8_t)(((x & 0xCCu) >> 2) | ((x & 0x33u) << 2)); // swap bit pairs
    x = (uint8_t)(((x & 0xAAu) >> 1) | ((x & 0x55u) << 1)); // swap individual bits
    return x;
}

static void initialize_file_hash()
{
    // Initialize the file hash through the firmware hash attribute stored in flash
    r_result_t flashRes = R_Flash_Read(R_FLASH_FIRMWARE_HASH, runImageFileHash, SHA256_HASH_SIZE);
    if (flashRes != R_RESULT_SUCCESS)
    {
        R_LOG_ERR("Unable to read the firmware hash from flash");
    }

    if (MEMISZERO_A(runImageFileHash))
    {
#if !defined(__i386) && !defined(__x86_64)
        uint8_t buf[128u];
        st_fw_header_t *p_hdr = (st_fw_header_t *)buf;
        r_fwup_wrap_flash_read((uint32_t) buf, FWUP_CFG_MAIN_AREA_ADDR_L, 128u);
        R_memcpy(runImageFileHash, p_hdr->sig, SHA256_HASH_SIZE);
#else
        memset(runImageFileHash, 0x12, SHA256_HASH_SIZE);
#endif
    }
}

static char hex_digit(uint8_t v)
{
    v &= 0x0F;
    return (v < 10) ? (char)('0' + v) : (char)('a' + (v - 10));
}

static void initialize_file_name()
{
    // Initialize the file hash through the firmware hash attribute stored in flash
    r_result_t flashRes = R_Flash_Read(R_FLASH_FIRMWARE_NAME, runImageFileName, FILE_NAME_SIZE);
    if (flashRes != R_RESULT_SUCCESS)
    {
        R_LOG_ERR("Unable to read the firmware name from flash");
    }

    /**
     * If no firmware file name is stored in data flash (meaning that the current firmware image was not uploaded by
     * FND), create a file name using the same syntax as FND so that images that are uploaded via a programmer will
     * report a similar file name as firmware images uploaded by FND
     */
    if (runImageFileName[0] == 0)
    {
        if (MEMISZERO_A(runImageFileHash))
        {
            initialize_file_hash();
        }
        if (sizeof(baseFileName) + sizeof(baseLibraryVersion) + sizeof(baseCsmpVersion) + 16 >= sizeof(runImageFileName))
        {
            R_LOG_ERR("File name initialization failed: File name too long");
            strncpy(runImageFileName, baseFileName, sizeof(runImageFileName));
            runImageFileName[sizeof(runImageFileName) - 1] = '\0';
        }
        R_memcpy(runImageFileName, baseFileName, sizeof(baseFileName));
        size_t j = sizeof(baseFileName) - 1;
        runImageFileName[j++] = '-';
        R_memcpy(&runImageFileName[j], baseLibraryVersion, sizeof(baseLibraryVersion));
        j += sizeof(baseLibraryVersion) - 1;
        runImageFileName[j++] = '-';
        R_memcpy(&runImageFileName[j], baseCsmpVersion, sizeof(baseCsmpVersion));
        j += sizeof(baseCsmpVersion) - 1;
        runImageFileName[j++] = '-';
        for (size_t i = 0; i < 8; i++) {
            uint8_t b = runImageFileHash[i];
            runImageFileName[j++] = hex_digit(b >> 4);
            runImageFileName[j++] = hex_digit(b);
        }
        runImageFileName[j] = '\0';
    }
}

static void initialize_file_size()
{
    // Initialize the file size through the attribute stored in flash
    r_result_t flashRes = R_Flash_Read(R_FLASH_FIRMWARE_SIZE, &runImageFileSize, sizeof(runImageFileSize));
    if (flashRes != R_RESULT_SUCCESS)
    {
        R_LOG_ERR("Unable to read the firmware size from flash");
    }
}
#endif

void* firmwareImageInfo_get(tlvid_t tlvid, uint32_t* num)
{
#if R_FWUP_SUPPORT
    (void)tlvid;
    *num = 0;
    R_LOG_DBG("## firmwareImageInfo_get: GET for TLV %{u32}.", tlvid.type);

    R_LOG_DBG("## firmwareImageInfo_get: Reading firmware image info for running image");
    Firmware_Image_Info* imageInfo = &g_firmwareImageInfo[RUN_IMAGE];
    MEMZERO_S(imageInfo);
    (*num)++;

    // Index
    imageInfo->has_index = true;
    imageInfo->index = RUN_IMAGE + 1;
    // Filehash
    imageInfo->has_filehash = true;
    if (MEMISZERO_A(runImageFileHash))
    {
        initialize_file_hash();
        initialize_file_name();
        initialize_file_size();
    }
    MEMCPY_A(imageInfo->filehash.data, runImageFileHash);
    imageInfo->filehash.len = SHA256_HASH_SIZE;
    // Filename
    imageInfo->has_filename = true;
    strncpy(imageInfo->filename, runImageFileName, sizeof(imageInfo->filename));
    imageInfo->filename[sizeof(imageInfo->filename) - 1] = '\0';   // Ensure null-termination of filename string
    // Version
    imageInfo->has_version = true;
    strncpy(imageInfo->version, baseCsmpVersion, sizeof(imageInfo->version));
    imageInfo->version[sizeof(imageInfo->version) - 1] = '\0';   // Ensure null-termination of version string
    // Filesize
    if (runImageFileSize != 0)
    {
        imageInfo->has_filesize = true;
        imageInfo->filesize = runImageFileSize;
    }
    else
    {
        imageInfo->has_filesize = false;
    }
    // Blocksize
    imageInfo->has_blocksize = false;
    // Blockcount
    imageInfo->blockcnt = 0xFFFFFFFF;
    // Bitmap
    imageInfo->has_bitmap = false;
    // Default image?
    imageInfo->has_isdefault = true;
    imageInfo->isdefault = false;
    // Running image?
    imageInfo->has_isrunning = true;
    imageInfo->isrunning = true;
    // Loadtime
    imageInfo->has_loadtime = false;
    // Hardware Id
    imageInfo->has_hwinfo = true;
    imageInfo->hwinfo.has_hwid = true;
    strncpy(imageInfo->hwinfo.hwid, baseHwId, sizeof(imageInfo->hwinfo.hwid));
    imageInfo->hwinfo.hwid[sizeof(imageInfo->hwinfo.hwid) - 1] = '\0';   // Ensure null-termination of hardware string
    // Vendor Hardware Id
    imageInfo->hwinfo.has_vendorhwid = false;
    // Kernel version
    imageInfo->has_kernelversion = false;
    // Subkernel version
    imageInfo->has_subkernelversion = false;
    // Loader error
    imageInfo->has_loaderrorcode = false;
    // Subloader error
    imageInfo->has_subloaderrorcode = false;
    // Download status
    imageInfo->status = FWHDR_STATUS_COMPLETE;

    R_LOG_DBG("## firmwareImageInfo_get: Reading firmware image info for upload image");
    imageInfo = &g_firmwareImageInfo[UPLOAD_IMAGE];
    memset(imageInfo, 0, sizeof(Firmware_Image_Info));
    (*num)++;

    // Index
    imageInfo->has_index = true;
    imageInfo->index = UPLOAD_IMAGE + 1;
    // Filehash
    imageInfo->has_filehash = true;
    imageInfo->filehash.len = sizeof(uploadSlotHdr.filehash);
    MEMCPY_A(imageInfo->filehash.data, uploadSlotHdr.filehash);
    // Filename
    imageInfo->has_filename = true;
    strncpy(imageInfo->filename, uploadSlotHdr.filename, sizeof(imageInfo->filename));
    imageInfo->filename[sizeof(imageInfo->filename) - 1] = '\0';
    // Version
    imageInfo->has_version = true;
    strncpy(imageInfo->version, uploadSlotHdr.version, sizeof(imageInfo->version));
    imageInfo->version[sizeof(imageInfo->version) - 1] = '\0';
    // Filesize
    imageInfo->has_filesize = true;
    imageInfo->filesize = uploadSlotHdr.filesize;
    // Blocksize
    imageInfo->has_blocksize = true;
    imageInfo->blocksize = uploadSlotHdr.blocksize;
    // Blockcount
    imageInfo->blockcnt = uploadSlotHdr.blockcnt;
    // Bitmap
    if (uploadSlotHdr.status == (uint32_t) FWHDR_STATUS_DOWNLOAD)
    {
        imageInfo->has_bitmap = true;
        imageInfo->bitmap.len =
                    (((uploadSlotHdr.filesize + uploadSlotHdr.blocksize - 1u) / uploadSlotHdr.blocksize) + 7u) / 8u;
        for (uint8_t i = 0; i < imageInfo->bitmap.len; i++)
        {
            imageInfo->bitmap.data[i] = reverse_bits8(fwupState.imageBitmap[sizeof(fwupState.imageBitmap) - i - 1]);
        }
    }
    else
    {
        imageInfo->has_bitmap = false;
    }
    // Default image?
    imageInfo->has_isdefault = true;
    imageInfo->isdefault = false;
    // Running image?
    imageInfo->has_isrunning = true;
    imageInfo->isrunning = false;
    // Loadtime
    if (curloadtime != 0)
    {
        imageInfo->has_loadtime = true;
        imageInfo->loadtime = curloadtime;
    }
    // Hardware Id
    imageInfo->has_hwinfo = true;
    imageInfo->hwinfo.has_hwid = true;
    strncpy(imageInfo->hwinfo.hwid, uploadSlotHdr.hwid, sizeof(imageInfo->hwinfo.hwid));
    imageInfo->hwinfo.hwid[sizeof(imageInfo->hwinfo.hwid) - 1] = '\0';
    // Vendor Hardware Id
    imageInfo->hwinfo.has_vendorhwid = false;
    // Kernel version
    imageInfo->has_kernelversion = false;
    // Subkernel version
    imageInfo->has_subkernelversion = false;
    // Loader error
    imageInfo->has_loaderrorcode = false;
    // Subloader error
    imageInfo->has_subloaderrorcode = false;
    // Download status
    imageInfo->status = uploadSlotHdr.status;

    R_LOG_DBG("## firmwareImageInfo_get: GET for TLV %{u32} done.", tlvid.type);
    return &g_firmwareImageInfo;
#else
    *num = 0;
    return NULL;
#endif
}

void rebootRequest_post(tlvid_t tlvid, Reboot_Request* tlv)
{
#if R_FWUP_SUPPORT
    switch (tlv->flag)
    {
        case REBOOT:
            R_LOG_DBG("Rebooting system...\n");
            R_FWUP_SoftwareReset();
            // Should not reach here
            break;
        default:
            R_LOG_ERR("csmp rebootRequest: Reboot flag not supported!");
    }
#else
    R_LOG_ERR("csmp rebootRequest: Reboot not supported!");
#endif
}