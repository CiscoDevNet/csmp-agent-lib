#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "CsmpAgentLib_sample_tlvs.h"
#include "CsmpAgentLib_sample.h"
#include "csmp_service.h"
#include "csmp_info.h"
#include "osal.h"
#include <ti/drivers/dpl/ClockP.h>
#include "net_interface.h"
#include "net_rpl.h"
#include "nwk_stats_api.h"
#include "mesh_system.h"
#include "mbed_config.h"
#include "nsconfig.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_common.h"
#include "6LoWPAN/ws/ws_cfg_settings.h"
#include "6LoWPAN/ws/ws_bootstrap.h"
#include "ws_management_api.h"
/* DeviceFamily.h must precede any DeviceFamily_constructPath() usage */
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/sys_ctrl.h)
#include <ti_drivers_config.h>   /* CONFIG_NVSEXTERNAL */
#include <ti/drivers/NVS.h>      /* Off-Chip OAD secondary slot via NVSSPI25X */

/* ---- Global TLV structs ---- */
Hardware_Desc g_hardwareDesc = HARDWARE_DESC_INIT;
Interface_Desc g_interfaceDesc[interface_max_num] = {INTERFACE_DESC_INIT};
IP_Address g_ipAddress[ipaddress_max_num] = {IPADDRESS_INIT};
IP_Route g_ipRoute = IPROUTE_INIT;
Current_Time g_currentTime = CURRENT_TIME_INIT;
Up_Time g_upTime = UPTIME_INIT;
Interface_Metrics g_interfaceMetrics[interface_max_num] = {INTERFACE_METRICS_INIT};
IPRoute_RPLMetrics g_iprouteRplmetrics = IPROUTE_RPLMETRICS_INIT;
WPAN_Status g_wpanStatus = WPANSTATUS_INIT;
RPL_Instance g_rplInstance = RPLINSTANCE_INIT;
Transfer_Request g_transferRequest = TRANSFER_REQUEST_INIT;
Image_Block g_imageBlock = IMAGE_BLOCK_INIT;
Load_Request g_loadRequest = LOAD_REQUEST_INIT;
Cancel_Load_Request g_cancelLoadRequest = CANCEL_LOAD_REQUEST_INIT;
Set_Backup_Request g_setBackupRequest = SET_BACKUP_REQUEST_INIT;
Firmware_Image_Info g_firmwareImageInfo[CSMP_FWMGMT_ACTIVE_SLOTS] = {FIRMWARE_IMAGE_INFO_INIT};
Signature_Settings g_SignatureSettings = SIGNATURE_SETTINGS_INIT;
Vendor_Tlv g_vendorTlv[VENDOR_MAX_SUBTYPES] = {VENDOR_TLV_INIT};
Csmp_Slothdr g_slothdr[CSMP_FWMGMT_ACTIVE_SLOTS] = {0};

/* EUI64 — filled in application.c before sample_data_init() */
uint8_t g_eui64[8] = {0};
uint32_t g_init_time = 0; /* seconds since boot at Wi-SUN join — set in application.c */
int8_t g_tx_power_dbm = 0;  /* configured TX power in dBm — set in application.c */
bool g_reboot_request = false;

/* OTA state */
bool g_downloadbusy = false;
bool g_initxfer = false;
bool g_initload = false;
uint32_t g_curloadtime = 0;
uint32_t g_curloadslot = 0xFFu;
uint32_t g_curbackupslot = 0xFFu;

dev_config_t g_devconfig;
csmp_handle_t g_csmp_handle;

/* Forward declaration — OTA load timer callback */
static void loadreq_timer_fired(void);
static void ti_ota_activate(void);

extern int8_t interface_id; /* set by application.c after Wi-SUN join */
extern volatile bool connectedFlg;
extern int64_t g_ti_csmp_time_offset_sec;

/* ---- pubkey_get — binary key, no null terminator ---- */
#define PUBLIC_KEY_LEN 65
static const char pubkey[PUBLIC_KEY_LEN] = {
    0x04, 0x23, 0xD2, 0x83, 0x45, 0xE8, 0xD5, 0xDF, 0x86, 0x9D, 0x6E, 0xE7, 0x58,
    0x0D, 0xC1, 0x8F, 0x35, 0x9D, 0x57, 0xB1, 0x3D, 0x50, 0x4A, 0x16, 0x01, 0x15,
    0xC4, 0x81, 0x19, 0xB0, 0xE6, 0x60, 0xB8, 0x64, 0x14, 0x01, 0x5D, 0x56, 0x83,
    0xBE, 0xE1, 0x85, 0x98, 0xCB, 0x90, 0xE1, 0xF7, 0x9B, 0xF4, 0x33, 0x5A, 0x4B,
    0x29, 0xAD, 0x35, 0x69, 0x9B, 0x4F, 0xDC, 0x42, 0x7F, 0xEB, 0xC2, 0x99, 0xA5};
void pubkey_get(char *key)
{
    memcpy(key, pubkey, PUBLIC_KEY_LEN);
}

void *hardware_desc_get(uint32_t *num)
{
    *num = 1;
    g_hardwareDesc.has_entphysicalindex = true;
    g_hardwareDesc.entphysicalindex = 1;
    g_hardwareDesc.has_entphysicaldescr = true;

    snprintf(g_hardwareDesc.entphysicaldescr, sizeof(g_hardwareDesc.entphysicaldescr), "TI CC13xx Wi-SUN FAN Router Node");

    g_hardwareDesc.has_entphysicalclass = true;
    g_hardwareDesc.entphysicalclass = CLASS_MODULE;
    g_hardwareDesc.has_entphysicalname = true;

    snprintf(g_hardwareDesc.entphysicalname, sizeof(g_hardwareDesc.entphysicalname), "wisun0");

    g_hardwareDesc.has_entphysicalhardwarerev = true;

    snprintf(g_hardwareDesc.entphysicalhardwarerev, sizeof(g_hardwareDesc.entphysicalhardwarerev), "1.0");

    g_hardwareDesc.has_entphysicalfirmwarerev = true;

    memcpy(g_hardwareDesc.entphysicalfirmwarerev, g_slothdr[RUN_IMAGE].version, sizeof(g_hardwareDesc.entphysicalfirmwarerev));

    g_hardwareDesc.has_entphysicalserialnum = true;

    snprintf(g_hardwareDesc.entphysicalserialnum, sizeof(g_hardwareDesc.entphysicalserialnum), "%02X%02X%02X%02X%02X%02X%02X%02X",
             g_eui64[0], g_eui64[1], g_eui64[2], g_eui64[3],
             g_eui64[4], g_eui64[5], g_eui64[6], g_eui64[7]);

    g_hardwareDesc.has_entphysicalmfgname = true;

    snprintf(g_hardwareDesc.entphysicalmfgname, sizeof(g_hardwareDesc.entphysicalmfgname), "Texas Instruments");

    g_hardwareDesc.has_entphysicalmodelname = true;

    snprintf(g_hardwareDesc.entphysicalmodelname, sizeof(g_hardwareDesc.entphysicalmodelname), "CC13xx");

    g_hardwareDesc.has_entphysicalfunction = true;
    g_hardwareDesc.entphysicalfunction = FUNCTION_RE;

    return &g_hardwareDesc;
}

void *interface_desc_get(uint32_t *num)
{
    *num = 1;
    memset(g_interfaceDesc, 0, sizeof(g_interfaceDesc));
    /* Wi-SUN only — FND NPEs when an interface has no matching IPAddress entry */
    g_interfaceDesc[0].has_ifindex = true;
    g_interfaceDesc[0].ifindex = 2;
    g_interfaceDesc[0].has_ifname = true;
    snprintf(g_interfaceDesc[0].ifname, sizeof(g_interfaceDesc[0].ifname), "wisun0");
    g_interfaceDesc[0].has_ifdescr = true;
    snprintf(g_interfaceDesc[0].ifdescr, sizeof(g_interfaceDesc[0].ifdescr), "IEEE 802.15.4 Wi-SUN FAN");
    g_interfaceDesc[0].has_iftype = true;
    g_interfaceDesc[0].iftype = 259;
    g_interfaceDesc[0].has_ifphysaddress = true;
    g_interfaceDesc[0].ifphysaddress.len = 8u;
    memcpy(g_interfaceDesc[0].ifphysaddress.data, g_eui64, 8u);
    return g_interfaceDesc;
}

void *ipaddress_get(uint32_t *num)
{
    uint8_t addr[16];
    uint32_t cnt = 0;
    memset(g_ipAddress, 0, sizeof(g_ipAddress));
    nanostack_lock();
    /* Report GUA only — Cisco FND NPEs when processing link-local (fe80) because
     * FND has no managed network context for link-local space. Cisco guidance
     * explicitly requires "the same GUA" in TLV16. */
    if (interface_id >= 0 && arm_net_address_get(interface_id, ADDR_IPV6_GP, addr) == 0)
    {
        g_ipAddress[cnt].has_ipaddressindex = true;
        g_ipAddress[cnt].ipaddressindex = 2;
        g_ipAddress[cnt].has_ipaddressaddrtype = true;
        g_ipAddress[cnt].ipaddressaddrtype = IPV6;
        g_ipAddress[cnt].has_ipaddressaddr = true;
        g_ipAddress[cnt].ipaddressaddr.len = 16u;
        memcpy(g_ipAddress[cnt].ipaddressaddr.data, addr, 16u);
        g_ipAddress[cnt].has_ipaddressifindex = true;
        g_ipAddress[cnt].ipaddressifindex = 2;
        g_ipAddress[cnt].has_ipaddresstype = true;
        g_ipAddress[cnt].ipaddresstype = UNICAST;
        /* ip_origin_t has a gap (no value 3): DHCP=4, LINKLAYER=5.
         * RFC 4293 maps: dhcp=3, linklayer=4, random=5.
         * FND follows RFC 4293: our LINKLAYER(5) looks like "random" to FND.
         * Send DHCP(4) which equals RFC 4293 linklayer(4) for an EUI-64 GUA. */
        g_ipAddress[cnt].has_ipaddressorigin = true;
        g_ipAddress[cnt].ipaddressorigin = DHCP;
        g_ipAddress[cnt].has_ipaddressstatus = true;
        g_ipAddress[cnt].ipaddressstatus = 1;
        g_ipAddress[cnt].has_ipaddresspfxlen = true;
        g_ipAddress[cnt].ipaddresspfxlen = 64u;
        cnt++;
    }
    nanostack_unlock();
    *num = cnt;
    return (cnt > 0) ? g_ipAddress : NULL;
}

/* Discover the first active RPL instance and read its DODAG info.
 * Must be called while holding the nanostack lock.
 * Returns 1 on success, 0 on failure — same semantics as rpl_read_dodag_info. */
static uint8_t rpl_read_dodag_info_auto(rpl_dodag_info_t *dodag_ptr)
{
    uint8_t buf[8];
    uint8_t cnt = rpl_instance_list_read(buf, sizeof(buf));
    if (cnt == 0)
        return 0u;
    return rpl_read_dodag_info(dodag_ptr, buf[0]);
}

void *iproute_get(uint32_t *num)
{
    *num = 1;
    memset(&g_ipRoute, 0, sizeof(g_ipRoute));
    g_ipRoute.has_inetcidrrouteindex = true;
    g_ipRoute.inetcidrrouteindex = 1;
    g_ipRoute.has_inetcidrroutedesttype = true;
    g_ipRoute.inetcidrroutedesttype = IPV6;
    g_ipRoute.has_inetcidrroutedest = true;
    g_ipRoute.inetcidrroutedest.len = 16u;
    g_ipRoute.has_inetcidrroutepfxlen = true;
    g_ipRoute.inetcidrroutepfxlen = 0u;
    g_ipRoute.has_inetcidrrouteifindex = true;
    g_ipRoute.inetcidrrouteifindex = 2;
    g_ipRoute.has_inetcidrroutenexthoptype = true;
    g_ipRoute.has_inetcidrroutenexthop = true;
    g_ipRoute.inetcidrroutenexthop.len = 16u;
    rpl_dodag_info_t info = {0};
    nanostack_lock();
    int rpl_ret = rpl_read_dodag_info_auto(&info);
    nanostack_unlock();
    /* parent_flags is an enum value: RPL_ROOT_PARENT=0, RPL_PRIMARY_PARENT_SET=1,
     * RPL_SECONDARY_PARENT_SET=2. Any non-zero value means primary_parent is valid. */
    if (rpl_ret != 0 && info.parent_flags != RPL_ROOT_PARENT)
    {
        g_ipRoute.inetcidrroutenexthoptype = IPV6;
        memcpy(g_ipRoute.inetcidrroutenexthop.data, info.primary_parent, 16u);
    }
    else
    {
        g_ipRoute.inetcidrroutenexthoptype = IPV6;
        memset(g_ipRoute.inetcidrroutenexthop.data, 0, 16u);
    }
    return &g_ipRoute;
}

void *currenttime_get(uint32_t *num)
{
    struct timeval tv = {0};
    *num = 1;
    osal_gettime(&tv, NULL);
    memset(&g_currentTime, 0, sizeof(g_currentTime));
    g_currentTime.has_posix = true;
    g_currentTime.posix = (uint32_t)tv.tv_sec;
    return &g_currentTime;
}

void currenttime_post(Current_Time *tlv)
{
    struct timeval tv = {0};
    if (tlv && tlv->has_posix)
    {
        tv.tv_sec = (time_t)tlv->posix;
        osal_settime(&tv, NULL);
    }
}

void *uptime_get(uint32_t *num)
{
    *num = 1;
    memset(&g_upTime, 0, sizeof(g_upTime));
    uint32_t ticks = ClockP_getSystemTicks();
    uint32_t us_pt = ClockP_getSystemTickPeriod();
    uint32_t now_s = (uint32_t)(((uint64_t)ticks * us_pt) / 1000000ULL);
    g_upTime.has_sysuptime = true;
    g_upTime.sysuptime = now_s - g_init_time; /* g_init_time in seconds */
    return &g_upTime;
}

void *interface_metrics_get(uint32_t *num)
{
    *num = 1;
    memset(g_interfaceMetrics, 0, sizeof(g_interfaceMetrics));
    nwk_stats_t s = {0};
    nanostack_lock();
    protocol_stats_start(&s);
    nanostack_unlock();
    g_interfaceMetrics[0].has_ifindex = true;
    g_interfaceMetrics[0].ifindex = 2;  /* fixed: must match interfaceDesc[1].ifindex = 2 */
    g_interfaceMetrics[0].has_ifadminstatus = true;
    g_interfaceMetrics[0].ifadminstatus = IF_ADMIN_STATUS_UP;
    g_interfaceMetrics[0].has_ifoperstatus = true;
    g_interfaceMetrics[0].ifoperstatus = connectedFlg ? IF_OPER_STATUS_UP : IF_OPER_STATUS_DOWN;
    g_interfaceMetrics[0].has_ifinoctets = true;
    g_interfaceMetrics[0].ifinoctets = s.ip_rx_bytes;
    g_interfaceMetrics[0].has_ifoutoctets = true;
    g_interfaceMetrics[0].ifoutoctets = s.ip_tx_bytes;
    g_interfaceMetrics[0].has_ifindiscards = true;
    g_interfaceMetrics[0].ifindiscards = s.ip_rx_drop + s.frag_rx_errors;
    g_interfaceMetrics[0].has_ifouterrors = true;
    g_interfaceMetrics[0].ifouterrors = s.ip_no_route + s.adapt_layer_tx_congestion_drop + s.frag_tx_errors;
    return g_interfaceMetrics;
}

/* Cache last valid RPL metrics so FND sees stable values during RPL re-convergence
 * (Poison count events briefly set rank=0xFFFF, which would send empty/wrong data). */
static IPRoute_RPLMetrics g_iprouteRplmetrics_cache = IPROUTE_RPLMETRICS_INIT;
static bool g_iprouteRplmetrics_valid = false;

void *iproute_rplmetrics_get(uint32_t *num)
{
    *num = 1;
    memset(&g_iprouteRplmetrics, 0, sizeof(g_iprouteRplmetrics));
    rpl_dodag_info_t info = {0};
    ws_stack_info_t ws_info = {0};
    ws_neighbour_info_t nbr_buf[8] = {0};
    int nbr_cnt = 0;

    nanostack_lock();
    int rpl_ret2 = rpl_read_dodag_info_auto(&info);
    if (interface_id >= 0)
    {
        ws_stack_info_get(interface_id, &ws_info);
        int total = ws_neighbor_info_get(interface_id, NULL, 0);
        if (total > 0)
        {
            int fill = (total > 8) ? 8 : total;
            ws_neighbor_info_get(interface_id, nbr_buf, (uint16_t)fill);
            nbr_cnt = fill;
        }
    }
    nanostack_unlock();

    if (rpl_ret2 == 0)
    {
        return g_iprouteRplmetrics_valid ? &g_iprouteRplmetrics_cache : &g_iprouteRplmetrics;
    }
    if (info.curent_rank == 0xFFFF || info.curent_rank == 0)
    {
        return g_iprouteRplmetrics_valid ? &g_iprouteRplmetrics_cache : &g_iprouteRplmetrics;
    }

    /* Count direct neighbors for dagSize */
    uint32_t parent_cnt = 0u, child_cnt = 0u;
    uint16_t primary_etx = 0xFFFFu;
    for (int i = 0; i < nbr_cnt; i++)
    {
        if (nbr_buf[i].type == WS_PRIMARY_PARENT || nbr_buf[i].type == WS_SECONDARY_PARENT)
        {
            parent_cnt++;
            if (nbr_buf[i].type == WS_PRIMARY_PARENT && nbr_buf[i].etx != 0xFFFFu)
                primary_etx = nbr_buf[i].etx;
        }
        else if (nbr_buf[i].type == WS_CHILD)
            child_cnt++;
    }

    g_iprouteRplmetrics.has_inetcidrrouteindex = true;
    g_iprouteRplmetrics.inetcidrrouteindex = 1;
    g_iprouteRplmetrics.has_instanceindex = true;
    g_iprouteRplmetrics.instanceindex = info.instance_id;
    g_iprouteRplmetrics.has_rank = true;
    g_iprouteRplmetrics.rank = info.curent_rank;
    g_iprouteRplmetrics.has_dagsize = true;
    g_iprouteRplmetrics.dagsize = parent_cnt + child_cnt + 1u;

    /* dag_min_hop_rank_inc = MinHopRankIncrease (128 in Wi-SUN). Root rank equals this
     * value, so hops from root = (rank / mhri) - 1. */
    {
        uint16_t mhri = (info.dag_min_hop_rank_inc > 0u) ? info.dag_min_hop_rank_inc : 128u;
        uint32_t rank_steps = (uint32_t)(info.curent_rank / mhri);
        g_iprouteRplmetrics.has_hops = true;
        g_iprouteRplmetrics.hops = (rank_steps > 1u) ? (rank_steps - 1u) : 0u;

        /* pathetx: use actual measured routing cost (ETX to border router) from ws_stack_info
         * when available — more accurate than rank-based approximation. */
        g_iprouteRplmetrics.has_pathetx = true;
        if (ws_info.routing_cost > 0u && ws_info.routing_cost != 0xFFFFu)
            g_iprouteRplmetrics.pathetx = ws_info.routing_cost;
        else
            g_iprouteRplmetrics.pathetx = (info.curent_rank > mhri) ?
                (uint16_t)(info.curent_rank - mhri) : 0u;

        /* linketx: use primary parent's measured ETX from neighbor table when available. */
        g_iprouteRplmetrics.has_linketx = true;
        if (primary_etx != 0xFFFFu)
            g_iprouteRplmetrics.linketx = primary_etx;
        else
            g_iprouteRplmetrics.linketx = (info.curent_rank > info.primary_parent_rank) ?
                (uint16_t)(info.curent_rank - info.primary_parent_rank) : 0u;
    }

    /* RSSI: Wi-SUN RSL encoding is 0–254 where dBm = rsl - 174.
     * rsl_in  = RSSI of parent's signal as received at this node (forward path).
     * rsl_out = estimated RSSI of our signal as received at parent (reverse path). */
    /* Wi-SUN RSL: 0 = -174 dBm, 254 = +80 dBm. Value 255 (0xFF) is nanostack's
     * sentinel for "not yet measured" — must be excluded from both bounds checks. */
    if (ws_info.rsl_in > 0u && ws_info.rsl_in < 255u)
    {
        g_iprouteRplmetrics.has_rssiforward = true;
        g_iprouteRplmetrics.rssiforward = (int32_t)ws_info.rsl_in - 174;
    }
    if (ws_info.rsl_out > 0u && ws_info.rsl_out < 255u)
    {
        g_iprouteRplmetrics.has_rssireverse = true;
        g_iprouteRplmetrics.rssireverse = (int32_t)ws_info.rsl_out - 174;
    }

    g_iprouteRplmetrics_cache = g_iprouteRplmetrics;
    g_iprouteRplmetrics_valid = true;
    return &g_iprouteRplmetrics;
}

void *wpanstatus_get(uint32_t *num)
{
    *num = 1;
    memset(&g_wpanStatus, 0, sizeof(g_wpanStatus));
    nanostack_lock();
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get(IF_6LoWPAN);
    if (!cur || !cur->ws_info)
    {
        nanostack_unlock();
        return &g_wpanStatus;
    }
    ws_info_t *ws = cur->ws_info;
    g_wpanStatus.has_ifindex = true;
    g_wpanStatus.ifindex = 2;  /* fixed: must match interfaceDesc[1].ifindex = 2 */
    g_wpanStatus.has_panid = true;
    g_wpanStatus.panid = ws->network_pan_id;
    if (ws->cfg)
    {
        size_t nlen = strlen(ws->cfg->gen.network_name);
        if (nlen > 0)
        {
            g_wpanStatus.has_ssid = true;
            g_wpanStatus.ssid.len = (uint32_t)nlen;
            memcpy(g_wpanStatus.ssid.data, ws->cfg->gen.network_name, nlen);
        }
    }
    g_wpanStatus.has_txpower = true;
    g_wpanStatus.txpower = (int32_t)g_tx_power_dbm;
    {
        ws_neighbour_info_t nbr_buf[8] = {0};
        int total = ws_neighbor_info_get(interface_id, NULL, 0);
        uint32_t nbr_cnt = 0u;
        if (total > 0)
        {
            int fill = (total > 8) ? 8 : total;
            ws_neighbor_info_get(interface_id, nbr_buf, (uint16_t)fill);
            for (int i = 0; i < fill; i++)
            {
                if (nbr_buf[i].type == WS_PRIMARY_PARENT ||
                    nbr_buf[i].type == WS_SECONDARY_PARENT ||
                    nbr_buf[i].type == WS_CHILD)
                    nbr_cnt++;
            }
        }
        g_wpanStatus.has_dagsize = true;
        g_wpanStatus.dagsize = nbr_cnt + 1u;
    }
    g_wpanStatus.has_master = true;
    g_wpanStatus.master = false;
    g_wpanStatus.has_dot1xenabled = true;
    g_wpanStatus.dot1xenabled = false;
    g_wpanStatus.has_securitylevel = true;
    g_wpanStatus.securitylevel = IEEE154_SEC_MIC_32;
    rpl_dodag_info_t wpan_info = {0};
    int wpan_rpl_ret = rpl_read_dodag_info_auto(&wpan_info);
    nanostack_unlock();
    if (wpan_rpl_ret != 0)
    {
        g_wpanStatus.has_rank = true;
        g_wpanStatus.rank = wpan_info.curent_rank;
        g_wpanStatus.has_beaconvalid = true;
        g_wpanStatus.beaconvalid = true;
        g_wpanStatus.has_beaconversion = true;
        g_wpanStatus.beaconversion = wpan_info.version_num;
        g_wpanStatus.has_metric = true;
        g_wpanStatus.metric = wpan_info.curent_rank / 256u;
    }
    return &g_wpanStatus;
}

void *rplinstance_get(uint32_t *num)
{
    *num = 1;
    memset(&g_rplInstance, 0, sizeof(g_rplInstance));
    rpl_dodag_info_t rpl_info = {0};
    ws_neighbour_info_t nbr_buf[8] = {0};
    int nbr_cnt = 0;

    nanostack_lock();
    int rpl_ret3 = rpl_read_dodag_info_auto(&rpl_info);
    if (interface_id >= 0)
    {
        int total = ws_neighbor_info_get(interface_id, NULL, 0);
        if (total > 0)
        {
            int fill = (total > 8) ? 8 : total;
            ws_neighbor_info_get(interface_id, nbr_buf, (uint16_t)fill);
            nbr_cnt = fill;
        }
    }
    nanostack_unlock();

    if (rpl_ret3 == 0)
        return &g_rplInstance;

    uint32_t parent_cnt = 0u, child_cnt = 0u;
    for (int i = 0; i < nbr_cnt; i++)
    {
        if (nbr_buf[i].type == WS_PRIMARY_PARENT || nbr_buf[i].type == WS_SECONDARY_PARENT)
            parent_cnt++;
        else if (nbr_buf[i].type == WS_CHILD)
            child_cnt++;
    }

    g_rplInstance.has_instanceindex = true;
    g_rplInstance.instanceindex = 1;
    g_rplInstance.has_instanceid = true;
    g_rplInstance.instanceid = rpl_info.instance_id;
    g_rplInstance.has_dodagid = true;
    g_rplInstance.dodagid.len = 16u;
    memcpy(g_rplInstance.dodagid.data, rpl_info.dodag_id, 16u);
    g_rplInstance.has_dodagversionnumber = true;
    g_rplInstance.dodagversionnumber = rpl_info.version_num;
    g_rplInstance.has_rank = true;
    g_rplInstance.rank = rpl_info.curent_rank;
    g_rplInstance.has_parentcount = true;
    g_rplInstance.parentcount = (int32_t)parent_cnt;
    g_rplInstance.has_dagsize = true;
    g_rplInstance.dagsize = parent_cnt + child_cnt + 1u;
    return &g_rplInstance;
}

void *transferRequest_get(tlvid_t id, uint32_t *n)
{
    (void)id;

    /* Return transfer request only if a download is in progress */
    if (g_slothdr[UPLOAD_IMAGE].status != FWHDR_STATUS_DOWNLOAD)
    {
        g_transferRequest.status = g_slothdr[UPLOAD_IMAGE].status;
        *n = 1;
        return &g_transferRequest;
    }

    memcpy(g_transferRequest.filehash.data, g_slothdr[UPLOAD_IMAGE].filehash, SHA256_HASH_SIZE);
    g_transferRequest.filehash.len = SHA256_HASH_SIZE;
    strncpy(g_transferRequest.filename, g_slothdr[UPLOAD_IMAGE].filename, FILE_NAME_SIZE);
    strncpy(g_transferRequest.version, g_slothdr[UPLOAD_IMAGE].version, VERSION_SIZE);
    strncpy(g_transferRequest.hwinfo.hwid, g_slothdr[UPLOAD_IMAGE].hwid, HWID_SIZE);
    g_transferRequest.filesize = g_slothdr[UPLOAD_IMAGE].filesize;
    g_transferRequest.blocksize = g_slothdr[UPLOAD_IMAGE].blocksize;
    g_transferRequest.report_int_min = g_slothdr[UPLOAD_IMAGE].reportintervalmin;
    g_transferRequest.report_int_max = g_slothdr[UPLOAD_IMAGE].reportintervalmax;
    g_transferRequest.status = g_slothdr[UPLOAD_IMAGE].status;
    *n = 1;
    return &g_transferRequest;
}

void transferRequest_post(tlvid_t id, Transfer_Request *tlv)
{
    (void)id;
    DPRINTF("OTA: TransferRequest POST\n");

    if (!tlv)
        return;

    /* --- Validation --- */
    if (!tlv->hwinfo.has_hwid)
    {
        tlv->response = RESPONSE_INCOMPATIBLE_HW;
        DPRINTF("OTA: no hwid\n");
        return;
    }
    if (tlv->filehash.len != SHA256_HASH_SIZE)
    {
        tlv->response = RESPONSE_INVALID_REQ;
        DPRINTF("OTA: bad hash len %u\n", (unsigned)tlv->filehash.len);
        return;
    }
    if (tlv->filesize == 0 || tlv->filesize > CSMP_FWMGMT_SLOTIMG_SIZE)
    {
        tlv->response = RESPONSE_FILE_SIZE_TOO_BIG;
        DPRINTF("OTA: bad filesize %lu\n", (unsigned long)tlv->filesize);
        return;
    }
    /* blocksize <= CSMP max (1024) and large enough to fit in 1024-block bitmap */
    if (tlv->blocksize == 0 ||
        tlv->blocksize > BLOCK_SIZE ||
        tlv->blocksize < tlv->filesize / (CSMP_FWMGMT_BLKMAP_CNT * 32U))
    {
        tlv->response = RESPONSE_INVALID_BLOCK_SIZE;
        DPRINTF("OTA: bad blocksize %lu\n", (unsigned long)tlv->blocksize);
        return;
    }
    /* Reject if a load request is already pending for the upload slot */
    if (g_initload && g_curloadslot == UPLOAD_IMAGE)
    {
        tlv->response = RESPONSE_PENDING_REBOOT;
        DPRINTF("OTA: pending reboot\n");
        return;
    }
    /* Reject duplicate transfers */
    if (memcmp(tlv->filehash.data, g_slothdr[UPLOAD_IMAGE].filehash, SHA256_HASH_SIZE) == 0)
    {
        tlv->response = RESPONSE_DUP_XFER;
        DPRINTF("OTA: duplicate (matches upload slot)\n");
        return;
    }
    if (memcmp(tlv->filehash.data, g_slothdr[RUN_IMAGE].filehash, SHA256_HASH_SIZE) == 0)
    {
        tlv->response = RESPONSE_MATCH_RUN_XFER;
        DPRINTF("OTA: matches running image\n");
        return;
    }
    if (memcmp(tlv->filehash.data, g_slothdr[BACKUP_IMAGE].filehash, SHA256_HASH_SIZE) == 0)
    {
        tlv->response = RESPONSE_MATCH_BAK_XFER;
        DPRINTF("OTA: matches backup image\n");
        return;
    }

    /* --- Initialise upload slot --- */
    tlv->has_response = true;
    tlv->response = RESPONSE_OK;
    memcpy(&g_transferRequest, tlv, sizeof(g_transferRequest));
    g_initxfer = true;

    /* Clamp report intervals */
    uint32_t tmin = tlv->report_int_min * 1000U;
    uint32_t tmax = tlv->report_int_max * 1000U;
    tmin = (tmin >= (uint32_t)MIN_REPORT_MIN) ? tmin : (uint32_t)MIN_REPORT_MIN;
    tmax = (tmax >= (uint32_t)MAX_REPORT_MIN) ? tmax : (uint32_t)MAX_REPORT_MIN;
    tmax = (tmax < tmin) ? tmin : tmax;

    /* Clear upload slot header in RAM */
    memset(&g_slothdr[UPLOAD_IMAGE], 0xFF, sizeof(Csmp_Slothdr));

    /* Populate upload slot header */
    memcpy(g_slothdr[UPLOAD_IMAGE].filehash, tlv->filehash.data, SHA256_HASH_SIZE);
    strncpy(g_slothdr[UPLOAD_IMAGE].filename, tlv->filename, FILE_NAME_SIZE);
    strncpy(g_slothdr[UPLOAD_IMAGE].version, tlv->version, VERSION_SIZE);
    strncpy(g_slothdr[UPLOAD_IMAGE].hwid, tlv->hwinfo.hwid, HWID_SIZE);
    g_slothdr[UPLOAD_IMAGE].filesize = tlv->filesize;
    g_slothdr[UPLOAD_IMAGE].blocksize = tlv->blocksize;
    g_slothdr[UPLOAD_IMAGE].blockcnt = (tlv->filesize + tlv->blocksize - 1U) / tlv->blocksize;
    g_slothdr[UPLOAD_IMAGE].filesizelastblk = tlv->filesize % tlv->blocksize;
    if (g_slothdr[UPLOAD_IMAGE].filesizelastblk == 0)
        g_slothdr[UPLOAD_IMAGE].filesizelastblk = tlv->blocksize;
    g_slothdr[UPLOAD_IMAGE].reportintervalmin = tmin;
    g_slothdr[UPLOAD_IMAGE].reportintervalmax = tmax;
    g_slothdr[UPLOAD_IMAGE].status = FWHDR_STATUS_DOWNLOAD;

    /* Erase the external flash secondary slot.
     * ti_flash_erase_slot calls NVS_erase — safe because the secondary
     * slot is in external SPI flash, not in the internal flash where the
     * application code currently runs. */
    if (ti_flash_erase_slot(UPLOAD_IMAGE) != OSAL_SUCCESS)
    {
        tlv->response = RESPONSE_INVALID_REQ;
        g_initxfer = false;
        DPRINTF("OTA: external flash erase failed\n");
        return;
    }

    g_initxfer = false;
    DPRINTF("OTA: upload slot ready, %lu blocks of %lu bytes\n",
            (unsigned long)g_slothdr[UPLOAD_IMAGE].blockcnt,
            (unsigned long)g_slothdr[UPLOAD_IMAGE].blocksize);
}

void imageBlock_post(tlvid_t id, Image_Block *tlv)
{
    (void)id;

    if (!tlv)
        return;

    /* Validate: filehash must match upload slot */
    if (tlv->filehash.len < (uint32_t)MIN_HASH_COMPARE_LEN || tlv->filehash.len > SHA256_HASH_SIZE ||
        memcmp(tlv->filehash.data, g_slothdr[UPLOAD_IMAGE].filehash, tlv->filehash.len) != 0 ||
        tlv->blockdata.len > g_slothdr[UPLOAD_IMAGE].blocksize)
    {
        tlv->retval = false;
        DPRINTF("OTA: ImageBlock validation failed (blocknum=%lu)\n",
                (unsigned long)tlv->blocknum);
        g_downloadbusy = false;
        return;
    }

    memcpy(&g_imageBlock, tlv, sizeof(g_imageBlock));

    uint32_t blocknum = g_imageBlock.blocknum;
    uint32_t word = blocknum >> 5;
    uint32_t bit = 31U - (blocknum & 31U);

    /* Guard: bitmap overflow */
    if (word >= CSMP_FWMGMT_BLKMAP_CNT)
    {
        DPRINTF("OTA: blocknum %lu exceeds bitmap\n", (unsigned long)blocknum);
        g_downloadbusy = false;
        return;
    }

    if (g_initxfer)
    {
        DPRINTF("OTA: transfer still initialising (block %lu)\n", (unsigned long)blocknum);
        g_downloadbusy = false;
        return;
    }

    /* ----- Block already received? Check for download completion ----- */
    uint32_t mapval = g_slothdr[UPLOAD_IMAGE].nblkmap[word];
    if ((mapval & (1UL << bit)) == 0)
    {
        /* Bit already clear = block already written */
        if (g_slothdr[UPLOAD_IMAGE].status == FWHDR_STATUS_COMPLETE)
        {
            DPRINTF("OTA: redundant block %lu after completion\n", (unsigned long)blocknum);
            g_downloadbusy = false;
            return;
        }
        /* Check if all blocks are done */
        bool all_done = true;
        uint32_t full_words = g_slothdr[UPLOAD_IMAGE].blockcnt / 32U;
        for (uint32_t w = 0; w < full_words && all_done; w++)
        {
            if (g_slothdr[UPLOAD_IMAGE].nblkmap[w] != 0)
                all_done = false;
        }
        uint32_t partial_bits = g_slothdr[UPLOAD_IMAGE].blockcnt % 32U;
        if (partial_bits && all_done)
        {
            uint32_t partial_mask = ~((1UL << (32U - partial_bits)) - 1U);
            if (g_slothdr[UPLOAD_IMAGE].nblkmap[full_words] & partial_mask)
                all_done = false;
        }
        if (all_done)
        {
            DPRINTF("OTA: download complete!\n");
            g_slothdr[UPLOAD_IMAGE].status = FWHDR_STATUS_COMPLETE;
            osal_write_slothdr(UPLOAD_IMAGE, &g_slothdr[UPLOAD_IMAGE]);
        }
        g_downloadbusy = false;
        return;
    }

    /* ----- Write this block to flash ----- */
    uint32_t offset = blocknum * g_slothdr[UPLOAD_IMAGE].blocksize;
    uint32_t flash_addr;
    uint8_t *data_ptr;
    uint32_t data_len;

    if (blocknum == 0)
    {
        /*
         * Block 0: strip the 256-byte CSMP image header.
         * CSMP prepends metadata (filename, version, hwid, hash) to the image.
         * This header must NOT be written to flash — Gecko Bootloader uses a GBL
         * file format; TI expects a raw binary starting with the vector table.
         */
        if (g_imageBlock.blockdata.len <= CSMP_IMAGE_HDR_SIZE)
        {
            DPRINTF("OTA: block 0 too small to strip header\n");
            g_downloadbusy = false;
            return;
        }
        flash_addr = CSMP_TI_UPLOAD_SLOT_ADDR;
        data_ptr = g_imageBlock.blockdata.data + CSMP_IMAGE_HDR_SIZE;
        data_len = g_imageBlock.blockdata.len - CSMP_IMAGE_HDR_SIZE;
    }
    else
    {
        /* All subsequent blocks are offset by -CSMP_IMAGE_HDR_SIZE to account for stripped header */
        flash_addr = CSMP_TI_UPLOAD_SLOT_ADDR + offset - CSMP_IMAGE_HDR_SIZE;
        data_ptr = g_imageBlock.blockdata.data;
        data_len = g_imageBlock.blockdata.len;
    }

    /* Write block to external SPI flash via NVS driver (NVSSPI25X).
     * flash_addr is a byte offset within the NVS region (0-based). */
    NVS_Handle h = NVS_open(CONFIG_NVSEXTERNAL, NULL);
    if (h == NULL)
    {
        DPRINTF("OTA: NVS_open failed block %lu\n", (unsigned long)blocknum);
        tlv->retval = false;
        g_downloadbusy = false;
        return;
    }
    int_fast16_t nvsResult = NVS_write(h, flash_addr, data_ptr, data_len,
                                       NVS_WRITE_POST_VERIFY);
    NVS_close(h);

    if (nvsResult != NVS_STATUS_SUCCESS)
    {
        DPRINTF("OTA: NVS_write failed block %lu offset 0x%08lx err=%d\n",
                (unsigned long)blocknum, (unsigned long)flash_addr, (int)nvsResult);
        tlv->retval = false;
        g_downloadbusy = false;
        return;
    }

    tlv->retval = true;

    /* Mark block as received (clear bit in inverted bitmap) */
    mapval ^= (1UL << bit);
    g_slothdr[UPLOAD_IMAGE].nblkmap[word] = mapval;

    DPRINTF("OTA: block %lu written @ 0x%08lx len=%lu map[%lu]=0x%08lx\n",
            (unsigned long)blocknum, (unsigned long)flash_addr,
            (unsigned long)data_len, (unsigned long)word, (unsigned long)mapval);

    /* Persist slot header to NVM every 10 blocks (power-fail resume) */
    if (blocknum && !(blocknum % 10U))
    {
        osal_write_slothdr(UPLOAD_IMAGE, &g_slothdr[UPLOAD_IMAGE]);
        DPRINTF("OTA: slot header persisted at block %lu\n", (unsigned long)blocknum);
    }

    g_downloadbusy = false;
}

void *loadRequest_get(tlvid_t id, uint32_t *n)
{
    (void)id;

    if (!g_initload)
    {
        *n = 0;
        return NULL;
    }

    switch (g_curloadslot)
    {
    case UPLOAD_IMAGE:
        memcpy(g_loadRequest.filehash.data, g_slothdr[UPLOAD_IMAGE].filehash, SHA256_HASH_SIZE);
        break;
    case BACKUP_IMAGE:
        memcpy(g_loadRequest.filehash.data, g_slothdr[BACKUP_IMAGE].filehash, SHA256_HASH_SIZE);
        break;
    default:
        *n = 0;
        return NULL;
    }
    g_loadRequest.filehash.len = SHA256_HASH_SIZE;
    g_loadRequest.loadtime = g_curloadtime;
    *n = 1;
    return &g_loadRequest;
}

void loadRequest_post(tlvid_t id, Load_Request *tlv)
{
    (void)id;
    DPRINTF("OTA: LoadRequest POST\n");

    if (!tlv)
        return;

    uint32_t newloadslot;

    /* Match filehash to a slot */
    if (memcmp(tlv->filehash.data, g_slothdr[UPLOAD_IMAGE].filehash, tlv->filehash.len) == 0)
    {
        if (g_slothdr[UPLOAD_IMAGE].status != FWHDR_STATUS_COMPLETE)
        {
            tlv->response = RESPONSE_INCOMPLETE;
            DPRINTF("OTA: LoadRequest upload image not complete\n");
            return;
        }
        newloadslot = UPLOAD_IMAGE;
    }
    else if (memcmp(tlv->filehash.data, g_slothdr[RUN_IMAGE].filehash, tlv->filehash.len) == 0)
    {
        tlv->response = RESPONSE_IMAGE_RUNNING;
        DPRINTF("OTA: LoadRequest matches running image\n");
        return;
    }
    else if (memcmp(tlv->filehash.data, g_slothdr[BACKUP_IMAGE].filehash, tlv->filehash.len) == 0)
    {
        newloadslot = BACKUP_IMAGE;
    }
    else
    {
        tlv->response = RESPONSE_UNKNOWN_HASH;
        DPRINTF("OTA: LoadRequest unknown hash\n");
        return;
    }

    /* Redundant load request? */
    if (g_initload && newloadslot == g_curloadslot && tlv->loadtime == g_curloadtime)
    {
        DPRINTF("OTA: redundant LoadRequest\n");
        return;
    }

    /* Cancel any previous pending load timer */
    osal_trickle_timer_stop(lrq_timer);
    g_curloadslot = newloadslot;
    g_curloadtime = 0;
    g_initload = true;

    /* Calculate timer delay from loadtime */
    uint32_t delay = MIN_LOAD_DELAY;
    if (tlv->loadtime != 0)
    {
        struct timeval tv = {0};
        osal_gettime(&tv, NULL);
        if (tlv->loadtime > (uint32_t)tv.tv_sec)
        {
            delay = tlv->loadtime - (uint32_t)tv.tv_sec;
        }
        else
        {
            DPRINTF("OTA: LoadRequest loadtime in the past, using min delay\n");
        }
    }

    memcpy(&g_loadRequest, tlv, sizeof(g_loadRequest));
    g_curloadtime = g_loadRequest.loadtime;

    osal_trickle_timer_start(lrq_timer, delay, delay, (trickle_timer_fired_t)loadreq_timer_fired);
    DPRINTF("OTA: LoadRequest timer started, delay=%lu s, slot=%lu\n", (unsigned long)delay, (unsigned long)g_curloadslot);
}

void cancelLoadRequest_post(tlvid_t id, Cancel_Load_Request *tlv)
{
    (void)id;
    DPRINTF("OTA: CancelLoadRequest POST\n");

    if (!tlv)
        return;

    const uint8_t *filehash = NULL;
    switch (g_curloadslot)
    {
    case RUN_IMAGE:
        filehash = g_slothdr[RUN_IMAGE].filehash;
        break;
    case UPLOAD_IMAGE:
        filehash = g_slothdr[UPLOAD_IMAGE].filehash;
        break;
    case BACKUP_IMAGE:
        filehash = g_slothdr[BACKUP_IMAGE].filehash;
        break;
    default:
        if (g_curloadslot != 0xFFu)
        {
            tlv->response = RESPONSE_INVALID_REQ;
        }
        return;
    }

    if (filehash && memcmp(tlv->filehash.data, filehash, SHA256_HASH_SIZE) == 0)
    {
        DPRINTF("OTA: cancel load request valid\n");
        osal_trickle_timer_stop(lrq_timer);
        g_initload = false;
        g_curloadtime = 0;
        g_curloadslot = 0xFFu;
    }
}

void setBackupRequest_post(tlvid_t id, Set_Backup_Request *tlv)
{
    (void)id;
    DPRINTF("OTA: SetBackupRequest POST\n");

    if (!tlv)
        return;

    /* Identify the source image */
    if (memcmp(tlv->filehash.data, g_slothdr[UPLOAD_IMAGE].filehash, tlv->filehash.len) == 0)
    {
        if (g_slothdr[UPLOAD_IMAGE].status != FWHDR_STATUS_COMPLETE)
        {
            tlv->response = RESPONSE_INCOMPLETE;
            DPRINTF("OTA: SetBackup upload not complete\n");
            return;
        }
        g_curbackupslot = UPLOAD_IMAGE;
    }
    else if (memcmp(tlv->filehash.data, g_slothdr[RUN_IMAGE].filehash, tlv->filehash.len) == 0)
    {
        g_curbackupslot = RUN_IMAGE;
    }
    else
    {
        tlv->response = RESPONSE_UNKNOWN_HASH;
        DPRINTF("OTA: SetBackup unknown hash\n");
        return;
    }

    /* Don't overwrite backup while a load is pending from it */
    if (g_initload && g_curloadslot == BACKUP_IMAGE)
    {
        tlv->response = RESPONSE_PENDING_REBOOT;
        DPRINTF("OTA: SetBackup pending reboot on backup\n");
        return;
    }

    /* Copy source → backup slot using osal_copy_firmware() */
    DPRINTF("OTA: copying slot %lu → backup\n", (unsigned long)g_curbackupslot);
    if (osal_copy_firmware(g_curbackupslot, BACKUP_IMAGE, g_slothdr) != OSAL_SUCCESS)
    {
        tlv->response = RESPONSE_INVALID_REQ;
        DPRINTF("OTA: SetBackup copy failed\n");
        g_curbackupslot = 0xFFu;
        return;
    }

    g_slothdr[BACKUP_IMAGE].status = FWHDR_STATUS_COMPLETE;
    osal_write_slothdr(BACKUP_IMAGE, &g_slothdr[BACKUP_IMAGE]);
    g_curbackupslot = 0xFFu;
    DPRINTF("OTA: SetBackup complete\n");
}

void* firmwareImageInfo_get(tlvid_t tlvid, uint32_t *num)
{
    (void)tlvid;
    *num = 0;
    for (uint32_t i = 0; i < CSMP_FWMGMT_ACTIVE_SLOTS; i++)
    {
        memset(&g_firmwareImageInfo[i], 0, sizeof(g_firmwareImageInfo[i]));
        (*num)++;
        g_firmwareImageInfo[i].has_index = true;
        g_firmwareImageInfo[i].index = i + 1u;
        g_firmwareImageInfo[i].has_filehash = true;
        g_firmwareImageInfo[i].filehash.len = SHA256_HASH_SIZE;
        memcpy(g_firmwareImageInfo[i].filehash.data, g_slothdr[i].filehash, SHA256_HASH_SIZE);
        g_firmwareImageInfo[i].has_filename = true;
        strncpy(g_firmwareImageInfo[i].filename, g_slothdr[i].filename, FILE_NAME_SIZE);
        g_firmwareImageInfo[i].has_version = true;
        strncpy(g_firmwareImageInfo[i].version, g_slothdr[i].version, VERSION_SIZE);
        g_firmwareImageInfo[i].has_filesize = true;
        g_firmwareImageInfo[i].filesize = g_slothdr[i].filesize;
        g_firmwareImageInfo[i].has_blocksize = true;
        g_firmwareImageInfo[i].blocksize = g_slothdr[i].blocksize;
        g_firmwareImageInfo[i].blockcnt = g_slothdr[i].blockcnt;
        if (g_slothdr[i].status == FWHDR_STATUS_DOWNLOAD)
        {
            uint32_t blks = (g_slothdr[i].blockcnt + 31u) / 32u;
            if (blks > CSMP_FWMGMT_BLKMAP_CNT)
            {
                blks = CSMP_FWMGMT_BLKMAP_CNT;
            }
            g_firmwareImageInfo[i].has_bitmap = true;
            uint32_t j = 0u;
            for (uint32_t k = 0; k < blks && j < sizeof(g_firmwareImageInfo[i].bitmap.data); k++, j += 4u)
            {
                uint32_t v = ~g_slothdr[i].nblkmap[k];
                g_firmwareImageInfo[i].bitmap.data[j] = (v >> 24u) & 0xFFu;
                g_firmwareImageInfo[i].bitmap.data[j + 1u] = (v >> 16u) & 0xFFu;
                g_firmwareImageInfo[i].bitmap.data[j + 2u] = (v >> 8u) & 0xFFu;
                g_firmwareImageInfo[i].bitmap.data[j + 3u] = v & 0xFFu;
            }
            if (j > ((g_slothdr[i].blockcnt + 7u) / 8u))
            {
                j = (g_slothdr[i].blockcnt + 7u) / 8u;
            }
            g_firmwareImageInfo[i].bitmap.len = j;
        }
        g_firmwareImageInfo[i].has_isdefault = (i == BACKUP_IMAGE);
        g_firmwareImageInfo[i].isdefault = (i == BACKUP_IMAGE);
        g_firmwareImageInfo[i].has_isrunning = (i == RUN_IMAGE);
        g_firmwareImageInfo[i].isrunning = (i == RUN_IMAGE);
        if (g_curloadtime != 0u && g_curloadslot == i)
        {
            g_firmwareImageInfo[i].has_loadtime = true;
            g_firmwareImageInfo[i].loadtime = g_curloadtime;
        }
        g_firmwareImageInfo[i].has_hwinfo = true;
        g_firmwareImageInfo[i].hwinfo.has_hwid = true;
        strncpy(g_firmwareImageInfo[i].hwinfo.hwid, g_slothdr[i].hwid, HWID_SIZE);
        g_firmwareImageInfo[i].status = g_slothdr[i].status;
    }
    return g_firmwareImageInfo;
}

void *signature_settings_get(uint32_t *num)
{
    *num = 1;
    return &g_SignatureSettings;
}

void signature_settings_post(Signature_Settings *tlv)
{
    if (!tlv)
        return;
#define CP(f)                           \
    g_SignatureSettings.has_##f = true; \
    g_SignatureSettings.f = tlv->f
    CP(reqsignedpost);
    CP(reqvalidcheckpost);
    CP(reqtimesyncpost);
    CP(reqseclocalpost);
    CP(reqsignedresp);
    CP(reqvalidcheckresp);
    CP(reqtimesyncresp);
    CP(reqseclocalresp);
#undef CP
    if (tlv->has_cert && tlv->cert.len <= MAX_SIGNATURE_CERT_LENGTH)
    {
        g_SignatureSettings.has_cert = true;
        g_SignatureSettings.cert.len = tlv->cert.len;
        memcpy(g_SignatureSettings.cert.data, tlv->cert.data, tlv->cert.len);
    }
}

static void rebootreq_timer_fired(void)
{
    osal_trickle_timer_stop(async_timer);
    osal_system_reboot(&g_devconfig.NMSaddr);
    g_reboot_request = false;
}

void rebootRequest_post(tlvid_t tlvid, Reboot_Request *tlv)
{
    (void)tlvid;
    if (!tlv)
    {
        return;
    }
    if (tlv->flag == REBOOT)
    {
        osal_trickle_timer_start(async_timer, REBOOT_DELAY, REBOOT_DELAY, (trickle_timer_fired_t)rebootreq_timer_fired);
        g_reboot_request = true;
    }
}


void *vendorTlv_get(tlvid_t tlvid, uint32_t *num)
{
    if (tlvid.vendor != VENDOR_ID)
    {
        *num = 0;
        return NULL;
    }
    *num = VENDOR_MAX_SUBTYPES;
    /* Subtype 1: MAC diagnostics — stub (requires api_mac.c, not yet linked) */
    g_vendorTlv[0].has_subtype = true;
    g_vendorTlv[0].subtype = 1u;
    /* Subtype 2: FHSS config — stub (requires fh_pib.h, not yet linked) */
    g_vendorTlv[1].has_subtype = true;
    g_vendorTlv[1].subtype = 2u;
    /* Subtype 3: RPL health (16 bytes) */
    {
        nwk_stats_t s = {0};
        nanostack_lock();
        protocol_stats_start(&s);
        nanostack_unlock();
        uint32_t v[4] = {s.rpl_local_repair, s.rpl_global_repair,
                         s.rpl_total_memory, s.buf_headroom_fail};
        g_vendorTlv[2].has_subtype = true;
        g_vendorTlv[2].subtype = 3u;
        g_vendorTlv[2].has_value = true;
        g_vendorTlv[2].value.len = sizeof(v);
        memcpy(g_vendorTlv[2].value.data, v, sizeof(v));
    }
    /* Subtype 4: join time in seconds (4 bytes) */
    {
        uint32_t j = g_init_time; /* seconds since boot at join */
        g_vendorTlv[3].has_subtype = true;
        g_vendorTlv[3].subtype = 4u;
        g_vendorTlv[3].has_value = true;
        g_vendorTlv[3].value.len = 4u;
        memcpy(g_vendorTlv[3].value.data, &j, 4u);
    }
    return g_vendorTlv;
}

void vendorTlv_post(tlvid_t tlvid, Vendor_Tlv *tlv)
{
    if (tlvid.vendor != VENDOR_ID || !tlv)
        return;
    for (int i = 0; i < VENDOR_MAX_SUBTYPES; i++)
    {
        if (tlv->subtype == g_vendorTlv[i].subtype)
        {
            g_vendorTlv[i].value.len = tlv->value.len;
            memcpy(g_vendorTlv[i].value.data, tlv->value.data, tlv->value.len);
            return;
        }
    }
    g_vendorTlv[0].subtype = tlv->subtype;
    g_vendorTlv[0].value.len = tlv->value.len;
    memcpy(g_vendorTlv[0].value.data, tlv->value.data, tlv->value.len);
}

/*
 * ti_ota_activate — MCUBoot upgrade activation.
 *
 * Writes the 16-byte MCUBoot "upgrade pending" magic into the upload
 * slot trailer, then performs a normal software reset. MCUBoot runs at
 * next boot, finds the magic, copies the upload slot → run slot, and
 * boots the new firmware.
 *
 * Unlike the old ti_ota_copy_and_reboot():
 *   - Does NOT run from SRAM (.ramcode removed)
 *   - Does NOT call IntMasterDisable()
 *   - Does NOT directly erase or program the run slot
 *   - MCUBoot handles the flash copy safely before the application starts
 *
 * Requires MCUBoot binary flashed at 0x0 and -DSBL_ENABLE in build flags.
 * Without MCUBoot present the magic bytes are written to flash but nothing
 * acts on them — the device reboots normally into the unchanged firmware.
 */
static void ti_ota_activate(void)
{
    /* TI MCUBoot uses VERSION COMPARISON — no magic bytes, no pending flag.
     * On every reset MCUBoot reads ih_ver from primary (internal flash) and
     * secondary (external flash) slots. If secondary.ih_ver > primary.ih_ver
     * AND the ECDSA-P256 signature is valid, MCUBoot copies ext flash → internal
     * flash and boots the new image. Simply rebooting is all that is needed. */
    DPRINTF("OTA: rebooting — MCUBoot will install ext flash image if version is higher\n");
    osal_sleep_ms(100);   /* let trace output flush through ITM */
    SysCtrlSystemReset();

    for (;;) {} /* unreachable */
}

/* Called by lrq_timer when loadtime arrives */
static void loadreq_timer_fired(void)
{
    DPRINTF("OTA: loadreq_timer_fired — activating slot %lu\n",
            (unsigned long)g_curloadslot);

    osal_trickle_timer_stop(lrq_timer);

    /* Persist run slot header so the new version is visible after reboot */
    memcpy(&g_slothdr[RUN_IMAGE], &g_slothdr[g_curloadslot], sizeof(Csmp_Slothdr));
    osal_write_slothdr(RUN_IMAGE, &g_slothdr[RUN_IMAGE]);

    /* Signal MCUBoot and reboot — MCUBoot copies upload → run at next boot */
    ti_ota_activate();
}