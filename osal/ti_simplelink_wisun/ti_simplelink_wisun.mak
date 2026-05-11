####################################################################
# TI SimpleLink SDK include paths.
#
# subst converts Windows backslashes to forward slashes automatically.
####################################################################

SDK := $(subst \,/,$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR))

# -----------------------------------------------------------------------
# Target device family — change to match your chip.
# Supported values:
#   CC13X4  — CC1352P7, CC1314R10  (Cortex-M33)  [default]
#   CC13X2X7 — CC1352P7 x7 variant (Cortex-M4F)
#   CC13X2  — CC1352P, CC1352R     (Cortex-M4F)
#   CC13X1  — CC1311P3, CC1311R3   (Cortex-M4F)
# -----------------------------------------------------------------------
DEVICE_FAMILY ?= CC13X4

ifeq ($(DEVICE_FAMILY),CC13X4)
  C_DEFS        += -DDeviceFamily_CC13X4
  DEVICE_DIR     = cc13x4_cc26x4
  FREERTOS_PORT  = ARM_CM33/non_secure
  FREERTOS_CFG   = cc13x4_cc26x4
else ifeq ($(DEVICE_FAMILY),CC13X2X7)
  C_DEFS        += -DDeviceFamily_CC13X2X7
  DEVICE_DIR     = cc13x2x7_cc26x2x7
  FREERTOS_PORT  = ARM_CM4F
  FREERTOS_CFG   = cc13x2x7_cc26x2x7
else ifeq ($(DEVICE_FAMILY),CC13X2)
  C_DEFS        += -DDeviceFamily_CC13X2
  DEVICE_DIR     = cc13x2_cc26x2
  FREERTOS_PORT  = ARM_CM4F
  FREERTOS_CFG   = cc13x2_cc26x2
else ifeq ($(DEVICE_FAMILY),CC13X1)
  C_DEFS        += -DDeviceFamily_CC13X1
  DEVICE_DIR     = cc13x1_cc26x1
  FREERTOS_PORT  = ARM_CM4F
  FREERTOS_CFG   = cc13x1_cc26x1
else
  $(error Unsupported DEVICE_FAMILY: $(DEVICE_FAMILY). Use CC13X4, CC13X2X7, CC13X2, or CC13X1)
endif

# driverlib device-specific headers (resolved per DEVICE_FAMILY above)
INCLUDES += -I$(SDK)/source/ti/devices/$(DEVICE_DIR)
INCLUDES += -I$(SDK)/source/ti/devices/$(DEVICE_DIR)/driverlib

INCLUDES += -I$(SDK)/source/ti/posix/ticlang

# FreeRTOS kernel, portable layer and config (resolved per DEVICE_FAMILY above)
INCLUDES += -I$(SDK)/source/third_party/freertos/include
INCLUDES += -I$(SDK)/source/third_party/freertos/portable/GCC/$(FREERTOS_PORT)
INCLUDES += -I$(SDK)/kernel/freertos/builds/.meta/$(FREERTOS_CFG)/release/pregenerated_configuration

# SDK source root — for <ti/drivers/dpl/ClockP.h>
INCLUDES += -I$(SDK)/source

# Nanostack public API — socket_api.h, ns_address.h, net_interface.h, ip6string.h
INCLUDES += \
  -I$(SDK)/source/third_party/mbed/nanostack/sal-stack-nanostack/nanostack \
  -I$(SDK)/source/third_party/mbed/frameworks/nanostack-libservice/mbed-client-libservice \
  -I$(SDK)/source/third_party/mbed/nanostack/sal-stack-nanostack-eventloop \
  -I$(SDK)/source/third_party/mbed/nanostack/mbed-mesh-api/source/include

# mbedTLS — sha256.h, ecdsa.h used in OTA and signature verify
INCLUDES += \
  -I$(SDK)/source/third_party/ti_wisunfan/mbedtls/inc \
  -I$(SDK)/source/third_party/ti_wisunfan/mbedtls/platform/inc \
  -I$(SDK)/source/ti/ti_wisunfan/mbed_port/mbedtls2tirtos

# NVOCMP NV driver — nvocmp.h and nvintf.h 
INCLUDES += -I$(SDK)/source/ti/common/nv