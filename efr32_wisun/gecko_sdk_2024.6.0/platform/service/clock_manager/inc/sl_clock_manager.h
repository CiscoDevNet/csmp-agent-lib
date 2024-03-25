/***************************************************************************//**
 * @file
 * @brief Clock Manager APIs.
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_CLOCK_MANAGER_H
#define SL_CLOCK_MANAGER_H

#include <stdbool.h>
#include <stdlib.h>
#include "sl_status.h"
#include "sl_enum.h"
#include "sl_technology_abstraction_clock.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup clock_manager Clock Manager
 * @brief Clock Manager
 * @details
 * ## Overview
 *
 * TODO
 *
 * @{
 ******************************************************************************/

/// Peripheral Clock
SL_ENUM(sl_clock_manager_peripheral_clock_t) {
  SL_CLOCK_MANAGER_PERIPHERAL_NONE
};

// Export clock source
SL_ENUM(sl_clock_manager_export_clock_source_t) {
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_DISABLED,
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_FSRCO,
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HFXO,
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HFRCODPLL,
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HFRCOEM23,
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HFEXPCLK,
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_LFXO,
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_PLFRCO,
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_LFRCO,
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_ULFRCO,
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HCLK,
};

// Export clock output select
SL_ENUM(sl_clock_manager_export_clock_output_select_t) {
  SL_CLOCK_MANAGER_EXPORT_CLOCK_OUTPUT_SELECT_0 = 0,
  SL_CLOCK_MANAGER_EXPORT_CLOCK_OUTPUT_SELECT_1,
  SL_CLOCK_MANAGER_EXPORT_CLOCK_OUTPUT_SELECT_2,
};

// Clocks available for Calibration
SL_ENUM(sl_clock_manager_clock_calibration_t) {
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_HCLK,
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_PRS,
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_HFXO,
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_LFXO,
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_HFRCODPLL,
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_HFRCOEM23,
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_FSRCO,
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_LFRCO,
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_ULFRCO
};

// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************//**
 * Performs Clock Manager runtime initialization.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_runtime_init(void);

/***************************************************************************//**
 * Gets frequency of given oscillator.
 *
 * @param[in] oscillator  Oscillator
 *
 * @param[out] frequency  Oscillator's frequency in Hertz
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_oscillator_frequency(sl_oscillator_t oscillator,
                                                      uint32_t        *frequency);

/***************************************************************************//**
 * Gets precision of given oscillator.
 *
 * @param[in] oscillator  Oscillator
 *
 * @param[out] precision  Oscillator's precision in PPM
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_oscillator_precision(sl_oscillator_t oscillator,
                                                      uint16_t        *precision);

/***************************************************************************//**
 * Gets frequency of given clock branch.
 *
 * @param[in] clock_branch  Clock Branch
 *
 * @param[out] frequency    Clock Branch's frequency in Hertz
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_clock_branch_frequency(sl_clock_branch_t clock_branch,
                                                        uint32_t          *frequency);

/***************************************************************************//**
 * Gets precision of given clock branch.
 *
 * @param[in] clock_branch  Clock Branch
 *
 * @param[out] precision    Clock Branch's precision in PPM
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_clock_branch_precision(sl_clock_branch_t clock_branch,
                                                        uint16_t          *precision);

/***************************************************************************//**
 * Enables the bus clock associated with the given module.
 *
 * @param[in] module  module on which to enable its bus clock
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_enable_bus_clock(sl_bus_clock_t module);

/***************************************************************************//**
 * Disables the bus clock associated with the given module.
 *
 * @param[in] module  module on which to disable its bus clock
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_disable_bus_clock(sl_bus_clock_t module);

/***************************************************************************//**
 * Enables the peripheral clock associated with the given peripheral.
 *
 * @param[in] peripheral  peripheral on which to enable its clock
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_enable_peripheral_clock(sl_clock_manager_peripheral_clock_t peripheral);

/***************************************************************************//**
 * Disables the peripheral clock associated with the given peripheral.
 *
 * @param[in] peripheral  peripheral on which to disable its clock
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_disable_peripheral_clock(sl_clock_manager_peripheral_clock_t peripheral);

/***************************************************************************//**
 * Configure one clock export output with specified clock source.
 *
 * @param[in] export_clock_source One of the exportable clock source.
 *
 * @param[in] output_select       Selected export clock output channel.
 *
 * @param[in] hfexp_divider       HFEXP clock divider (1 to 32).
 *                                Note: This parameter only affects the EXPCLK branch frequency.
 *
 * @param[in] port                GPIO port to output exported clock.
 *
 * @param[in] pin                 GPIO pin number to output exported clock.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_set_gpio_clock_output(sl_clock_manager_export_clock_source_t export_clock_source,
                                                   sl_clock_manager_export_clock_output_select_t output_select,
                                                   uint16_t hfexp_divider,
                                                   uint32_t port,
                                                   uint32_t pin);

/***************************************************************************//**
 * @brief
 *   Set the RC oscillator frequency tuning control.
 *
 * @note
 *   RC Oscillator tuning is done during production, and the tuning value is
 *   automatically loaded after a reset. Changing the tuning value from the
 *   calibrated value is for more advanced use. Certain oscillators also have
 *   build-in tuning optimization.
 *
 * @note
 *   Supported RC oscillators include:
 *      SL_OSCILLATOR_HFRCODPLL,
 *      SL_OSCILLATOR_HFRCOEM23,
 *      SL_OSCILLATOR_LFRCO
 *
 * @param[in] osc
 *   RC Oscillator to set tuning value for.
 *
 * @param[in] val
 *   The RC oscillator frequency tuning setting to use.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_set_rc_oscillator_calibration(sl_oscillator_t oscillator,
                                                           uint32_t val);

/***************************************************************************//**
 * @brief
 *   Get the RC oscillator frequency tuning setting.
 *
 * @note
 *   Supported RC oscillators include:
 *      SL_OSCILLATOR_HFRCODPLL,
 *      SL_OSCILLATOR_HFRCOEM23,
 *      SL_OSCILLATOR_LFRCO
 *
 * @param[in] osc
 *   An RC oscillator to get tuning value for
 *
 * @param[out] val
 *   The RC oscillator frequency tuning setting in use.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_rc_oscillator_calibration(sl_oscillator_t oscillator,
                                                           uint32_t *val);

/***************************************************************************//**
 * @brief
 *   Set the HFXO frequency tuning control.
 *
 * @param[in] val
 *   The HFXO frequency tuning setting to use.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_set_hfxo_calibration(uint32_t val);

/***************************************************************************//**
 * @brief
 *   Get the HFXO frequency tuning setting.
 *
 * @param[out] val
 *   The HFXO frequency tuning setting to use.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_hfxo_calibration(uint32_t *val);

/***************************************************************************//**
 * @brief
 *   Set the HFXO's CTUNE.
 *
 * @param[in] ctune
 *   The HFXO's CTUNE value.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  Sets the XI value to the given ctune value and sets the XO value based
 *        on that same value , but with an offset that is hardware dependant.
 *        Updating CTune while the crystal oscillator is running can
 *        result in significant clock glitches for one XO clock period.
 *        Should be used with caution.
 ******************************************************************************/
sl_status_t slx_clock_manager_hfxo_set_ctune(uint32_t ctune);

/***************************************************************************//**
 * @brief
 *   Get the HFXO's CTUNE.
 *
 * @param[out] ctune
 *   The returned HFXO's CTUNE value.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  This function only returns the CTUNE XI value.
 *        The XO value follows the XI value with a fixed delta that is
 *        hardware dependant.
 ******************************************************************************/
sl_status_t slx_clock_manager_hfxo_get_ctune(uint32_t *ctune);

/***************************************************************************//**
 * @brief
 *   Update the tuning capacitances and calibrate the Core Bias Current.
 *
 * @param[in] ctune
 *   The HFXO's CTUNE value.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  Calibrating the CTUNE is time consuming and will cause glitches on the
 *        HFXO's clock. Care and caution should be taken when using this API.
 ******************************************************************************/
sl_status_t slx_clock_manager_hfxo_calibrate_ctune(uint32_t ctune);

/***************************************************************************//**
 * @brief
 *   Set the LFXO frequency tuning control.
 *
 * @param[in] val
 *   The LFXO frequency tuning setting to use.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_set_lfxo_calibration(uint32_t val);

/***************************************************************************//**
 * @brief
 *   Get the LFXO frequency tuning setting.
 *
 * @param[out] val
 *   The LFXO frequency tuning setting to use.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_lfxo_calibration(uint32_t *val);

/***************************************************************************//**
 * Configures the RCO calibration.
 *
 * @param[in] cycles  Number of cycles to run calibration. Increasing this
 *                    number increases precision, but the calibration will
 *                    take more time.
 * @param[in] down_counter_selection
 *   The clock which will be counted down cycles.
 *
 * @param[in] up_counter_selection
 *   The number of cycles generated by this clock will be counted and
 *   added up, the result can be given with
 *   @ref sl_clock_manager_get_rco_calibration_count.
 *
 * @param[in] continuous_calibration  Flag when true configures continuous calibration.
 *
 ******************************************************************************/
sl_status_t sl_clock_manager_configure_rco_calibration(uint32_t cycles,
                                                       sl_clock_manager_clock_calibration_t down_counter_selection,
                                                       sl_clock_manager_clock_calibration_t up_counter_selection,
                                                       bool continuous_calibration);

/***************************************************************************//**
 * Starts the RCO calibration.
 ******************************************************************************/
void sl_clock_manager_start_rco_calibration(void);

/***************************************************************************//**
 * Stops the RCO calibration.
 ******************************************************************************/
void sl_clock_manager_stop_rco_calibration(void);

/***************************************************************************//**
 * Wait for the RCO calibration to finish.
 ******************************************************************************/
void sl_clock_manager_wait_rco_calibration(void);

/***************************************************************************//**
 * Gets calibration count value, returns the value of the up counter as
 * @ref count.
 *
 * @param[out] count  Calibration count value
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_rco_calibration_count(uint32_t *count);

/***************************************************************************//**
 * Waits for USBPLL clock to be ready.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_wait_usbpll(void);

/***************************************************************************//**
 * When this callback function is called, it means that HFXO failed twice in
 * a row to start with normal configurations. This may mean that there is a
 * bad crystal. When getting this callback, HFXO is running but its properties
 * (frequency, precision) are not guaranteed. This should be considered as an
 * error situation.
 *
 * @note This callback will be called only when the
 *       SL_CLOCK_MANAGER_HFXO_SLEEPY_CRYSTAL_SUPPORT config is enabled
 ******************************************************************************/
void sl_clock_manager_hfxo_notify_consecutive_failed_startups(void);

/** @} (end addtogroup clock_manager) */

#ifdef __cplusplus
}
#endif

#endif // SL_CLOCK_MANAGER_H
