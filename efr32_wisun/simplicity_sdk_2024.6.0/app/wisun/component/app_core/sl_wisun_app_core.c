/***************************************************************************//**
 * @file
 * @brief Wi-SUN Application Core Component
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "sl_status.h"
#include "sl_wisun_app_core.h"
#include "sl_wisun_app_core_config.h"
#include "cmsis_os2.h"
#include "sl_cmsis_os2_common.h"
#include "sl_status.h"
#include "sl_wisun_types.h"
#include "sl_wisun_api.h"
#include "sl_wisun_config.h"
#include "sl_wisun_event_mgr.h"
#include "sl_wisun_trace_util.h"
#include "sl_sleeptimer.h"

#if defined(SL_CATALOG_WISUN_APP_SETTING_PRESENT)
  #include "sl_wisun_app_setting.h"
#endif

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
  #warning Power Manager component is presented. Features/peripherals are constrained.
#endif

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

/// MDR capability
#define APP_WISUN_MDR_COMMAND_CAPABILITY                  0U

/// Eventflag error mask
#define APP_WISUN_EVTFLAG_ERROR_MSK                       (0x00000001UL << 31UL)

///  Release mutex and return
#define _return_and_mtx_release() \
  do {                            \
    _app_wisun_mutex_release();   \
    return;                       \
  } while (0)

/// Release mutex and return value
#define _return_val_and_mtx_release(retval) \
  do {                                      \
    _app_wisun_mutex_release();             \
    return (retval);                        \
  } while (0)

/// Synthetize app settings if it's not available
#if !defined(SL_CATALOG_WISUN_APP_SETTING_PRESENT)
typedef struct app_setting_wisun{
  char network_name[SL_WISUN_NETWORK_NAME_SIZE + 1];
  uint8_t network_size;
  int8_t tx_power;
  uint8_t device_type;
  uint8_t lfn_profile;
  bool is_default_phy;
  sl_wisun_phy_config_t phy;
} app_setting_wisun_t;
#endif

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------
/**************************************************************************//**
 * @brief Application setting
 * @details It setup Wi-SUN with application related configuration.
 * @param[in] setting setting structure that contains basic configuration
 * @return sl_status_t SL_STATUS_OK if it is successful.
 *****************************************************************************/
static sl_status_t _app_wisun_application_setting(const app_setting_wisun_t * const setting);

/**************************************************************************//**
 * @brief Security setting
 * @details It setup Wi-SUN with security related configuration.
 * @return sl_status_t SL_STATUS_OK if it is successful.
 *****************************************************************************/
sl_status_t _app_wisun_security_setting(void);

#if (SL_WISUN_APP_CORE_REGULATION != SL_WISUN_APP_CORE_REGULATION_NONE)
/**************************************************************************//**
 * @brief Regulation setting
 * @details It setup Wi-SUN with regulation related parameters.
 * @return sl_status_t SL_STATUS_OK if it is successful.
 *****************************************************************************/
__STATIC_INLINE sl_status_t _app_wisun_regulation_setting(void);
#endif

/**************************************************************************//**
 * @brief Acquire application mutex
 * @details Internal mutex lock
 *****************************************************************************/
__STATIC_INLINE void _app_wisun_mutex_acquire(void);

/**************************************************************************//**
 * @brief Release application mutex
 * @details Internal mutex release
 *****************************************************************************/
__STATIC_INLINE void _app_wisun_mutex_release(void);

/**************************************************************************//**
 * @brief Storing address to the current address
 * @details helper function
 * @param[in] addr_name address name
 * @param[in] addr_type address type
 * @param[in] addr address
 *****************************************************************************/
static void _store_address(const char *addr_name,
                           const sl_wisun_ip_address_type_t addr_type,
                           in6_addr_t *addr);

/**************************************************************************//**
 * @brief Setting state flag
 * @details It sets the state by a flag
 * @param[in] flag is a flag bit
 *****************************************************************************/
__STATIC_INLINE void _app_wisun_core_set_state(const sl_wisun_app_core_state_t flag);

/**************************************************************************//**
 * @brief Clear state flag
 * @details It clears the state by a flag
 * @param[in] flag is a flag bit
 *****************************************************************************/
__STATIC_INLINE void _app_wisun_core_clear_state(const sl_wisun_app_core_state_t flag);

/**************************************************************************//**
 * @brief Storing the current address
 * @details Helper function
 *****************************************************************************/
static void _store_current_addresses(void);

/**************************************************************************//**
 * @brief Get Length of certifacete string
 * @details special port of strnlen with different types
 * @param[in] cert Certificate ptr
 * @param[in] max_cert_len certificate buffer max length
 * @return uint16_t length of certificate string
 *****************************************************************************/
static uint16_t _get_cert_str_len(const uint8_t *cert, const uint16_t max_cert_len);

// -----------------------------------------------------------------------------
//                                Static Variables
// -----------------------------------------------------------------------------

/// Create default setting if app settings is not available
#if !defined(SL_CATALOG_WISUN_APP_SETTING_PRESENT)
static const app_setting_wisun_t _app_default_settings = {
#if defined(WISUN_CONFIG_NETWORK_NAME)
  .network_name = WISUN_CONFIG_NETWORK_NAME,
#else
  .network_name = "Wi-SUN Network",
#endif
#if defined(WISUN_CONFIG_NETWORK_SIZE)
  .network_size = WISUN_CONFIG_NETWORK_SIZE,
#else
  .network_size = SL_WISUN_NETWORK_SIZE_SMALL,
#endif
#if defined(WISUN_CONFIG_TX_POWER)
  .tx_power = WISUN_CONFIG_TX_POWER,
#else
  .tx_power = 20U,
#endif
  .is_default_phy = true,
#if defined(WISUN_CONFIG_DEVICE_TYPE)
  .device_type = WISUN_CONFIG_DEVICE_TYPE,
#else
  .device_type = SL_WISUN_ROUTER,
#endif
#if defined(WISUN_CONFIG_DEVICE_PROFILE)
  .lfn_profile = WISUN_CONFIG_DEVICE_PROFILE,
#else
  .lfn_profile = SL_WISUN_LFN_PROFILE_TEST,
#endif
#if defined(WISUN_CONFIG_DEFAULT_PHY_FAN10)
  .phy = {
    .type = SL_WISUN_PHY_CONFIG_FAN10,
    .config.fan10.reg_domain = WISUN_CONFIG_REGULATORY_DOMAIN,
    .config.fan10.op_class = WISUN_CONFIG_OPERATING_CLASS,
    .config.fan10.op_mode = WISUN_CONFIG_OPERATING_MODE,
  },
#elif defined(WISUN_CONFIG_DEFAULT_PHY_FAN11)
  .phy = {
    .type = SL_WISUN_PHY_CONFIG_FAN11,
    .config.fan11.reg_domain = WISUN_CONFIG_REGULATORY_DOMAIN,
    .config.fan11.chan_plan_id = WISUN_CONFIG_CHANNEL_PLAN_ID,
    .config.fan11.phy_mode_id = WISUN_CONFIG_PHY_MODE_ID,
  },
#else
  .phy = { 0 },
#endif
};
#endif

/// Here we track if regional regulation is active or not
static bool _regional_regulation_active = false;

static sl_wisun_app_core_reg_thresholds_t _tresholds = {
  .warning_threshold = SL_WISUN_APP_CORE_DEFAULT_REGULATION_WARNING_THRESHOLD,
  .alert_threshold = SL_WISUN_APP_CORE_DEFAULT_REGULATION_ALERT_THRESHOLD,
};

/// App framework mutex
static osMutexId_t _app_core_mtx = NULL;

///  App framework mutex attribute
static const osMutexAttr_t _app_wisun_mtx_attr = {
  .name      = "AppWisunkMutex",
  .attr_bits = osMutexRecursive,
  .cb_mem    = NULL,
  .cb_size   = 0
};

/// Current address storage
static sl_wisun_app_core_current_addr_t _current_addr = { 0U };

/// error flag for errors
static osEventFlagsId_t _app_core_state = NULL;

/// Sate event flags attributes
static const osEventFlagsAttr_t _app_wisun_evt_attr = {
  .name      = "AppWisunEvtFlags",
  .attr_bits = 0,
  .cb_mem    = NULL,
  .cb_size   = 0
};

/// Internal join state
static sl_wisun_join_state_t _join_state = SL_WISUN_JOIN_STATE_DISCONNECTED;

/// Internal setting storage
static app_setting_wisun_t _setting = { 0U };

/// Time statistic storage
static sl_wisun_app_core_time_stat_t _time_stat = { 0U };

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

/* Network update event handler */
void sl_wisun_network_update_event_hnd(sl_wisun_evt_t *evt)
{
  const char *ip_str = NULL;
  const char *time_str = NULL;
  static sl_wisun_trace_util_time_t time = { 0U };
  uint64_t time_ms = 0ULL;

  sl_sleeptimer_tick64_to_ms(sl_sleeptimer_get_tick_count64(), &time_ms);
  time_ms = time_ms - _time_stat.connected_ms;
  app_wisun_trace_util_timestamp_init(time_ms, &time);
  time_str = app_wisun_trace_util_time_to_str(&time);
  if (time_str == NULL) {
    time_str = "";
  }

  if (evt->evt.network_update.status == SL_STATUS_OK) {
    if (evt->evt.network_update.flags & (1 << SL_WISUN_NETWORK_UPDATE_FLAGS_GLOBAL_IP)) {
      _store_address("GLOBAL", SL_WISUN_IP_ADDRESS_TYPE_GLOBAL, &_current_addr.global);
      ip_str = app_wisun_trace_util_get_ip_str(&_current_addr.global);
      printf("[%s Network update: Global %s]\n", time_str, ip_str);
    }
    if (evt->evt.network_update.flags & (1 << SL_WISUN_NETWORK_UPDATE_FLAGS_PRIMARY_PARENT)) {
      _store_address("PRIM_PARENT", SL_WISUN_IP_ADDRESS_TYPE_PRIMARY_PARENT, &_current_addr.primary_parent);
      ip_str = app_wisun_trace_util_get_ip_str(&_current_addr.primary_parent);
      printf("[%s Network update: Primary Parent %s]\n", time_str, ip_str);
    }
    if (evt->evt.network_update.flags & (1 << SL_WISUN_NETWORK_UPDATE_FLAGS_SECONDARY_PARENT)) {
      _store_address("SEC_PARENT", SL_WISUN_IP_ADDRESS_TYPE_SECONDARY_PARENT, &_current_addr.secondary_parent);
      ip_str = app_wisun_trace_util_get_ip_str(&_current_addr.secondary_parent);
      printf("[%s Network update: Secondary Parent %s]\n", time_str, ip_str);
    }
  }

  app_wisun_trace_util_destroy_ip_str(ip_str);
  app_wisun_trace_util_destroy_time_str(time_str);
  __CHECK_FOR_STATUS(evt->evt.error.status);
}

/* Connected event handler */
void sl_wisun_connected_event_hnd(sl_wisun_evt_t *evt)
{
  uint64_t time_ms = 0;

  if (evt->evt.connected.status != SL_STATUS_OK) {
    printf("[Connection failed. Status: %lu]\n", evt->evt.connected.status);
    _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_NETWORK_DISCONNECTED);
    _app_wisun_core_clear_state(SL_WISUN_APP_CORE_STATE_NETWORK_CONNECTED);
    return;
  }
  // store the current addresses
  _store_current_addresses();

  // update internal time stat
  sl_sleeptimer_tick64_to_ms(sl_sleeptimer_get_tick_count64(), &time_ms);
  _time_stat.tot_disconnected_ms += (time_ms  - _time_stat.connected_ms);
  _time_stat.curr_ms = time_ms;
  _time_stat.connected_ms = time_ms;
  ++_time_stat.conn_cnt;

  time_ms = time_ms - _time_stat.disconnected_ms;

  printf("[%lu s]\n", (uint32_t)time_ms / 1000U);
  _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_NETWORK_CONNECTED);
  _app_wisun_core_clear_state(SL_WISUN_APP_CORE_STATE_NETWORK_DISCONNECTED);
  __CHECK_FOR_STATUS(evt->evt.error.status);
}

/* Socket disconnected event handler */
void sl_wisun_disconnected_event_hnd(sl_wisun_evt_t *evt)
{
  uint64_t time_ms = 0;

  printf("[Disconnected]\n");

  // update internal time stat
  sl_sleeptimer_tick64_to_ms(sl_sleeptimer_get_tick_count64(), &time_ms);
  _time_stat.tot_connected_ms += (time_ms  - _time_stat.disconnected_ms);
  _time_stat.curr_ms = time_ms;
  _time_stat.disconnected_ms = time_ms;

  __CHECK_FOR_STATUS(evt->evt.error.status);
  _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_NETWORK_DISCONNECTED);
  _join_state = SL_WISUN_JOIN_STATE_DISCONNECTED;
}

/* Socket connection lost event handler*/
void sl_wisun_connection_lost_event_hnd(sl_wisun_evt_t *evt)
{
  sl_status_t stat;

  // connect to network (using the PHY settings)
  stat = sl_wisun_join((const uint8_t *)_setting.network_name, &_setting.phy);

  if (stat == SL_STATUS_OK) {
    printf("[Connection lost, connecting to \"%s\"]\n", _setting.network_name);
  }
  _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_NETWORK_CONNECTION_LOST);
  _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_NETWORK_DISCONNECTED);
  _app_wisun_core_clear_state(SL_WISUN_APP_CORE_STATE_NETWORK_CONNECTED);
  __CHECK_FOR_STATUS(evt->evt.error.status);
}

/* Error event handler */
void sl_wisun_error_event_hnd(sl_wisun_evt_t *evt)
{
  printf("[Wi-SUN network error occured. Status: %lu\n",
         evt->evt.error.status);
}

/* Join state event handler */
void sl_wisun_join_state_event_hnd(sl_wisun_evt_t *evt)
{
  _join_state = (sl_wisun_join_state_t)evt->evt.join_state.join_state;
#if HEARTBEAT_ENABLED
  (void) evt;
#else
  const char *join_state_str = NULL;
  // if heartbeat not enabled, print join state
  join_state_str = app_wisun_trace_util_conn_state_to_str(_join_state);
  if (join_state_str != NULL) {
    printf("[Join state: %s (%lu)]\n", join_state_str, evt->evt.join_state.join_state);
  }
#endif
  __CHECK_FOR_STATUS(evt->evt.error.status);
}

void sl_wisun_lfn_wake_up_hnd(sl_wisun_evt_t *evt)
{
  __CHECK_FOR_STATUS(evt->evt.error.status);
}

void sl_wisun_multicast_reg_finish_hnd(sl_wisun_evt_t *evt)
{
  __CHECK_FOR_STATUS(evt->evt.error.status);
}

/* Wisun app core init */
void sl_wisun_app_core_init(void)
{
  // init wisun network mutex
  _app_core_mtx = osMutexNew(&_app_wisun_mtx_attr);
  assert(_app_core_mtx != NULL);

  _app_core_state = osEventFlagsNew(&_app_wisun_evt_attr);
  assert(_app_core_state != NULL);
}

/* App core get error */
sl_status_t sl_wisun_app_core_get_state(uint32_t * const state)
{
  *state = osEventFlagsGet(_app_core_state);
  // Check error flag
  if (*state & APP_WISUN_EVTFLAG_ERROR_MSK) {
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

sl_status_t sl_wisun_app_core_wait_state(const uint32_t state, const uint32_t timeout)
{
  uint32_t ret = 0UL;
  ret = osEventFlagsWait(_app_core_state, state, osFlagsWaitAll | osFlagsNoClear, timeout);
  return (ret & APP_WISUN_EVTFLAG_ERROR_MSK) ? SL_STATUS_FAIL : SL_STATUS_OK;
}

/*Connecting to the wisun network*/
void sl_wisun_app_core_network_connect(void)
{
  sl_status_t ret = SL_STATUS_FAIL;
  sl_wisun_join_state_t join_state = SL_WISUN_JOIN_STATE_DISCONNECTED;
  uint64_t time_ms = 0ULL;

  _app_wisun_mutex_acquire(); // get mutex

#if defined(SL_CATALOG_WISUN_APP_SETTING_PRESENT)
  // Init app PHY config
  ret = app_wisun_setting_init_phy_cfg();

  // get full settings (PHY, network name, network size and TX power)
  ret = app_wisun_setting_get(&_setting);
  if (ret != SL_STATUS_OK) {
    printf("[Failed: unable to get settings\n");
    _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_SETTING_ERROR);
    _return_and_mtx_release();
  }
#else
  memcpy(&_setting, &_app_default_settings, sizeof(app_setting_wisun_t));
#endif

  // check join state before connection progress.
  ret = sl_wisun_get_join_state(&join_state);
  __CHECK_FOR_STATUS(ret);
  if (join_state != SL_WISUN_JOIN_STATE_DISCONNECTED) {
    printf("[Failed: already connecting or connected]\n");
    _return_and_mtx_release();
  }

  // application settings
  ret = _app_wisun_application_setting(&_setting);
  if (ret != SL_STATUS_OK) {
    _return_and_mtx_release();
  }

  // security settings
  ret = _app_wisun_security_setting();
  if (ret != SL_STATUS_OK) {
    _return_and_mtx_release();
  }

#if (SL_WISUN_APP_CORE_REGULATION != SL_WISUN_APP_CORE_REGULATION_NONE)
  ret = _app_wisun_regulation_setting();
  if (ret != SL_STATUS_OK) {
    _return_and_mtx_release();
  }
#endif

  ret = sl_wisun_join((const uint8_t *)_setting.network_name, &_setting.phy);

  if (ret == SL_STATUS_OK) {
    // update internal time stat
    sl_sleeptimer_tick64_to_ms(sl_sleeptimer_get_tick_count64(), &time_ms);
    _time_stat.curr_ms = time_ms;
    _time_stat.connected_ms = time_ms;
    _time_stat.disconnected_ms = time_ms;

    printf("\n[Connecting to \"%s\"]\n", _setting.network_name);
  } else {
    _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_CONNECTION_ERROR);
    printf("\n[Connection failed: %lu]\n", ret);
  }
  _app_wisun_mutex_release();
}

void sl_wisun_regulation_tx_level_hnd(sl_wisun_evt_t *evt)
{
  (void)*evt;
}

void sl_wisun_app_core_get_current_addresses(sl_wisun_app_core_current_addr_t * const dest_addresses)
{
  _app_wisun_mutex_acquire();
  memcpy(&dest_addresses->global, &_current_addr.global, sizeof(in6_addr_t));
  memcpy(&dest_addresses->border_router, &_current_addr.border_router, sizeof(in6_addr_t));
  memcpy(&dest_addresses->link_local, &_current_addr.link_local, sizeof(in6_addr_t));
  memcpy(&dest_addresses->primary_parent, &_current_addr.primary_parent, sizeof(in6_addr_t));
  memcpy(&dest_addresses->secondary_parent, &_current_addr.secondary_parent, sizeof(in6_addr_t));
  _app_wisun_mutex_release();
}

void sl_wisun_app_core_set_regulation_active(bool enabled)
{
  _app_wisun_mutex_acquire();
  _regional_regulation_active = enabled;
  _app_wisun_mutex_release();
}

bool sl_wisun_app_core_get_regulation_active(void)
{
  bool retval;
  _app_wisun_mutex_acquire();
  retval = _regional_regulation_active;
  _app_wisun_mutex_release();
  return retval;
}

void sl_wisun_app_core_set_regulation_thresholds(const int8_t warning_level, const  int8_t alert_level)
{
  _app_wisun_mutex_acquire();
  _tresholds.warning_threshold = warning_level;
  _tresholds.alert_threshold = alert_level;
  _app_wisun_mutex_release();
}

sl_status_t sl_wisun_app_core_get_regulation_thresholds(sl_wisun_app_core_reg_thresholds_t* thresholds_out)
{
  if (thresholds_out == NULL) {
    return SL_STATUS_FAIL;
  }

  _app_wisun_mutex_acquire();
  thresholds_out->warning_threshold = _tresholds.warning_threshold;
  thresholds_out->alert_threshold = _tresholds.alert_threshold;
  _app_wisun_mutex_release();
  return SL_STATUS_OK;
}

sl_status_t sl_wisun_app_core_get_remaining_tx_budget(uint32_t* const budget_out)
{
  sl_wisun_statistics_t stat;
  sl_status_t ret = SL_STATUS_FAIL;

  if (!_regional_regulation_active || budget_out == NULL) {
    return ret;
  }

  _app_wisun_mutex_acquire();

  if (sl_wisun_get_statistics(SL_WISUN_STATISTICS_TYPE_REGULATION, &stat) == SL_STATUS_OK) {
    // return a meaningful value (budget remaning) or zero (exceeded)
    *budget_out = (stat.regulation.arib.tx_duration_ms < SL_WISUN_APP_CORE_TX_BUDGET)
                  ? (SL_WISUN_APP_CORE_TX_BUDGET - stat.regulation.arib.tx_duration_ms) : 0UL;
    ret = SL_STATUS_OK;
  } else {
    ret = SL_STATUS_FAIL;
  }
  _app_wisun_mutex_release();

  return ret;
}

sl_wisun_join_state_t sl_wisun_app_core_get_join_state(void)
{
  sl_wisun_join_state_t join_state = SL_WISUN_JOIN_STATE_DISCONNECTED;
  _app_wisun_mutex_acquire();
  join_state = _join_state;
  _app_wisun_mutex_release();
  return join_state;
}

void sl_wisun_app_core_get_time_stat(sl_wisun_app_core_time_stat_t * const tstat)
{
  uint64_t time_ms = 0ULL;

  _app_wisun_mutex_acquire();

  // update current time
  sl_sleeptimer_tick64_to_ms(sl_sleeptimer_get_tick_count64(), &time_ms);

  memcpy(tstat, &_time_stat, sizeof(sl_wisun_app_core_time_stat_t));

  // add diff beetween current time and last update time to the actual state
  tstat->curr_ms = time_ms;
  time_ms = tstat->curr_ms - _time_stat.curr_ms;
  if (_join_state != SL_WISUN_JOIN_STATE_OPERATIONAL) {
    tstat->tot_disconnected_ms += time_ms;
  } else {
    tstat->tot_connected_ms += time_ms;
  }

  _app_wisun_mutex_release();
}
#if defined(SL_CATALOG_WISUN_LFN_DEVICE_SUPPORT_PRESENT)

sl_wisun_device_type_t sl_wisun_app_core_get_device_type(void)
{
  return (sl_wisun_device_type_t)_setting.device_type;
}

sl_wisun_lfn_profile_t sl_wisun_app_core_get_lfn_profile(void)
{
  return (sl_wisun_lfn_profile_t)_setting.lfn_profile;
}

const sl_wisun_lfn_params_t *sl_wisun_app_core_get_lfn_params(void)
{
  // Not lfn device
  if (_setting.device_type != SL_WISUN_LFN) {
    return NULL;
  }

  switch (_setting.lfn_profile) {
    case SL_WISUN_LFN_PROFILE_TEST:
      return &SL_WISUN_PARAMS_LFN_TEST;
    case SL_WISUN_LFN_PROFILE_BALANCED:
      return &SL_WISUN_PARAMS_LFN_BALANCED;
    case SL_WISUN_LFN_PROFILE_ECO:
      return &SL_WISUN_PARAMS_LFN_ECO;
    default:
      return NULL;
  }
}
#endif

// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------

static sl_status_t _app_wisun_application_setting(const app_setting_wisun_t * const setting)
{
  sl_status_t ret = SL_STATUS_FAIL;
  const sl_wisun_connection_params_t *conn_param = NULL;
#if defined(WISUN_CONFIG_BROADCAST_RETRIES)
  sl_wisun_connection_params_t update_param = { 0 };
#endif

  conn_param = sl_wisun_get_conn_param_by_nw_size((sl_wisun_network_size_t) setting->network_size);

  ret = sl_wisun_set_device_type((sl_wisun_device_type_t)setting->device_type);
  if (ret != SL_STATUS_OK) {
    printf("[Failed: unable to set device type: %lu]\n", ret);
    return ret;
  }
#if defined(SL_CATALOG_WISUN_LFN_DEVICE_SUPPORT_PRESENT)
  if (setting->device_type == SL_WISUN_LFN) {
    // Store LFN profile based on wisun config
    ret = sl_wisun_set_lfn_parameters(sl_wisun_app_core_get_lfn_params());
    if (ret != SL_STATUS_OK) {
      printf("[Failed: unable to set device type: %lu]\n", ret);
      return ret;
    }
  }
#endif

#if defined(WISUN_CONFIG_BROADCAST_RETRIES)
  memcpy(&update_param, conn_param, sizeof(sl_wisun_connection_params_t));
  update_param.mpl.trickle_expirations = WISUN_CONFIG_BROADCAST_RETRIES;
  conn_param = &update_param;
#endif

#if defined(SL_CATALOG_WISUN_FFN_DEVICE_SUPPORT_PRESENT)
  // sets the network name
  ret = sl_wisun_set_connection_parameters(conn_param);
  if (ret != SL_STATUS_OK) {
    printf("[Failed: unable to set network size: %lu]\n", ret);
    _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_SET_NETWORK_SIZE_ERROR);
    return ret;
  }
#endif

  // sets the TX power
  ret = sl_wisun_set_tx_power(setting->tx_power);
  if (ret != SL_STATUS_OK) {
    printf("[Failed: unable to set TX power: %lu]\n", ret);
    _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_SET_TX_POWER_ERROR);
    return ret;
  }
#if defined(WISUN_CONFIG_ALLOWED_CHANNELS)
  ret = sl_wisun_set_channel_mask(&wisun_config_allowed_channels_mask);
  if (ret != SL_STATUS_OK) {
    printf("[Failed: unable to set allowed channels: %lu]\n", ret);
    return ret;
  }
#endif
#if defined(WISUN_CONFIG_MODE_SWITCH_PHYS_NUMBER)
  ret = sl_wisun_set_pom_ie(WISUN_CONFIG_MODE_SWITCH_PHYS_NUMBER,
                            (uint8_t *)wisun_config_ms_phys,
                            APP_WISUN_MDR_COMMAND_CAPABILITY);
  if (ret != SL_STATUS_OK) {
    printf("[Failed: unable to set mode switch phys: %lu]\n", ret);
    return ret;
  }
#endif

#if defined(WISUN_CONFIG_DWELL_INTERVAL) && defined(SL_CATALOG_WISUN_FFN_DEVICE_SUPPORT_PRESENT)
  // sets unicast
  ret = sl_wisun_set_unicast_settings(WISUN_CONFIG_DWELL_INTERVAL);

  if (ret != SL_STATUS_OK) {
    printf("[Failed: unable to set dwell interval: %lu]\n", ret);
    _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_SET_DWELL_INTERVAL_ERROR);
    return ret;
  }
#endif

#if defined(WISUN_CONFIG_MAC_ADDRESS_PRESENT)
  // set mac address
  ret = sl_wisun_set_mac_address(&wisun_config_mac_address);
  if (ret != SL_STATUS_OK) {
    printf("[Failed: unable to set MAC address: %lu]\n", ret);
    _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_SET_MAC_ADDR_ERROR);
    return ret;
  }
#endif

#if defined(WISUN_CONFIG_MAC_ALLOW_LIST_PRESENT)
  // set allow mac addresses
  for (uint8_t index = 0U; index < wisun_config_mac_allow_list.mac_list_num; index++) {
    ret = sl_wisun_allow_mac_address(&wisun_config_mac_allow_list.mac_list[index]);
    if (ret != SL_STATUS_OK) {
      printf("[Failed: unable to set allow address: %lu]\n", ret);
      _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_SET_ALLOW_MAC_ADDR_ERROR);
      return ret;
    }
  }
#endif
#if defined(WISUN_CONFIG_MAC_DENY_LIST_PRESENT)
  // set deny mac addresses
  for (uint8_t index = 0U; index < wisun_config_mac_deny_list.mac_list_num; index++) {
    ret = sl_wisun_deny_mac_address(&wisun_config_mac_deny_list.mac_list[index]);
    if (ret != SL_STATUS_OK) {
      printf("[Failed: unable to set allow address: %lu]\n", ret);
      _app_wisun_core_set_state(SL_WISUN_APP_CORE_SET_DENY_MAC_ADDR_ERROR);
      return ret;
    }
  }
#endif

  return ret;
}

sl_status_t _app_wisun_security_setting(void)
{
  sl_status_t ret = SL_STATUS_FAIL;
  const uint32_t max_cert_str_len = 2048U;

  // sets the trusted certificate
  ret = sl_wisun_set_trusted_certificate(SL_WISUN_CERTIFICATE_OPTION_IS_REF,
                                         _get_cert_str_len(wisun_config_ca_certificate, max_cert_str_len) + 1,
                                         wisun_config_ca_certificate);
  if (ret != SL_STATUS_OK) {
    printf("[Failed: unable to set the trusted certificate: %lu]\n", ret);
    _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_SET_TRUSTED_CERTIFICATE_ERROR);
    return ret;
  }

  // sets the device certificate
  ret = sl_wisun_set_device_certificate(SL_WISUN_CERTIFICATE_OPTION_IS_REF | SL_WISUN_CERTIFICATE_OPTION_HAS_KEY,
                                        _get_cert_str_len(wisun_config_device_certificate, max_cert_str_len) + 1,
                                        wisun_config_device_certificate);
  if (ret != SL_STATUS_OK) {
    printf("[Failed: unable to set the device certificate: %lu]\n", ret);
    _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_SET_DEVICE_CERTIFICATE_ERROR);
    return ret;
  }

  // sets the device private key
  // NOTE: to use a wrapped PSA private key, the app needs to import the key
  // and use the API sl_wisun_set_device_private_key_id() instead of the one below
  ret = sl_wisun_set_device_private_key(SL_WISUN_PRIVATE_KEY_OPTION_IS_REF,
                                        _get_cert_str_len(wisun_config_device_private_key, max_cert_str_len) + 1,
                                        wisun_config_device_private_key);
  if (ret != SL_STATUS_OK) {
    printf("[Failed: unable to set the device private key: %lu]\n", ret);
    _app_wisun_core_set_state(SL_WISUN_APP_CORE_STATE_SET_DEVICE_PRIVATE_KEY_ERROR);
    return ret;
  }

  return ret;
}

#if (SL_WISUN_APP_CORE_REGULATION != SL_WISUN_APP_CORE_REGULATION_NONE)
__STATIC_INLINE sl_status_t _app_wisun_regulation_setting(void)
{
  sl_status_t ret = SL_STATUS_FAIL;

  // regulation thresholds
  sl_wisun_app_core_reg_thresholds_t thresholds;
  // name of the regulation type to print upon connection
  char* regulation_name;

  (void)sl_wisun_app_core_get_regulation_thresholds(&thresholds);
  ret = sl_wisun_set_regulation_tx_thresholds(thresholds.warning_threshold,
                                              thresholds.alert_threshold);
  if (ret == SL_STATUS_OK) {
    ret = sl_wisun_set_regulation(SL_WISUN_APP_CORE_REGULATION);
    if (ret != SL_STATUS_OK) {
      printf("[Failed: unable to set regulation: %lu]\n", ret);
    } else {
      switch (SL_WISUN_APP_CORE_REGULATION) {
        case SL_WISUN_REGULATION_ARIB:
          regulation_name = "ARIB";
          break;
        case SL_WISUN_REGULATION_WPC:
          regulation_name = "WPC";
          break;
        default:
          regulation_name = "UNKNOWN";
          break;
      }
      printf("\n[Regional regulation set to %s]\n", regulation_name);
      _regional_regulation_active = true;
    }
  } else {
    printf("\n[Failed: unable to set regulation TX thresholds]\n");
  }

  return ret;
}
#endif

/* Mutex acquire */
__STATIC_INLINE void _app_wisun_mutex_acquire(void)
{
  assert(osMutexAcquire(_app_core_mtx, osWaitForever) == osOK);
}

/* Mutex release */
__STATIC_INLINE void _app_wisun_mutex_release(void)
{
  assert(osMutexRelease(_app_core_mtx) == osOK);
}

/* Storing address */
static void _store_address(const char *addr_name,
                           const sl_wisun_ip_address_type_t addr_type,
                           in6_addr_t *addr)
{
  sl_status_t stat = SL_STATUS_FAIL;
  const char *ip_str = NULL;

#if defined(SL_CATALOG_WISUN_LFN_DEVICE_SUPPORT_PRESENT)
  // skip BR address failure for LFN node
  if (addr_type == SL_WISUN_IP_ADDRESS_TYPE_BORDER_ROUTER
      && _setting.device_type == SL_WISUN_LFN) {
    return;
  }
#endif
  stat = sl_wisun_get_ip_address(addr_type, addr);

  ip_str = app_wisun_trace_util_get_ip_str(addr);
  if (addr_type != SL_WISUN_IP_ADDRESS_TYPE_PRIMARY_PARENT
      && addr_type != SL_WISUN_IP_ADDRESS_TYPE_SECONDARY_PARENT) {
    if (stat != SL_STATUS_OK) {
      printf("[Getting '%s' address failed!]\n", addr_name);
    } else {
      printf("[%-14s: %s]\n", addr_name, ip_str);
    }
  }
  app_wisun_trace_util_destroy_ip_str(ip_str);
}

/* Setting error */
__STATIC_INLINE void _app_wisun_core_set_state(const sl_wisun_app_core_state_t flag)
{
  (void) osEventFlagsSet(_app_core_state, 1UL << flag);
}

__STATIC_INLINE void _app_wisun_core_clear_state(const sl_wisun_app_core_state_t flag)
{
  (void) osEventFlagsClear(_app_core_state, 1UL << flag);
}

/* Storing current addresses */
static void _store_current_addresses(void)
{
  memset(&_current_addr, 0, sizeof(sl_wisun_app_core_current_addr_t));
  printf("\nAddresses:\n");
  _store_address("GLOBAL", SL_WISUN_IP_ADDRESS_TYPE_GLOBAL, &_current_addr.global);
  _store_address("LINK_LOCAL", SL_WISUN_IP_ADDRESS_TYPE_LINK_LOCAL, &_current_addr.link_local);
  _store_address("BORDER_ROUTER", SL_WISUN_IP_ADDRESS_TYPE_BORDER_ROUTER, &_current_addr.border_router);
  _store_address("PRIMARY_PARENT", SL_WISUN_IP_ADDRESS_TYPE_PRIMARY_PARENT, &_current_addr.primary_parent);
  _store_address("SECONDARY_PARENT", SL_WISUN_IP_ADDRESS_TYPE_SECONDARY_PARENT, &_current_addr.secondary_parent);
}

/* Get certificate length */
static uint16_t _get_cert_str_len(const uint8_t *cert, const uint16_t max_cert_len)
{
  uint16_t n = 0;
  if (cert == NULL) {
    return 0U;
  }
  for (n = 0; n < max_cert_len && *cert; n++, cert++) {
    ;
  }
  return n;
}
