####################################################################
# Automatically-generated file. Do not edit!                       #
# Makefile Version 15                                              #
####################################################################

CSMP_AGENT_LIB_EFR32_WISUN_PATH ?= efr32_wisun
UNAME:=$(shell $(POSIX_TOOL_PATH)uname -s | $(POSIX_TOOL_PATH)sed -e 's/^\(CYGWIN\).*/\1/' | $(POSIX_TOOL_PATH)sed -e 's/^\(MINGW\).*/\1/')
COPIED_SDK_PATH ?= $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/gecko_sdk_2024.6.0

# This uses the explicit build rules below
PROJECT_SOURCE_FILES =

C_SOURCE_FILES   += $(filter %.c, $(PROJECT_SOURCE_FILES))
CXX_SOURCE_FILES += $(filter %.cpp, $(PROJECT_SOURCE_FILES))
CXX_SOURCE_FILES += $(filter %.cc, $(PROJECT_SOURCE_FILES))
ASM_SOURCE_FILES += $(filter %.s, $(PROJECT_SOURCE_FILES))
ASM_SOURCE_FILES += $(filter %.S, $(PROJECT_SOURCE_FILES))
LIB_FILES        += $(filter %.a, $(PROJECT_SOURCE_FILES))

C_DEFS += \
 '-DDEBUG_EFM=1' \
 '-DEFR32ZG28B322F1024IM68=1' \
 '-DSL_BOARD_NAME="BRD4401C"' \
 '-DSL_BOARD_REV="A01"' \
 '-DHARDWARE_BOARD_DEFAULT_RF_BAND_868=1' \
 '-DHARDWARE_BOARD_SUPPORTS_3_RF_BANDS=1' \
 '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' \
 '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_868=1' \
 '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_915=1' \
 '-DHFXO_FREQ=39000000' \
 '-DconfigNUM_SDK_THREAD_LOCAL_STORAGE_POINTERS=2' \
 '-DSL_COMPONENT_CATALOG_PRESENT=1' \
 '-DCMSIS_NVIC_VIRTUAL=1' \
 '-DCMSIS_NVIC_VIRTUAL_HEADER_FILE="cmsis_nvic_virtual.h"' \
 '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' \
 '-DSL_MEMORY_POOL_LIGHT=1' \
 '-DSL_CODE_COMPONENT_PERIPHERAL_SYSRTC=peripheral_sysrtc' \
 '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' \
 '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' \
 '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' \
 '-DRTT_USE_ASM=0' \
 '-DSEGGER_RTT_SECTION="SEGGER_RTT"' \
 '-DSL_CODE_COMPONENT_CORE=core' \
 '-DSL_CODE_COMPONENT_SLEEPTIMER=sleeptimer' \
 '-DHAVE_FFN=1' \
 '-DHAVE_LFN_PARENT=1' \
 '-DHAVE_LFN=1'

ASM_DEFS += \
 '-DDEBUG_EFM=1' \
 '-DEFR32ZG28B322F1024IM68=1' \
 '-DSL_BOARD_NAME="BRD4401C"' \
 '-DSL_BOARD_REV="A01"' \
 '-DHARDWARE_BOARD_DEFAULT_RF_BAND_868=1' \
 '-DHARDWARE_BOARD_SUPPORTS_3_RF_BANDS=1' \
 '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' \
 '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_868=1' \
 '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_915=1' \
 '-DHFXO_FREQ=39000000' \
 '-DconfigNUM_SDK_THREAD_LOCAL_STORAGE_POINTERS=2' \
 '-DSL_COMPONENT_CATALOG_PRESENT=1' \
 '-DCMSIS_NVIC_VIRTUAL=1' \
 '-DCMSIS_NVIC_VIRTUAL_HEADER_FILE="cmsis_nvic_virtual.h"' \
 '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' \
 '-DSL_MEMORY_POOL_LIGHT=1' \
 '-DSL_CODE_COMPONENT_PERIPHERAL_SYSRTC=peripheral_sysrtc' \
 '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' \
 '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' \
 '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' \
 '-DRTT_USE_ASM=0' \
 '-DSEGGER_RTT_SECTION="SEGGER_RTT"' \
 '-DSL_CODE_COMPONENT_CORE=core' \
 '-DSL_CODE_COMPONENT_SLEEPTIMER=sleeptimer' \
 '-DHAVE_FFN=1' \
 '-DHAVE_LFN_PARENT=1' \
 '-DHAVE_LFN=1'

INCLUDES += \
 -I$(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/config \
 -I$(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/config/rail \
 -I$(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/config/wisun \
 -I$(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen \
 -I$(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/. \
 -I$(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32ZG28/Include \
 -I$(COPIED_SDK_PATH)/app/wisun/component/app_project_info \
 -I$(COPIED_SDK_PATH)/platform/common/inc \
 -I$(COPIED_SDK_PATH)/hardware/board/inc \
 -I$(COPIED_SDK_PATH)/platform/driver/button/inc \
 -I$(COPIED_SDK_PATH)/platform/service/cli/inc \
 -I$(COPIED_SDK_PATH)/platform/service/cli/src \
 -I$(COPIED_SDK_PATH)/platform/service/clock_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/clock_manager/src \
 -I$(COPIED_SDK_PATH)/platform/CMSIS/Core/Include \
 -I$(COPIED_SDK_PATH)/platform/CMSIS/RTOS2/Include \
 -I$(COPIED_SDK_PATH)/hardware/driver/configuration_over_swo/inc \
 -I$(COPIED_SDK_PATH)/platform/driver/debug/inc \
 -I$(COPIED_SDK_PATH)/platform/service/device_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/device_init/inc \
 -I$(COPIED_SDK_PATH)/platform/emdrv/dmadrv/inc \
 -I$(COPIED_SDK_PATH)/platform/emdrv/dmadrv/inc/s2_signals \
 -I$(COPIED_SDK_PATH)/platform/emdrv/common/inc \
 -I$(COPIED_SDK_PATH)/platform/emlib/inc \
 -I$(COPIED_SDK_PATH)/util/third_party/freertos/cmsis/Include \
 -I$(COPIED_SDK_PATH)/util/third_party/freertos/kernel/include \
 -I$(COPIED_SDK_PATH)/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure \
 -I$(COPIED_SDK_PATH)/platform/emdrv/gpiointerrupt/inc \
 -I$(COPIED_SDK_PATH)/platform/service/interrupt_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/interrupt_manager/inc/arm \
 -I$(COPIED_SDK_PATH)/platform/service/iostream/inc \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/config \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/config/preset \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/inc \
 -I$(COPIED_SDK_PATH)/util/third_party/mbedtls/include \
 -I$(COPIED_SDK_PATH)/util/third_party/mbedtls/library \
 -I$(COPIED_SDK_PATH)/platform/service/memory_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/memory_manager/src \
 -I$(COPIED_SDK_PATH)/hardware/driver/mx25_flash_shutdown/inc/sl_mx25_flash_shutdown_usart \
 -I$(COPIED_SDK_PATH)/platform/emdrv/nvm3/inc \
 -I$(COPIED_SDK_PATH)/platform/peripheral/inc \
 -I$(COPIED_SDK_PATH)/util/third_party/printf \
 -I$(COPIED_SDK_PATH)/util/third_party/printf/inc \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/inc \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/common \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/protocol/ble \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/protocol/ieee802154 \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/protocol/wmbus \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/protocol/zwave \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/chip/efr32/efr32xg2x \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/protocol/sidewalk \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions/efr32xg28 \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_pti \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_rf_path_switch \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src \
 -I$(COPIED_SDK_PATH)/util/third_party/segger/systemview/SEGGER \
 -I$(COPIED_SDK_PATH)/util/silicon_labs/silabs_core/memory_manager \
 -I$(COPIED_SDK_PATH)/platform/common/toolchain/inc \
 -I$(COPIED_SDK_PATH)/platform/service/system/inc \
 -I$(COPIED_SDK_PATH)/app/wisun/component/app_core \
 -I$(COPIED_SDK_PATH)/app/wisun/component/app_setting \
 -I$(COPIED_SDK_PATH)/app/wisun/component/cli_util \
 -I$(COPIED_SDK_PATH)/app/wisun/component/event_manager \
 -I$(COPIED_SDK_PATH)/app/wisun/component/ping \
 -I$(COPIED_SDK_PATH)/app/wisun/component/ntp_timesync \
 -I$(COPIED_SDK_PATH)/app/wisun/component/trace_util \
 -I$(COPIED_SDK_PATH)/app/wisun/component/trace_util/nanostack/nanostack-libservice/mbed-client-libservice \
 -I$(COPIED_SDK_PATH)/platform/service/sleeptimer/inc \
 -I$(COPIED_SDK_PATH)/platform/service/udelay/inc \
 -I$(COPIED_SDK_PATH)/protocol/wisun/plugin \
 -I$(COPIED_SDK_PATH)/protocol/wisun/stack/inc \
 -I$(COPIED_SDK_PATH)/protocol/wisun/stack/inc/socket

GROUP_START =-Wl,--start-group
GROUP_END =-Wl,--end-group

PROJECT_LIBS = \
 -lgcc \
 -lc \
 -lm \
 -lnosys \
 $(COPIED_SDK_PATH)/platform/emdrv/nvm3/lib/libnvm3_CM33_gcc.a \
 $(COPIED_SDK_PATH)/platform/radio/rail_lib/autogen/librail_release/librail_efr32xg28_gcc_release.a \
 $(COPIED_SDK_PATH)/protocol/wisun/stack/libwisun_full_efr32xg2x_gcc_debug.a

LIBS += $(GROUP_START) $(PROJECT_LIBS) $(GROUP_END)

LIB_FILES += $(filter %.a, $(PROJECT_LIBS))

C_FLAGS += \
 -mcpu=cortex-m33 \
 -mthumb \
 -mfpu=fpv5-sp-d16 \
 -mfloat-abi=hard \
 -std=c18 \
 -Wall \
 -Wextra \
 -Og \
 -fdata-sections \
 -ffunction-sections \
 -fomit-frame-pointer \
 -imacros sl_gcc_preinclude.h \
 -mcmse \
 -fno-builtin-printf \
 -fno-builtin-sprintf \
 --specs=nano.specs \
 -g

CXX_FLAGS += \
 -mcpu=cortex-m33 \
 -mthumb \
 -mfpu=fpv5-sp-d16 \
 -mfloat-abi=hard \
 -std=c++17 \
 -fno-rtti \
 -fno-exceptions \
 -Wall \
 -Wextra \
 -Og \
 -fdata-sections \
 -ffunction-sections \
 -fomit-frame-pointer \
 -imacros sl_gcc_preinclude.h \
 -mcmse \
 -fno-builtin-printf \
 -fno-builtin-sprintf \
 --specs=nano.specs \
 -g

ASM_FLAGS += \
 -mcpu=cortex-m33 \
 -mthumb \
 -mfpu=fpv5-sp-d16 \
 -mfloat-abi=hard \
 -imacros sl_gcc_preinclude.h \
 -x assembler-with-cpp

LD_FLAGS += \
 -mcpu=cortex-m33 \
 -mthumb \
 -mfpu=fpv5-sp-d16 \
 -mfloat-abi=hard \
 -T"$(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/linkerfile.ld" \
 --specs=nano.specs \
 -Xlinker -Map=$(OUTPUT_DIR)/$(PROJECTNAME).map \
 -Wl,--wrap=_free_r -Wl,--wrap=_malloc_r -Wl,--wrap=_calloc_r -Wl,--wrap=_realloc_r \
 -Wl,--gc-sections \
 -Wl,--no-warn-rwx-segments


####################################################################
# Pre/Post Build Rules                                             #
####################################################################
pre-build:
	# No pre-build defined

# post-build: $(OUTPUT_DIR)/$(PROJECTNAME).out
post-build:
	# No post-build defined

####################################################################
# SDK Build Rules                                                  #
####################################################################
$(OUTPUT_DIR)/sdk/app/wisun/component/app_cli/sl_wisun_app_cli.o: $(COPIED_SDK_PATH)/app/wisun/component/app_cli/sl_wisun_app_cli.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/app_cli/sl_wisun_app_cli.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/app_cli/sl_wisun_app_cli.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/app_cli/sl_wisun_app_cli.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/app_cli/sl_wisun_app_cli.o

$(OUTPUT_DIR)/sdk/app/wisun/component/app_core/sl_wisun_app_core.o: $(COPIED_SDK_PATH)/app/wisun/component/app_core/sl_wisun_app_core.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/app_core/sl_wisun_app_core.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/app_core/sl_wisun_app_core.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/app_core/sl_wisun_app_core.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/app_core/sl_wisun_app_core.o

$(OUTPUT_DIR)/sdk/app/wisun/component/app_core/sl_wisun_app_core_util.o: $(COPIED_SDK_PATH)/app/wisun/component/app_core/sl_wisun_app_core_util.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/app_core/sl_wisun_app_core_util.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/app_core/sl_wisun_app_core_util.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/app_core/sl_wisun_app_core_util.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/app_core/sl_wisun_app_core_util.o

$(OUTPUT_DIR)/sdk/app/wisun/component/app_project_info/app_project_info.o: $(COPIED_SDK_PATH)/app/wisun/component/app_project_info/app_project_info.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/app_project_info/app_project_info.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/app_project_info/app_project_info.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/app_project_info/app_project_info.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/app_project_info/app_project_info.o

$(OUTPUT_DIR)/sdk/app/wisun/component/app_setting/sl_wisun_app_setting.o: $(COPIED_SDK_PATH)/app/wisun/component/app_setting/sl_wisun_app_setting.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/app_setting/sl_wisun_app_setting.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/app_setting/sl_wisun_app_setting.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/app_setting/sl_wisun_app_setting.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/app_setting/sl_wisun_app_setting.o

$(OUTPUT_DIR)/sdk/app/wisun/component/cli_util/sl_wisun_cli_core.o: $(COPIED_SDK_PATH)/app/wisun/component/cli_util/sl_wisun_cli_core.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/cli_util/sl_wisun_cli_core.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/cli_util/sl_wisun_cli_core.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/cli_util/sl_wisun_cli_core.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/cli_util/sl_wisun_cli_core.o

$(OUTPUT_DIR)/sdk/app/wisun/component/cli_util/sl_wisun_cli_settings.o: $(COPIED_SDK_PATH)/app/wisun/component/cli_util/sl_wisun_cli_settings.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/cli_util/sl_wisun_cli_settings.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/cli_util/sl_wisun_cli_settings.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/cli_util/sl_wisun_cli_settings.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/cli_util/sl_wisun_cli_settings.o

$(OUTPUT_DIR)/sdk/app/wisun/component/cli_util/sl_wisun_cli_util.o: $(COPIED_SDK_PATH)/app/wisun/component/cli_util/sl_wisun_cli_util.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/cli_util/sl_wisun_cli_util.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/cli_util/sl_wisun_cli_util.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/cli_util/sl_wisun_cli_util.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/cli_util/sl_wisun_cli_util.o

$(OUTPUT_DIR)/sdk/app/wisun/component/cli_util_cli/sl_wisun_cli_util_cli.o: $(COPIED_SDK_PATH)/app/wisun/component/cli_util_cli/sl_wisun_cli_util_cli.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/cli_util_cli/sl_wisun_cli_util_cli.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/cli_util_cli/sl_wisun_cli_util_cli.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/cli_util_cli/sl_wisun_cli_util_cli.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/cli_util_cli/sl_wisun_cli_util_cli.o

$(OUTPUT_DIR)/sdk/app/wisun/component/event_manager/sl_wisun_event_mgr.o: $(COPIED_SDK_PATH)/app/wisun/component/event_manager/sl_wisun_event_mgr.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/event_manager/sl_wisun_event_mgr.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/event_manager/sl_wisun_event_mgr.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/event_manager/sl_wisun_event_mgr.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/event_manager/sl_wisun_event_mgr.o

$(OUTPUT_DIR)/sdk/app/wisun/component/ping/sl_wisun_ping.o: $(COPIED_SDK_PATH)/app/wisun/component/ping/sl_wisun_ping.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/ping/sl_wisun_ping.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/ping/sl_wisun_ping.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/ping/sl_wisun_ping.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/ping/sl_wisun_ping.o

$(OUTPUT_DIR)/sdk/app/wisun/component/ping_cli/sl_wisun_ping_cli.o: $(COPIED_SDK_PATH)/app/wisun/component/ping_cli/sl_wisun_ping_cli.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/ping_cli/sl_wisun_ping_cli.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/ping_cli/sl_wisun_ping_cli.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/ping_cli/sl_wisun_ping_cli.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/ping_cli/sl_wisun_ping_cli.o

$(OUTPUT_DIR)/sdk/app/wisun/component/ntp_timesync/sl_wisun_ntp_timesync.o: $(COPIED_SDK_PATH)/app/wisun/component/ntp_timesync/sl_wisun_ntp_timesync.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/ntp_timesync/sl_wisun_ntp_timesync.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/ntp_timesync/sl_wisun_ntp_timesync.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/ntp_timesync/sl_wisun_ntp_timesync.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/ntp_timesync/sl_wisun_ntp_timesync.o

$(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libBits/common_functions.o: $(COPIED_SDK_PATH)/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libBits/common_functions.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libBits/common_functions.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libBits/common_functions.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libBits/common_functions.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libBits/common_functions.o

$(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libBits/common_inlined_functions.o: $(COPIED_SDK_PATH)/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libBits/common_inlined_functions.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libBits/common_inlined_functions.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libBits/common_inlined_functions.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libBits/common_inlined_functions.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libBits/common_inlined_functions.o

$(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libip6string/ip6tos.o: $(COPIED_SDK_PATH)/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libip6string/ip6tos.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libip6string/ip6tos.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libip6string/ip6tos.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libip6string/ip6tos.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libip6string/ip6tos.o

$(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libip6string/stoip6.o: $(COPIED_SDK_PATH)/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libip6string/stoip6.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libip6string/stoip6.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libip6string/stoip6.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libip6string/stoip6.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/nanostack/nanostack-libservice/source/libip6string/stoip6.o

$(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/sl_wisun_trace_util.o: $(COPIED_SDK_PATH)/app/wisun/component/trace_util/sl_wisun_trace_util.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/wisun/component/trace_util/sl_wisun_trace_util.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/wisun/component/trace_util/sl_wisun_trace_util.c
CDEPS += $(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/sl_wisun_trace_util.d
OBJS += $(OUTPUT_DIR)/sdk/app/wisun/component/trace_util/sl_wisun_trace_util.o

$(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_control_gpio.o: $(COPIED_SDK_PATH)/hardware/board/src/sl_board_control_gpio.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/hardware/board/src/sl_board_control_gpio.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/hardware/board/src/sl_board_control_gpio.c
CDEPS += $(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_control_gpio.d
OBJS += $(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_control_gpio.o

$(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_init.o: $(COPIED_SDK_PATH)/hardware/board/src/sl_board_init.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/hardware/board/src/sl_board_init.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/hardware/board/src/sl_board_init.c
CDEPS += $(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_init.d
OBJS += $(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_init.o

$(OUTPUT_DIR)/sdk/hardware/driver/configuration_over_swo/src/sl_cos.o: $(COPIED_SDK_PATH)/hardware/driver/configuration_over_swo/src/sl_cos.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/hardware/driver/configuration_over_swo/src/sl_cos.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/hardware/driver/configuration_over_swo/src/sl_cos.c
CDEPS += $(OUTPUT_DIR)/sdk/hardware/driver/configuration_over_swo/src/sl_cos.d
OBJS += $(OUTPUT_DIR)/sdk/hardware/driver/configuration_over_swo/src/sl_cos.o

$(OUTPUT_DIR)/sdk/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.o: $(COPIED_SDK_PATH)/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.c
CDEPS += $(OUTPUT_DIR)/sdk/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.d
OBJS += $(OUTPUT_DIR)/sdk/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.o

$(OUTPUT_DIR)/sdk/platform/CMSIS/RTOS2/Source/os_systick.o: $(COPIED_SDK_PATH)/platform/CMSIS/RTOS2/Source/os_systick.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/CMSIS/RTOS2/Source/os_systick.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/CMSIS/RTOS2/Source/os_systick.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/CMSIS/RTOS2/Source/os_systick.d
OBJS += $(OUTPUT_DIR)/sdk/platform/CMSIS/RTOS2/Source/os_systick.o

$(OUTPUT_DIR)/sdk/platform/common/src/sl_assert.o: $(COPIED_SDK_PATH)/platform/common/src/sl_assert.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/common/src/sl_assert.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/common/src/sl_assert.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_assert.d
OBJS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_assert.o

$(OUTPUT_DIR)/sdk/platform/common/src/sl_core_cortexm.o: $(COPIED_SDK_PATH)/platform/common/src/sl_core_cortexm.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/common/src/sl_core_cortexm.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/common/src/sl_core_cortexm.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_core_cortexm.d
OBJS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_core_cortexm.o

$(OUTPUT_DIR)/sdk/platform/common/src/sl_slist.o: $(COPIED_SDK_PATH)/platform/common/src/sl_slist.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/common/src/sl_slist.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/common/src/sl_slist.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_slist.d
OBJS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_slist.o

$(OUTPUT_DIR)/sdk/platform/common/src/sl_string.o: $(COPIED_SDK_PATH)/platform/common/src/sl_string.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/common/src/sl_string.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/common/src/sl_string.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_string.d
OBJS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_string.o

$(OUTPUT_DIR)/sdk/platform/common/src/sl_syscalls.o: $(COPIED_SDK_PATH)/platform/common/src/sl_syscalls.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/common/src/sl_syscalls.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/common/src/sl_syscalls.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_syscalls.d
OBJS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_syscalls.o

$(OUTPUT_DIR)/sdk/platform/common/src/sli_cmsis_os2_ext_task_register.o: $(COPIED_SDK_PATH)/platform/common/src/sli_cmsis_os2_ext_task_register.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/common/src/sli_cmsis_os2_ext_task_register.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/common/src/sli_cmsis_os2_ext_task_register.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/common/src/sli_cmsis_os2_ext_task_register.d
OBJS += $(OUTPUT_DIR)/sdk/platform/common/src/sli_cmsis_os2_ext_task_register.o

$(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFR32ZG28/Source/startup_efr32zg28.o: $(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32ZG28/Source/startup_efr32zg28.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32ZG28/Source/startup_efr32zg28.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32ZG28/Source/startup_efr32zg28.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFR32ZG28/Source/startup_efr32zg28.d
OBJS += $(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFR32ZG28/Source/startup_efr32zg28.o

$(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFR32ZG28/Source/system_efr32zg28.o: $(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32ZG28/Source/system_efr32zg28.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32ZG28/Source/system_efr32zg28.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32ZG28/Source/system_efr32zg28.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFR32ZG28/Source/system_efr32zg28.d
OBJS += $(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFR32ZG28/Source/system_efr32zg28.o

$(OUTPUT_DIR)/sdk/platform/driver/button/src/sl_button.o: $(COPIED_SDK_PATH)/platform/driver/button/src/sl_button.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/driver/button/src/sl_button.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/driver/button/src/sl_button.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/driver/button/src/sl_button.d
OBJS += $(OUTPUT_DIR)/sdk/platform/driver/button/src/sl_button.o

$(OUTPUT_DIR)/sdk/platform/driver/button/src/sl_simple_button.o: $(COPIED_SDK_PATH)/platform/driver/button/src/sl_simple_button.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/driver/button/src/sl_simple_button.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/driver/button/src/sl_simple_button.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/driver/button/src/sl_simple_button.d
OBJS += $(OUTPUT_DIR)/sdk/platform/driver/button/src/sl_simple_button.o

$(OUTPUT_DIR)/sdk/platform/driver/debug/src/sl_debug_swo.o: $(COPIED_SDK_PATH)/platform/driver/debug/src/sl_debug_swo.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/driver/debug/src/sl_debug_swo.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/driver/debug/src/sl_debug_swo.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/driver/debug/src/sl_debug_swo.d
OBJS += $(OUTPUT_DIR)/sdk/platform/driver/debug/src/sl_debug_swo.o

$(OUTPUT_DIR)/sdk/platform/emdrv/dmadrv/src/dmadrv.o: $(COPIED_SDK_PATH)/platform/emdrv/dmadrv/src/dmadrv.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emdrv/dmadrv/src/dmadrv.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emdrv/dmadrv/src/dmadrv.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emdrv/dmadrv/src/dmadrv.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emdrv/dmadrv/src/dmadrv.o

$(OUTPUT_DIR)/sdk/platform/emdrv/gpiointerrupt/src/gpiointerrupt.o: $(COPIED_SDK_PATH)/platform/emdrv/gpiointerrupt/src/gpiointerrupt.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emdrv/gpiointerrupt/src/gpiointerrupt.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emdrv/gpiointerrupt/src/gpiointerrupt.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emdrv/gpiointerrupt/src/gpiointerrupt.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emdrv/gpiointerrupt/src/gpiointerrupt.o

$(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_default_common_linker.o: $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_default_common_linker.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_default_common_linker.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_default_common_linker.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_default_common_linker.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_default_common_linker.o

$(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_hal_flash.o: $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_hal_flash.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_hal_flash.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_hal_flash.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_hal_flash.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_hal_flash.o

$(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_lock.o: $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_lock.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_lock.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_lock.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_lock.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_lock.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_burtc.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_burtc.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_burtc.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_burtc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_burtc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_burtc.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_cmu.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_cmu.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_cmu.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_cmu.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_cmu.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_cmu.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_core.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_core.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_core.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_core.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_core.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_core.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_emu.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_emu.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_emu.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_emu.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_emu.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_emu.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_eusart.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_eusart.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_eusart.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_eusart.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_eusart.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_eusart.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_gpio.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_gpio.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_gpio.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_gpio.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_gpio.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_gpio.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_ldma.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_ldma.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_ldma.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_ldma.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_ldma.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_ldma.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_msc.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_msc.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_msc.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_msc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_msc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_msc.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_prs.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_prs.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_prs.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_prs.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_prs.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_prs.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_se.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_se.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_se.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_se.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_se.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_se.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_system.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_system.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_system.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_system.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_system.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_system.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_timer.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_timer.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_timer.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_timer.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_timer.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_timer.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_usart.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_usart.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_usart.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_usart.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_usart.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_usart.o

$(OUTPUT_DIR)/sdk/platform/peripheral/src/peripheral_sysrtc.o: $(COPIED_SDK_PATH)/platform/peripheral/src/peripheral_sysrtc.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/peripheral/src/peripheral_sysrtc.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/peripheral/src/peripheral_sysrtc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/peripheral/src/peripheral_sysrtc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/peripheral/src/peripheral_sysrtc.o

$(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.o: $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.d
OBJS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.o

$(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.o: $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.d
OBJS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.o

$(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.o: $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.d
OBJS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.o

$(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_rf_path_switch/sl_rail_util_rf_path_switch.o: $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_rf_path_switch/sl_rail_util_rf_path_switch.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_rf_path_switch/sl_rail_util_rf_path_switch.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_rf_path_switch/sl_rail_util_rf_path_switch.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_rf_path_switch/sl_rail_util_rf_path_switch.d
OBJS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_rf_path_switch/sl_rail_util_rf_path_switch.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_attestation.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_attestation.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_attestation.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_attestation.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_attestation.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_attestation.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_cipher.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_cipher.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_cipher.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_cipher.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_cipher.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_cipher.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_entropy.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_entropy.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_entropy.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_entropy.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_entropy.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_entropy.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_hash.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_hash.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_hash.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_hash.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_hash.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_hash.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_key_derivation.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_key_derivation.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_key_derivation.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_key_derivation.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_key_derivation.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_key_derivation.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_key_handling.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_key_handling.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_key_handling.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_key_handling.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_key_handling.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_key_handling.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_signature.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_signature.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_signature.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_signature.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_signature.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_signature.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_util.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_util.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_util.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_util.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_util.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_util.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_ccm.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_ccm.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_ccm.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_ccm.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_ccm.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_ccm.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_ecdsa_ecdh.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_ecdsa_ecdh.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_ecdsa_ecdh.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_ecdsa_ecdh.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_ecdsa_ecdh.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_ecdsa_ecdh.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_sha.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_sha.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_sha.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_sha.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_sha.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_sha.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/se_aes.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/se_aes.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/se_aes.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/se_aes.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/se_aes.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/se_aes.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sl_entropy_hardware.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sl_entropy_hardware.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sl_entropy_hardware.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sl_entropy_hardware.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sl_entropy_hardware.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sl_entropy_hardware.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sl_mbedtls.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sl_mbedtls.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sl_mbedtls.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sl_mbedtls.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sl_mbedtls.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sl_mbedtls.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sl_psa_crypto.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sl_psa_crypto.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sl_psa_crypto.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sl_psa_crypto.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sl_psa_crypto.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sl_psa_crypto.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sli_psa_crypto.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sli_psa_crypto.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sli_psa_crypto.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sli_psa_crypto.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sli_psa_crypto.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sli_psa_crypto.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sl_psa_its_nvm3.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sl_psa_its_nvm3.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sl_psa_its_nvm3.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sl_psa_its_nvm3.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sl_psa_its_nvm3.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sl_psa_its_nvm3.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_common.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_common.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_common.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_common.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_common.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_common.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_init.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_init.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_init.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_init.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_init.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_init.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_trng.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_trng.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_trng.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_trng.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_trng.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_trng.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_aead.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_aead.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_aead.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_aead.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_aead.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_aead.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_builtin_keys.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_builtin_keys.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_builtin_keys.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_builtin_keys.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_builtin_keys.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_builtin_keys.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_cipher.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_cipher.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_cipher.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_cipher.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_cipher.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_cipher.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_derivation.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_derivation.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_derivation.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_derivation.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_derivation.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_derivation.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_management.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_management.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_management.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_management.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_management.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_management.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_mac.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_mac.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_mac.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_mac.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_mac.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_mac.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_signature.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_signature.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_signature.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_signature.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_signature.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_signature.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_aead.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_aead.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_aead.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_aead.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_aead.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_aead.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_cipher.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_cipher.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_cipher.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_cipher.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_cipher.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_cipher.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_mac.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_mac.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_mac.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_mac.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_mac.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_mac.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_key_derivation.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_key_derivation.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_key_derivation.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_key_derivation.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_key_derivation.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_key_derivation.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_aead.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_aead.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_aead.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_aead.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_aead.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_aead.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_cipher.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_cipher.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_cipher.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_cipher.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_cipher.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_cipher.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_hash.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_hash.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_hash.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_hash.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_hash.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_hash.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_mac.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_mac.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_mac.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_mac.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_mac.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_mac.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_key_derivation.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_key_derivation.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_key_derivation.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_key_derivation.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_key_derivation.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_key_derivation.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_version_dependencies.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_version_dependencies.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_version_dependencies.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_version_dependencies.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_version_dependencies.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_version_dependencies.o

$(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli.o: $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli.o

$(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_arguments.o: $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_arguments.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_arguments.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_arguments.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_arguments.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_arguments.o

$(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_command.o: $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_command.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_command.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_command.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_command.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_command.o

$(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_input.o: $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_input.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_input.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_input.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_input.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_input.o

$(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_io.o: $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_io.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_io.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_io.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_io.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_io.o

$(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_tokenize.o: $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_tokenize.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_tokenize.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/cli/src/sl_cli_tokenize.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_tokenize.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/cli/src/sl_cli_tokenize.o

$(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager.o: $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager.o

$(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager_hal_s2.o: $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager_hal_s2.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager_hal_s2.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager_hal_s2.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager_hal_s2.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager_hal_s2.o

$(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_dcdc_s2.o: $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_dcdc_s2.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_dcdc_s2.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_dcdc_s2.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_dcdc_s2.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_dcdc_s2.o

$(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_emu_s2.o: $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_emu_s2.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_emu_s2.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_emu_s2.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_emu_s2.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_emu_s2.o

$(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_hfxo_s2.o: $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_hfxo_s2.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_hfxo_s2.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_hfxo_s2.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_hfxo_s2.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_hfxo_s2.o

$(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_lfxo_s2.o: $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_lfxo_s2.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_lfxo_s2.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_lfxo_s2.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_lfxo_s2.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_lfxo_s2.o

$(OUTPUT_DIR)/sdk/platform/service/device_manager/clocks/sl_clock_abstraction_efr32xg28.o: $(COPIED_SDK_PATH)/platform/service/device_manager/clocks/sl_clock_abstraction_efr32xg28.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/device_manager/clocks/sl_clock_abstraction_efr32xg28.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/device_manager/clocks/sl_clock_abstraction_efr32xg28.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/clocks/sl_clock_abstraction_efr32xg28.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/clocks/sl_clock_abstraction_efr32xg28.o

$(OUTPUT_DIR)/sdk/platform/service/device_manager/devices/sl_device_abstraction_efr32xg28.o: $(COPIED_SDK_PATH)/platform/service/device_manager/devices/sl_device_abstraction_efr32xg28.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/device_manager/devices/sl_device_abstraction_efr32xg28.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/device_manager/devices/sl_device_abstraction_efr32xg28.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/devices/sl_device_abstraction_efr32xg28.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/devices/sl_device_abstraction_efr32xg28.o

$(OUTPUT_DIR)/sdk/platform/service/device_manager/src/sl_device_abstraction_peripheral.o: $(COPIED_SDK_PATH)/platform/service/device_manager/src/sl_device_abstraction_peripheral.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/device_manager/src/sl_device_abstraction_peripheral.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/device_manager/src/sl_device_abstraction_peripheral.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/src/sl_device_abstraction_peripheral.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/src/sl_device_abstraction_peripheral.o

$(OUTPUT_DIR)/sdk/platform/service/device_manager/src/sl_technology_abstraction_clock.o: $(COPIED_SDK_PATH)/platform/service/device_manager/src/sl_technology_abstraction_clock.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/device_manager/src/sl_technology_abstraction_clock.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/device_manager/src/sl_technology_abstraction_clock.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/src/sl_technology_abstraction_clock.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/src/sl_technology_abstraction_clock.o

$(OUTPUT_DIR)/sdk/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.o: $(COPIED_SDK_PATH)/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.o

$(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream.o: $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream.o

$(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_eusart.o: $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_eusart.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_eusart.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_eusart.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_eusart.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_eusart.o

$(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_retarget_stdio.o: $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_retarget_stdio.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_retarget_stdio.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_retarget_stdio.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_retarget_stdio.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_retarget_stdio.o

$(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_stdlib_config.o: $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_stdlib_config.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_stdlib_config.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_stdlib_config.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_stdlib_config.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_stdlib_config.o

$(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_uart.o: $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_uart.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_uart.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_uart.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_uart.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_uart.o

$(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager.o: $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager.o

$(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.o: $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.o

$(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_pool_light.o: $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_pool_light.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_pool_light.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_pool_light.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_pool_light.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_pool_light.o

$(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_region.o: $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_region.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_region.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_region.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_region.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_region.o

$(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_retarget.o: $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_retarget.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_retarget.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_retarget.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_retarget.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_retarget.o

$(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sli_memory_manager_common.o: $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sli_memory_manager_common.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sli_memory_manager_common.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sli_memory_manager_common.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sli_memory_manager_common.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sli_memory_manager_common.o

$(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer.o: $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer.o

$(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.o: $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.o

$(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.o: $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.o

$(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.o: $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.o

$(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_init.o: $(COPIED_SDK_PATH)/platform/service/system/src/sl_system_init.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/system/src/sl_system_init.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/system/src/sl_system_init.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_init.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_init.o

$(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_kernel.o: $(COPIED_SDK_PATH)/platform/service/system/src/sl_system_kernel.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/system/src/sl_system_kernel.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/system/src/sl_system_kernel.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_kernel.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_kernel.o

$(OUTPUT_DIR)/sdk/platform/service/udelay/src/sl_udelay.o: $(COPIED_SDK_PATH)/platform/service/udelay/src/sl_udelay.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/udelay/src/sl_udelay.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/udelay/src/sl_udelay.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/udelay/src/sl_udelay.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/udelay/src/sl_udelay.o

$(OUTPUT_DIR)/sdk/platform/service/udelay/src/sl_udelay_armv6m_gcc.o: $(COPIED_SDK_PATH)/platform/service/udelay/src/sl_udelay_armv6m_gcc.S
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/udelay/src/sl_udelay_armv6m_gcc.S'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(ASMFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/udelay/src/sl_udelay_armv6m_gcc.S
ASMDEPS_S += $(OUTPUT_DIR)/sdk/platform/service/udelay/src/sl_udelay_armv6m_gcc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/udelay/src/sl_udelay_armv6m_gcc.o

$(OUTPUT_DIR)/sdk/protocol/wisun/plugin/sl_wisun_legacy_socket_wrapper.o: $(COPIED_SDK_PATH)/protocol/wisun/plugin/sl_wisun_legacy_socket_wrapper.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/protocol/wisun/plugin/sl_wisun_legacy_socket_wrapper.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/protocol/wisun/plugin/sl_wisun_legacy_socket_wrapper.c
CDEPS += $(OUTPUT_DIR)/sdk/protocol/wisun/plugin/sl_wisun_legacy_socket_wrapper.d
OBJS += $(OUTPUT_DIR)/sdk/protocol/wisun/plugin/sl_wisun_legacy_socket_wrapper.o

$(OUTPUT_DIR)/sdk/util/third_party/freertos/cmsis/Source/cmsis_os2.o: $(COPIED_SDK_PATH)/util/third_party/freertos/cmsis/Source/cmsis_os2.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/freertos/cmsis/Source/cmsis_os2.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/freertos/cmsis/Source/cmsis_os2.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/cmsis/Source/cmsis_os2.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/cmsis/Source/cmsis_os2.o

$(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/croutine.o: $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/croutine.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/croutine.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/croutine.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/croutine.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/croutine.o

$(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/event_groups.o: $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/event_groups.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/event_groups.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/event_groups.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/event_groups.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/event_groups.o

$(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/list.o: $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/list.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/list.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/list.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/list.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/list.o

$(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.o: $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.o

$(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.o: $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.o

$(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/portable/MemMang/heap_3.o: $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/portable/MemMang/heap_3.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/portable/MemMang/heap_3.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/portable/MemMang/heap_3.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/portable/MemMang/heap_3.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/portable/MemMang/heap_3.o

$(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/queue.o: $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/queue.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/queue.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/queue.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/queue.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/queue.o

$(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/stream_buffer.o: $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/stream_buffer.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/stream_buffer.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/stream_buffer.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/stream_buffer.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/stream_buffer.o

$(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/tasks.o: $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/tasks.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/tasks.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/tasks.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/tasks.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/tasks.o

$(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/timers.o: $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/timers.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/timers.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/freertos/kernel/timers.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/timers.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/freertos/kernel/timers.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/aes.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/aes.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/aes.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/aes.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/aes.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/aes.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/asn1parse.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/asn1parse.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/asn1parse.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/asn1parse.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/asn1parse.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/asn1parse.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/asn1write.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/asn1write.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/asn1write.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/asn1write.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/asn1write.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/asn1write.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/base64.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/base64.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/base64.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/base64.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/base64.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/base64.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/bignum.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/bignum.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/bignum.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/bignum.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/bignum.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/bignum.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/bignum_core.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/bignum_core.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/bignum_core.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/bignum_core.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/bignum_core.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/bignum_core.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/bignum_mod.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/bignum_mod.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/bignum_mod.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/bignum_mod.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/bignum_mod.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/bignum_mod.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/bignum_mod_raw.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/bignum_mod_raw.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/bignum_mod_raw.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/bignum_mod_raw.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/bignum_mod_raw.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/bignum_mod_raw.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ccm.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ccm.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ccm.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ccm.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ccm.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ccm.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/cipher.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/cipher.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/cipher.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/cipher.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/cipher.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/cipher.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/cipher_wrap.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/cipher_wrap.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/cipher_wrap.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/cipher_wrap.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/cipher_wrap.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/cipher_wrap.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/constant_time.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/constant_time.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/constant_time.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/constant_time.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/constant_time.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/constant_time.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ctr_drbg.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ctr_drbg.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ctr_drbg.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ctr_drbg.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ctr_drbg.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ctr_drbg.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecdh.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecdh.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecdh.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecdh.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecdh.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecdh.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecdsa.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecdsa.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecdsa.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecdsa.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecdsa.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecdsa.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecp.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecp.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecp.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecp.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecp.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecp.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecp_curves.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecp_curves.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecp_curves.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecp_curves.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecp_curves.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecp_curves.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecp_curves_new.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecp_curves_new.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecp_curves_new.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ecp_curves_new.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecp_curves_new.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ecp_curves_new.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/entropy.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/entropy.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/entropy.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/entropy.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/entropy.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/entropy.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/entropy_poll.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/entropy_poll.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/entropy_poll.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/entropy_poll.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/entropy_poll.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/entropy_poll.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/hmac_drbg.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/hmac_drbg.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/hmac_drbg.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/hmac_drbg.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/hmac_drbg.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/hmac_drbg.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/md.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/md.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/md.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/md.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/md.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/md.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/nist_kw.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/nist_kw.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/nist_kw.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/nist_kw.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/nist_kw.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/nist_kw.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/oid.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/oid.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/oid.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/oid.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/oid.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/oid.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pem.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pem.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pem.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pem.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pem.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pem.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pk.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pk.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pk.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pk.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pk.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pk.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pk_wrap.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pk_wrap.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pk_wrap.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pk_wrap.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pk_wrap.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pk_wrap.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pkparse.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pkparse.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pkparse.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pkparse.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pkparse.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pkparse.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pkwrite.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pkwrite.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pkwrite.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/pkwrite.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pkwrite.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/pkwrite.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/platform.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/platform.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/platform.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/platform.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/platform.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/platform.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/platform_util.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/platform_util.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/platform_util.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/platform_util.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/platform_util.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/platform_util.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_aead.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_aead.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_aead.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_aead.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_aead.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_aead.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_cipher.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_cipher.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_cipher.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_cipher.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_cipher.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_cipher.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_client.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_client.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_client.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_client.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_client.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_client.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_driver_wrappers_no_static.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_driver_wrappers_no_static.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_driver_wrappers_no_static.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_driver_wrappers_no_static.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_driver_wrappers_no_static.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_driver_wrappers_no_static.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_ecp.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_ecp.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_ecp.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_ecp.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_ecp.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_ecp.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_ffdh.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_ffdh.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_ffdh.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_ffdh.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_ffdh.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_ffdh.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_hash.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_hash.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_hash.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_hash.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_hash.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_hash.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_mac.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_mac.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_mac.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_mac.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_mac.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_mac.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_pake.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_pake.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_pake.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_pake.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_pake.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_pake.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_rsa.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_rsa.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_rsa.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_rsa.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_rsa.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_rsa.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_se.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_se.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_se.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_se.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_se.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_se.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_slot_management.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_slot_management.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_slot_management.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_slot_management.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_slot_management.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_slot_management.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_storage.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_storage.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_storage.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_storage.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_storage.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_storage.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_util.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_util.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_util.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_util.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_util.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_util.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/sha1.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/sha1.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/sha1.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/sha1.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/sha1.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/sha1.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/sha256.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/sha256.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/sha256.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/sha256.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/sha256.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/sha256.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_cache.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_cache.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_cache.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_cache.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_cache.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_cache.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_ciphersuites.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_ciphersuites.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_ciphersuites.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_ciphersuites.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_ciphersuites.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_ciphersuites.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_client.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_client.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_client.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_client.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_client.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_client.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_cookie.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_cookie.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_cookie.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_cookie.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_cookie.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_cookie.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_debug_helpers_generated.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_debug_helpers_generated.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_debug_helpers_generated.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_debug_helpers_generated.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_debug_helpers_generated.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_debug_helpers_generated.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_msg.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_msg.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_msg.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_msg.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_msg.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_msg.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_ticket.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_ticket.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_ticket.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_ticket.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_ticket.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_ticket.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_tls.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_tls.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_tls.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_tls.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_tls.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_tls.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_tls12_client.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_tls12_client.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_tls12_client.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_tls12_client.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_tls12_client.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_tls12_client.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_tls12_server.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_tls12_server.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_tls12_server.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/ssl_tls12_server.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_tls12_server.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/ssl_tls12_server.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/threading.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/threading.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/threading.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/threading.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/threading.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/threading.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509_create.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509_create.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509_create.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509_create.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509_create.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509_create.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509_crl.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509_crl.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509_crl.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509_crl.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509_crl.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509_crl.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509_crt.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509_crt.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509_crt.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509_crt.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509_crt.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509_crt.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509_csr.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509_csr.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509_csr.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509_csr.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509_csr.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509_csr.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509write.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509write.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509write.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509write.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509write.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509write.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509write_crt.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509write_crt.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509write_crt.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509write_crt.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509write_crt.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509write_crt.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509write_csr.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509write_csr.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509write_csr.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/x509write_csr.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509write_csr.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/x509write_csr.o

$(OUTPUT_DIR)/sdk/util/third_party/printf/printf.o: $(COPIED_SDK_PATH)/util/third_party/printf/printf.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/printf/printf.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/printf/printf.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/printf/printf.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/printf/printf.o

$(OUTPUT_DIR)/sdk/util/third_party/printf/src/iostream_printf.o: $(COPIED_SDK_PATH)/util/third_party/printf/src/iostream_printf.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/printf/src/iostream_printf.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/printf/src/iostream_printf.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/printf/src/iostream_printf.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/printf/src/iostream_printf.o

$(OUTPUT_DIR)/sdk/util/third_party/segger/systemview/SEGGER/SEGGER_RTT.o: $(COPIED_SDK_PATH)/util/third_party/segger/systemview/SEGGER/SEGGER_RTT.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/segger/systemview/SEGGER/SEGGER_RTT.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/segger/systemview/SEGGER/SEGGER_RTT.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/segger/systemview/SEGGER/SEGGER_RTT.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/segger/systemview/SEGGER/SEGGER_RTT.o



$(OUTPUT_DIR)/project/autogen/rail_config.o: $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/rail_config.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/rail_config.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/rail_config.c
CDEPS += $(OUTPUT_DIR)/project/autogen/rail_config.d
OBJS += $(OUTPUT_DIR)/project/autogen/rail_config.o

$(OUTPUT_DIR)/project/autogen/sl_board_default_init.o: $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_board_default_init.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_board_default_init.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_board_default_init.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_board_default_init.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_board_default_init.o

$(OUTPUT_DIR)/project/autogen/sl_cli_command_table.o: $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_cli_command_table.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_cli_command_table.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_cli_command_table.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_cli_command_table.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_cli_command_table.o

$(OUTPUT_DIR)/project/autogen/sl_cli_instances.o: $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_cli_instances.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_cli_instances.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_cli_instances.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_cli_instances.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_cli_instances.o

$(OUTPUT_DIR)/project/autogen/sl_device_init_clocks.o: $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_device_init_clocks.c
	@$(POSIX_TOOL_PATH)echo 'Building $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_device_init_clocks.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_device_init_clocks.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_device_init_clocks.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_device_init_clocks.o

$(OUTPUT_DIR)/project/autogen/sl_event_handler.o: $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_event_handler.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_event_handler.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_event_handler.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_event_handler.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_event_handler.o

$(OUTPUT_DIR)/project/autogen/sl_iostream_handles.o: $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_iostream_handles.c
	@$(POSIX_TOOL_PATH)echo 'Building $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_iostream_handles.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_iostream_handles.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_iostream_handles.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_iostream_handles.o

$(OUTPUT_DIR)/project/autogen/sl_iostream_init_eusart_instances.o: $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_iostream_init_eusart_instances.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_iostream_init_eusart_instances.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_iostream_init_eusart_instances.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_iostream_init_eusart_instances.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_iostream_init_eusart_instances.o

$(OUTPUT_DIR)/project/autogen/sl_simple_button_instances.o: $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_simple_button_instances.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_simple_button_instances.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_simple_button_instances.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_simple_button_instances.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_simple_button_instances.o

$(OUTPUT_DIR)/project/autogen/sl_wisun_config.o: $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_wisun_config.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_wisun_config.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sl_wisun_config.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_wisun_config.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_wisun_config.o

$(OUTPUT_DIR)/project/autogen/sli_cli_hooks.o: $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sli_cli_hooks.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sli_cli_hooks.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(CSMP_AGENT_LIB_EFR32_WISUN_PATH)/autogen/sli_cli_hooks.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sli_cli_hooks.d
OBJS += $(OUTPUT_DIR)/project/autogen/sli_cli_hooks.o


# Automatically-generated Simplicity Studio Metadata
# Please do not edit or delete these lines!
# SIMPLICITY_STUDIO_METADATA=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END_SIMPLICITY_STUDIO_METADATA
# END OF METADATA