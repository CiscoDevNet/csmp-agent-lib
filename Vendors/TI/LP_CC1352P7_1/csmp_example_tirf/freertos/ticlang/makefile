# SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR is passed in by the parent build
# (sample/ti_simplelink_wisun.target) or can be set on the command line:
#   make SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR=/path/to/simplelink_cc13xx_cc26xx_sdk_8_32_00_07
SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR ?=

-include $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/imports.mak

CC = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"
LNK = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"
HEX = "$(TICLANG_ARMCOMPILER)/bin/tiarmobjcopy"

SYSCONFIG_GUI_TOOL = $(dir $(SYSCONFIG_TOOL))sysconfig_gui$(suffix $(SYSCONFIG_TOOL))
SYSCFG_CMD_STUB = $(SYSCONFIG_TOOL) --compiler ticlang --product $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_GUI_CMD_STUB = $(SYSCONFIG_GUI_TOOL) --compiler ticlang --product $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_FILES := $(shell $(SYSCFG_CMD_STUB) --listGeneratedFiles --listReferencedFiles --output . ../../freertos/csmp_example.syscfg)

SYSCFG_C_FILES = $(filter %.c,$(SYSCFG_FILES))
SYSCFG_H_FILES = $(filter %.h,$(SYSCFG_FILES))
SYSCFG_OPT_FILES = $(filter %.opt,$(SYSCFG_FILES))

OBJECTS = rcp_rcp_mlme.obj csmp_sample_tlvs_ti_simplelink_wisun_tlvs.obj mbed_ti-nv-driver_nvocmp.obj mbed_mbednanostack2tirtos_nanostack_nanostack-hal-mbed-cmsis-rtos_arm_hal_interrupt.obj rcp_rcp_queue.obj rcp_rcp_host.obj software_stack_ti15_4mac_osal_port_osal_port.obj application_main.obj software_stack_ti15_4mac_radio_configuration_mac_user_config.obj mbed_mbednanostack2tirtos_platform_ns_trace.obj mbed_mbednanostack2tirtos_nanostack_nanostack-hal-mbed-cmsis-rtos_ns_event_loop_mutex.obj mbed_mbednanostack2tirtos_nanostack_nanostack-hal-mbed-cmsis-rtos_arm_hal_timer.obj mbed_nanostack_mac_sw_mac.obj rcp_rcp_host_helpers.obj software_stack_ti15_4mac_stack_user_api_api_mac_timac_fhss_config.obj mbed_mbednanostack2tirtos_nanostack_nanostack-hal-mbed-cmsis-rtos_ns_hal_init.obj rcp_rcp_mcps_sap.obj mbed_mbednanostack2tirtos_nanostack_nanostack-hal-mbed-cmsis-rtos_arm_hal_random.obj software_stack_ti15_4mac_macTask.obj mbed_nanostack_mbed-mesh-api_source_wisun_tasklet.obj software_stack_ti15_4mac_mac_settings_mac_settings.obj rcp_rcp_lmac.obj mbed_ti-nv-driver_crc.obj mbed_ti-rf-driver_source_RFQueue.obj csmp_osal_osal_ti_simplelink_wisun.obj software_stack_ti15_4mac_mac_settings_mac_cfg.obj software_stack_ti15_4mac_osal_port_osal_port_timers.obj mbed_nanostack_mac_mac_header_helper_functions.obj rcp_rcp_security_mib.obj application_application.obj rcp_rcp_lmac_helpers.obj mbed_mbednanostack2tirtos_nanostack_nanostack-hal-mbed-cmsis-rtos_ns_event_loop.obj mbed_mbednanostack2tirtos_nanostack_nanostack-hal-mbed-cmsis-rtos_arm_hal_fhss_timer.obj wisun_ncp_platform_uart.obj wisun_ncp_platform_debug_uart.obj wisun_ncp_src_ncp_ncp_base_mtd.obj $(patsubst %.c,%.obj,$(notdir $(SYSCFG_C_FILES)))
NAME = ns_node_csmp

# Each config-specific .mak file will initialize appropriate options (AFLAGS, CFLAGS, LFLAGS),
# which will then be appended to below.
CONFIG := RELEASE
ifeq ($(CONFIG), RELEASE)
	include Release.mak
else ifeq ($(CONFIG), SECUREBOOT)
	include SecureBoot.mak
endif

CFLAGS += -I../.. \
    -I. \
    $(addprefix @,$(SYSCFG_OPT_FILES)) \
    -Oz \
    -munaligned-access \
    -fshort-enums \
    -fcommon \
    -funsigned-char \
    -mlittle-endian \
    -fdiagnostics-show-option \
    -ffunction-sections \
    -g \
    -fno-strict-aliasing \
    -flto \
    -DxWISUN_NCP_ENABLE \
    -DxFIXED_GTK_KEYS \
    -DWISUN_RCP_ENABLE \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/ti-rf-driver/ti-rf-driver" \
    -I$(CSMP_AGENT_LIB_INSTALL_DIR)/Vendors/TI/application \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/apps/common/include" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/common/nv" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_config/ws_router" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/include" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/csmpagent" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/csmpagent/tlvs" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/csmptlv" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/coap" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/csmpservice" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/csmpapi" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/lib" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/lib/protobuf-c" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/osal" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/osal/ti_simplelink_wisun" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/sample" \
    -include mbed_config.h \
    @$(CSMP_AGENT_LIB_INSTALL_DIR)/Vendors/TI/application/defines/router.opts \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/posix/ticlang" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/ti-rf-driver/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbednanostack2tirtos" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbednanostack2tirtos/platform" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbedtls2tirtos" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbedtls2tirtos/platform" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbedtls2tirtos/mbedtls/mbed-crypto" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/nanostack-libservice/mbed-client-libservice" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/nanostack-libservice" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/nanostack-libservice/mbed-client-libservice/platform" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/mbed-client-randlib/mbed-client-randlib/platform" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/mbed-client-randlib/mbed-client-randlib" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/mbed-client-randlib" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/mbed-trace" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/mbed-coap" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/mbed-coap/source/include" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/nanostack" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/mdns" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/mdns/fnet" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/mdns/fnet/fnet_stack" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/mdns/fnet/fnet_stack/stack" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/mdns/fnet/fnet_stack/services/serial" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Core/include" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/6LoWPAN" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack-eventloop" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack-eventloop/nanostack-event-loop" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack-eventloop/nanostack-event-loop/platform" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/nanostack-hal-mbed-cmsis-rtos" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/mbed-mesh-api/source/include" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/mbed-mesh-api/mbed-mesh-api" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/mbed-mesh-api" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/coap-service/coap-service" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/coap-service/source/include" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/ti_wisunfan/mbedtls" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/ti_wisunfan/mbedtls/platform/inc" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/ti_wisunfan/mbedtls/inc" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/ti_wisunfan/mbedtls/platform/COMPONENT_PSA_SRV_IMPL/COMPONENT_NSPE" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/rcp" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/services" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/hal/assert" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/hal/appasrt" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/hal/crypto" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/hal/platform" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/hal/rf" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/high_level" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/low_level" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/osal_port" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/boards" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/util" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/inc" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/heapmgr" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/services/saddr" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/services/sdata" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/fh" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/inc" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/rom" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/inc/cc13xx" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/low_level/cc13xx" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/tracer" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/mbed-mesh-api/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/6LoWPAN/MAC" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/6LoWPAN/Thread" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Security/PANA" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/mdns/fnet/fnet_stack/services/mdns" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/fnv_hash" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/hmac" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/load_balance" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/libNET/src" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack-eventloop/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/ti_wisunfan/mbedtls/platform/COMPONENT_PSA_SRV_IMPL" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/whiteboard" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/6LoWPAN/ws" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/DHCPv6_client" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/libDHCPv6" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/utils" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/pan_blacklist" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Common_Protocols" \
    -DEXCLUDE_TRACE \
    -DxSWITCH_NCP_TO_TRACE \
    -DTLS_SEC_PROT_LIB_USE_MBEDTLS_PLATFORM_MEMORY \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/config" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/include" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/src" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/src/core" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/src/core/utils" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/examples/platforms" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/examples/ncp_ftd" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/examples/ncp_ftd/platform" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/src/core/thread" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/src/ncp" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/examples/ncp_ftd/platform/nv" \
    -DxWISUN_AUTO_START \
    -DTI_WISUN_FAN_DEBUG \
    -DFEATURE_WISUN_MIN_EDFE \
    -DBoard_EXCLUDE_NVS_EXTERNAL_FLASH \
    -DDeviceFamily_CC13X2X7 \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/devices/cc13x2x7_cc26x2x7" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/devices/cc13x2x7_cc26x2x7/driverlib" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/devices/cc13x2x7_cc26x2x7" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/devices/cc13x2x7_cc26x2x7/inc" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/devices/cc13x2x7_cc26x2x7/driverlib" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/kernel/freertos" \
    -DFREERTOS_SUPPORT \
    -DFREERTOS \
    -DNVOCMP_POSIX_MUTEX \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/freertos/include" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/freertos/portable/GCC/ARM_CM4F" \
    -gdwarf-3 \
    -mcpu=cortex-m4 \
    -march=armv7e-m \
    -mthumb \
    -mfloat-abi=hard \
    -mfpu=fpv4-sp-d16

CPPFLAGS += -I../.. \
    -I. \
    $(addprefix @,$(SYSCFG_OPT_FILES)) \
    -Oz \
    -munaligned-access \
    -fshort-enums \
    -fcommon \
    -funsigned-char \
    -mlittle-endian \
    -fdiagnostics-show-option \
    -ffunction-sections \
    -g \
    -fno-strict-aliasing \
    -flto \
    -DxWISUN_NCP_ENABLE \
    -DxFIXED_GTK_KEYS \
    -DWISUN_RCP_ENABLE \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/ti-rf-driver/ti-rf-driver" \
    -I$(CSMP_AGENT_LIB_INSTALL_DIR)/Vendors/TI/application \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/apps/common/include" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/common/nv" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_config/ws_router" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/include" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/csmpagent" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/csmpagent/tlvs" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/csmptlv" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/coap" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/csmpservice" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/csmpapi" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/lib" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/src/lib/protobuf-c" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/osal" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/osal/ti_simplelink_wisun" \
    "-I$(CSMP_AGENT_LIB_INSTALL_DIR)/sample" \
    -include mbed_config.h \
    @$(CSMP_AGENT_LIB_INSTALL_DIR)/Vendors/TI/application/defines/router.opts \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/posix/ticlang" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/ti-rf-driver/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbednanostack2tirtos" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbednanostack2tirtos/platform" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbedtls2tirtos" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbedtls2tirtos/platform" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbedtls2tirtos/mbedtls/mbed-crypto" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/nanostack-libservice/mbed-client-libservice" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/nanostack-libservice" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/nanostack-libservice/mbed-client-libservice/platform" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/mbed-client-randlib/mbed-client-randlib/platform" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/mbed-client-randlib/mbed-client-randlib" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/mbed-client-randlib" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/mbed-trace" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/mbed-coap" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/frameworks/mbed-coap/source/include" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/nanostack" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/mdns" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/mdns/fnet" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/mdns/fnet/fnet_stack" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/mdns/fnet/fnet_stack/stack" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/mdns/fnet/fnet_stack/services/serial" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Core/include" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/6LoWPAN" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack-eventloop" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack-eventloop/nanostack-event-loop" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack-eventloop/nanostack-event-loop/platform" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/nanostack-hal-mbed-cmsis-rtos" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/mbed-mesh-api/source/include" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/mbed-mesh-api/mbed-mesh-api" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/mbed-mesh-api" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/coap-service/coap-service" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/coap-service/source/include" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/ti_wisunfan/mbedtls" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/ti_wisunfan/mbedtls/platform/inc" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/ti_wisunfan/mbedtls/inc" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/ti_wisunfan/mbedtls/platform/COMPONENT_PSA_SRV_IMPL/COMPONENT_NSPE" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/rcp" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/services" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/hal/assert" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/hal/appasrt" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/hal/crypto" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/hal/platform" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/hal/rf" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/high_level" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/low_level" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/osal_port" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/boards" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/util" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/inc" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/heapmgr" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/services/saddr" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/services/sdata" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/fh" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/inc" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/rom" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/inc/cc13xx" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/low_level/cc13xx" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/tracer" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/mbed-mesh-api/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/6LoWPAN/MAC" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/6LoWPAN/Thread" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Security/PANA" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/mdns/fnet/fnet_stack/services/mdns" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/fnv_hash" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/hmac" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/load_balance" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/libNET/src" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack-eventloop/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/ti_wisunfan/mbedtls/platform/COMPONENT_PSA_SRV_IMPL" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/whiteboard" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/6LoWPAN/ws" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/DHCPv6_client" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/libDHCPv6" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/utils" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Service_Libs/pan_blacklist" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/Common_Protocols" \
    -DEXCLUDE_TRACE \
    -DxSWITCH_NCP_TO_TRACE \
    -DTLS_SEC_PROT_LIB_USE_MBEDTLS_PLATFORM_MEMORY \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/config" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/include" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/src" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/src/core" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/src/core/utils" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/examples/platforms" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/examples/ncp_ftd" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/examples/ncp_ftd/platform" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/src/core/thread" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/src/ncp" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/examples/ncp_ftd/platform/nv" \
    -DxWISUN_AUTO_START \
    -DTI_WISUN_FAN_DEBUG \
    -DFEATURE_WISUN_MIN_EDFE \
    -DBoard_EXCLUDE_NVS_EXTERNAL_FLASH \
    -DDeviceFamily_CC13X2X7 \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/devices/cc13x2x7_cc26x2x7" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/devices/cc13x2x7_cc26x2x7/driverlib" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/devices/cc13x2x7_cc26x2x7" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/devices/cc13x2x7_cc26x2x7/inc" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/devices/cc13x2x7_cc26x2x7/driverlib" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/kernel/freertos" \
    -DFREERTOS_SUPPORT \
    -DFREERTOS \
    -DNVOCMP_POSIX_MUTEX \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/freertos/include" \
    "-I$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/freertos/portable/GCC/ARM_CM4F" \
    -gdwarf-3 \
    -mcpu=cortex-m4 \
    -march=armv7e-m \
    -mthumb \
    -mfloat-abi=hard \
    -mfpu=fpv4-sp-d16

LFLAGS += "$(CSMP_AGENT_LIB_INSTALL_DIR)/build/debug/csmp_agent_lib_ti_simplelink_wisun.a" \
    "-L$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source" \
    "-L$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/kernel" \
     -Wl,--diag_suppress=10063 \
    -Wl,--define=FREERTOS \
    ti_utils_build_linker.cmd.genlibs \
    ../../freertos/ticlang/cc13x2x7_cc26x2x7.cmd \
    "-Wl,-m,$(NAME).map" \
    -Wl,--rom_model \
    -Wl,--warn_sections \
    "-L$(TICLANG_ARMCOMPILER)/lib" \
    -llibc.a

# Note, the config-specific postbuild goal is defined in each {config}.mak file,
# which was included above based on the value of the CONFIG variable.
all: postbuild

.INTERMEDIATE: syscfg
$(SYSCFG_FILES): syscfg
	@ echo generation complete

syscfg: ../../freertos/csmp_example.syscfg
	@ echo Generating configuration files...
	@ $(SYSCFG_CMD_STUB) --output $(@D) $<


# Helpful hint that the user needs to use a standalone SysConfig installation
$(SYSCONFIG_GUI_TOOL):
	$(error $(dir $(SYSCONFIG_TOOL)) does not contain the GUI framework \
        necessary to launch the SysConfig GUI.  Please set SYSCONFIG_TOOL \
        (in your SDK's imports.mak) to a standalone SysConfig installation \
        rather than one inside CCS)

syscfg-gui: ../../freertos/csmp_example.syscfg $(SYSCONFIG_GUI_TOOL)
	@ echo Opening SysConfig GUI
	@ $(SYSCFG_GUI_CMD_STUB) $<

define C_RULE
$(basename $(notdir $(1))).obj: $(1) $(SYSCFG_H_FILES)
	@ echo Building $$@
	@ $(CC) $(CFLAGS) -c $$< -o $$@
endef

$(foreach c_file,$(SYSCFG_C_FILES),$(eval $(call C_RULE,$(c_file))))

application_main.obj: $(CSMP_AGENT_LIB_INSTALL_DIR)/Vendors/TI/application/main.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

application_application.obj: $(CSMP_AGENT_LIB_INSTALL_DIR)/Vendors/TI/application/application.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbed_ti-rf-driver_source_RFQueue.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/ti-rf-driver/source/RFQueue.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbed_ti-nv-driver_crc.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/common/nv/crc.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbed_ti-nv-driver_nvocmp.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/common/nv/nvocmp.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

csmp_sample_tlvs_ti_simplelink_wisun_tlvs.obj: $(CSMP_AGENT_LIB_INSTALL_DIR)/sample/tlvs/ti_simplelink_wisun_tlvs.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

csmp_osal_osal_ti_simplelink_wisun.obj: $(CSMP_AGENT_LIB_INSTALL_DIR)/osal/ti_simplelink_wisun/osal_ti_simplelink_wisun.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbed_mbednanostack2tirtos_nanostack_nanostack-hal-mbed-cmsis-rtos_arm_hal_interrupt.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbednanostack2tirtos/nanostack/nanostack-hal-mbed-cmsis-rtos/arm_hal_interrupt.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbed_mbednanostack2tirtos_nanostack_nanostack-hal-mbed-cmsis-rtos_arm_hal_random.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbednanostack2tirtos/nanostack/nanostack-hal-mbed-cmsis-rtos/arm_hal_random.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbed_mbednanostack2tirtos_nanostack_nanostack-hal-mbed-cmsis-rtos_arm_hal_timer.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbednanostack2tirtos/nanostack/nanostack-hal-mbed-cmsis-rtos/arm_hal_timer.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbed_mbednanostack2tirtos_nanostack_nanostack-hal-mbed-cmsis-rtos_ns_event_loop.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbednanostack2tirtos/nanostack/nanostack-hal-mbed-cmsis-rtos/ns_event_loop.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbed_mbednanostack2tirtos_nanostack_nanostack-hal-mbed-cmsis-rtos_ns_event_loop_mutex.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbednanostack2tirtos/nanostack/nanostack-hal-mbed-cmsis-rtos/ns_event_loop_mutex.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbed_mbednanostack2tirtos_nanostack_nanostack-hal-mbed-cmsis-rtos_ns_hal_init.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbednanostack2tirtos/nanostack/nanostack-hal-mbed-cmsis-rtos/ns_hal_init.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbed_mbednanostack2tirtos_nanostack_nanostack-hal-mbed-cmsis-rtos_arm_hal_fhss_timer.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbednanostack2tirtos/nanostack/nanostack-hal-mbed-cmsis-rtos/arm_hal_fhss_timer.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbed_mbednanostack2tirtos_platform_ns_trace.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/mbed_port/mbednanostack2tirtos/platform/ns_trace.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbed_nanostack_mbed-mesh-api_source_wisun_tasklet.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/mbed-mesh-api/source/wisun_tasklet.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

software_stack_ti15_4mac_mac_settings_mac_cfg.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/high_level/mac_cfg.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

software_stack_ti15_4mac_mac_settings_mac_settings.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/low_level/cc13xx/mac_settings.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

software_stack_ti15_4mac_radio_configuration_mac_user_config.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/boards/mac_user_config.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

software_stack_ti15_4mac_macTask.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/osal_port/macTask.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

software_stack_ti15_4mac_stack_user_api_api_mac_timac_fhss_config.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/osal_port/timac_fhss_config.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

software_stack_ti15_4mac_osal_port_osal_port.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/osal_port/osal_port_tirtos/osal_port.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

software_stack_ti15_4mac_osal_port_osal_port_timers.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/wisunfan_mac/common/osal_port/osal_port_tirtos/osal_port_timers.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbed_nanostack_mac_mac_header_helper_functions.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/MAC/IEEE802_15_4/mac_header_helper_functions.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mbed_nanostack_mac_sw_mac.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mbed/nanostack/sal-stack-nanostack/source/MAC/IEEE802_15_4/sw_mac.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

rcp_rcp_mcps_sap.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/rcp/rcp_mcps_sap.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

rcp_rcp_security_mib.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/rcp/rcp_security_mib.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

rcp_rcp_mlme.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/rcp/rcp_mlme.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

rcp_rcp_host.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/rcp/rcp_host.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

rcp_rcp_host_helpers.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/rcp/rcp_host_helpers.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

rcp_rcp_queue.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/rcp/rcp_queue.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

rcp_rcp_lmac.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/rcp/rcp_lmac.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

rcp_rcp_lmac_helpers.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/rcp/rcp_lmac_helpers.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

wisun_ncp_platform_uart.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/examples/ncp_ftd/platform/uart.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

wisun_ncp_platform_debug_uart.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/examples/ncp_ftd/platform/debug_uart.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

wisun_ncp_src_ncp_ncp_base_mtd.obj: $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/ti/ti_wisunfan/ncp_interface/src/ncp/ncp_base_mtd.cpp $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

$(NAME).out: $(OBJECTS)
	@ echo linking $@
	@ $(LNK) -Wl,-u,_c_int00 $(OBJECTS)  $(LFLAGS) -o $(NAME).out

$(NAME).hex: $(NAME).out
	@ echo building $@
	@ $(HEX) -O ihex $? $@

clean:
	@ echo Cleaning...
	@ $(RM) *.obj *.out *.hex *.map *.d > $(DEVNULL) 2>&1
	@ $(RM) ti_devices_config.c ti_radio_config.c ti_radio_config.h > $(DEVNULL) 2>&1
	@ $(RM) ti_drivers_config.c ti_drivers_config.h > $(DEVNULL) 2>&1
	@ $(RM) ti_wisunfan_config.h ti_wisunfan_features.h > $(DEVNULL) 2>&1
	@ $(RM) ti_utils_build_linker.cmd.genlibs ti_utils_build_linker.cmd.genmap > $(DEVNULL) 2>&1
	@ $(RM) ti_utils_build_compiler.opt FreeRTOSConfig.h > $(DEVNULL) 2>&1
	@ $(RM) ti_freertos_config.c ti_freertos_portable_config.c syscfg_c.rov.xs > $(DEVNULL) 2>&1
