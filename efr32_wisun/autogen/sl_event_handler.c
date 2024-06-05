#include "sl_event_handler.h"

#include "em_chip.h"
#include "sl_interrupt_manager.h"
#include "sl_board_init.h"
#include "sl_clock_manager_init.h"
#include "sl_device_init_dcdc.h"
#include "sl_clock_manager.h"
#include "sl_device_init_emu.h"
#include "SEGGER_RTT.h"
#include "sl_memory_manager.h"
#include "pa_conversions_efr32.h"
#include "sl_rail_util_pti.h"
#include "sl_board_control.h"
#include "sl_sleeptimer.h"
#include "sl_debug_swo.h"
#include "gpiointerrupt.h"
#include "sl_iostream_init_eusart_instances.h"
#include "sl_iostream_stdlib_config.h"
#include "sl_mbedtls.h"
#include "nvm3_default.h"
#include "sl_simple_button_instances.h"
#include "sl_wisun_app_core.h"
#include "sl_wisun_app_setting.h"
#include "sl_wisun_cli_core.h"
#include "sl_wisun_event_mgr.h"
#include "sl_wisun_ping.h"
#include "sl_wisun_stack.h"
#include "sl_cli_instances.h"
#include "psa/crypto.h"
#include "cmsis_os2.h"
#include "sl_iostream_init_instances.h"
#include "sl_cos.h"
#include "sl_rail_util_rf_path_switch.h"

void sl_platform_init(void)
{
  CHIP_Init();
  sl_interrupt_manager_init();
  sl_board_preinit();
  sl_clock_manager_init();
  sl_device_init_dcdc();
  sl_clock_manager_runtime_init();
  sl_device_init_emu();
  SEGGER_RTT_Init();
  sl_memory_init();
  sl_board_init();
  nvm3_initDefault();
  osKernelInitialize();
}

void sl_kernel_start(void)
{
  osKernelStart();
}

void sl_driver_init(void)
{
  sl_debug_swo_init();
  GPIOINT_Init();
  sl_simple_button_init_instances();
  sl_cos_send_config();
}

void sl_service_init(void)
{
  sl_board_configure_vcom();
  sl_sleeptimer_init();
  sl_iostream_stdlib_disable_buffering();
  sl_mbedtls_init();
  psa_crypto_init();
  sl_iostream_init_instances();
  sl_cli_instances_init();
}

void sl_stack_init(void)
{
  sl_rail_util_pa_init();
  sl_rail_util_pti_init();
  sl_wisun_stack_init();
  sl_rail_util_rf_path_switch_init();
}

void sl_internal_app_init(void)
{
  sl_wisun_app_core_init();
  app_wisun_setting_init();
  app_wisun_cli_init();
  app_wisun_event_mgr_init();
  sl_wisun_ping_init();
}

void sl_iostream_init_instances(void)
{
  sl_iostream_eusart_init_instances();
}

