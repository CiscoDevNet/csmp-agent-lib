# Renesas Wi-SUN FAN CSMP Agent Integration

<p align="left">
  <img src="resources/renesas-logo.jpg" alt="Renesas logo" width="220">
</p>

## Summary

The integration targets Renesas Wi-SUN FAN products based on the Renesas RX and RA MCU families.
Renesas provides Wi-SUN FAN-compliant Sub-GHz solutions for Wi-SUN FAN applications.
For the list of officially supported devices, stack versions, configuration options, and available features, please refer to the Renesas Wi-SUN FAN product and software documentation.

The purpose of this integration is to enable a Renesas Wi-SUN FAN node to communicate with Cisco Field Network Director (FND) using CSMP.
It provides platform-specific functionality required by the CSMP Agent Library, including:

- UDP/IPv6 communication over the Renesas Wi-SUN FAN protocol stack,
- access to platform, stack, and firmware information,
- mapping of Wi-SUN FAN network and firmware information to CSMP TLV objects,
- firmware update integration.

## Requirements

The following components are required to build and run the Renesas Wi-SUN FAN CSMP Agent integration:

- A running instance of Cisco Field Network Director (FND). For a development setup, please refer to the [CSMP Developer Tutorial](../../docs/CSMP%20Developer%20Tutorial%20-%200v11.pdf).

- At least two [Renesas Wi-SUN FAN Development Kits](https://www.renesas.com/en/design-resources/boards-kits/rtk0ee0013d10002bj).

- The latest Renesas [Wi-SUN FAN Protocol Stack](https://www.renesas.com/en/software-tool/sub-ghzwi-sun-protocol-stack). The proprietary Renesas Protocol Stack release already includes the CSMP integration for the Renesas Wi-SUN FAN sample application.

- The proprietary Renesas CC-RX compiler toolchain for RX MCUs.

The Renesas integration is intended to be built with the CC-RX toolchain for RX MCUs and with GCC ARM Embedded for RA MCUs. For both RX and RA MCU families, corresponding project configurations are delivered with the Renesas Wi-SUN FAN Protocol Stack.

## Renesas Wi-SUN FAN Stack

This Renesas CSMP integration layer is designed to be used in conjunction with the latest version of the [Renesas Wi-SUN FAN Protocol Stack](https://www.renesas.com/en/software-tool/sub-ghzwi-sun-protocol-stack).
Different versions may not work, so please make sure that this CSMP integration layer is used with the correct version of the Renesas Wi-SUN FAN Protocol Stack!

Further information about the CSMP usage with the Renesas Wi-SUN FAN protocol stack, APIs, configuration, sample applications, and supported platforms is provided in the user manual of the Renesas Wi-SUN FAN Protocol Stack and accompanying documentation, which are located in the release package of the Renesas Wi-SUN FAN Protocol Stack.
It also includes instructions for configuring and running a sample setup for CSMP with Renesas Wi-SUN FAN nodes.

A project configuration including the CSMP integration is provided in the Renesas Wi-SUN FAN Protocol Stack release package.

## Renesas Library Usage

The Renesas integration uses the following functions from the Renesas Protocol Stack. Further details are provided by the Renesas Wi-SUN FAN Protocol Stack user manual.

| Function or macro | Used by | Purpose                                                                                                                                                       |
| --- | --- |---------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `R_UDP_DataRequest` | `osal_sendto` | Sends CSMP UDP/IPv6 payloads through the Renesas Wi-SUN FAN network stack.                                                                                    |
| `R_Timer_StartSecs` | `osal_trickle_timer_start`, `R_TrickleTimer_HandleExpired` | Starts or restarts the Renesas timer used by CSMP trickle timers.                                                                                             |
| `R_Timer_Stop` | `osal_trickle_timer_stop` | Stops a Renesas timer backing a CSMP trickle timer.                                                                                                           |
| `R_CLOCK_GetUnixTimestamp` | `osal_gettime` | Reads the current Unix timestamp from the Renesas clock service.                                                                                              |
| `R_CLOCK_SetUnixTimestampReference` | `osal_settime`, `currenttime_post` | Updates the Renesas clock reference from a CSMP Current Time TLV.                                                                                             |
| `R_IPv6_IsUnspecified` | `R_CSMP_Start` | Rejects an unspecified FND IPv6 address before starting CSMP.                                                                                                 |
| `R_IPv6_IsMulticast` | `R_CSMP_Start` | Rejects a multicast FND IPv6 address before starting CSMP.                                                                                                    |
| `R_NWK_GetRequest` | `ipaddress_get`, `iproute_get`, `interface_metrics_get`, `iproute_rplmetrics_get`, `wpanstatus_get`, `rplinstance_get` | Reads Wi-SUN FAN stack attributes such as IPv6 addresses, parent addresses, PHY data rate, interface counters, PAN ID, RPL data, PAN size, and transmit power. |
| `R_NWK_GetRequestMultipart` | `fillIpRouteRplMetric`, `wpanstatus_get` | Iterates through multipart Renesas network data, especially the neighbor cache used for RPL metrics.                                                          |
| `R_APP_GetAppName` | `hardware_desc_get` | Supplies the running application name for the CSMP hardware/software description.                                                                             |
| `R_APP_GetHardwareName` | `hardware_desc_get` | Supplies the Renesas hardware or board name for the CSMP model description.                                                                                   |
| `R_APP_GetNetworkName` | `wpanstatus_get` | Supplies the Wi-SUN FAN network name for the CSMP WPAN status SSID field.                                                                                     |
| `AppIpGetMacAddr` | `hardware_desc_get`, `interface_desc_get`, `ipaddress_get` | Supplies the local EUI-64/MAC address used for serial number, OUI, interface address, and link-local IPv6 address fields.                                     |
| `R_APP_FWUP_TransferRequest` | `transferRequest_post` | Initializes a firmware transfer in the Renesas firmware update module.                                                                                        |
| `R_APP_FWUP_BlockRequest` | `imageBlock_post` | Writes a firmware image block into the Renesas firmware update upload area.                                                                                   |
| `R_APP_FWUP_LoadRequest` | `loadRequest_post` | Schedules the uploaded firmware image for loading at the requested time.                                                                                      |
| `R_APP_FWUP_CancelRequest` | `cancelLoadRequest_post` | Cancels a pending firmware load request.                                                                                                                      |
| `R_FWUP_SoftwareReset` | `rebootRequest_post` | Performs a software reset when CSMP requests a reboot.                                                                                                        |
| `R_Flash_Read` | `initialize_file_hash`, `initialize_file_name`, `initialize_file_size` | Reads firmware metadata stored in Renesas data flash.                                                                                                         |
| `r_fwup_wrap_flash_read` | `initialize_file_hash` | Reads the running firmware header from the Renesas firmware update flash wrapper when no stored firmware hash is available.                                   |
| `R_memcpy` | OSAL startup and TLV helpers | Copies Renesas address, EUI-64, firmware metadata, and TLV data using Renesas memory utilities.                                                               |
| `MEMZERO_S`, `MEMZERO_A`, `MEMCPY_S`, `MEMCPY_A`, `MEMISZERO_A`, `MEMISNOTZERO_S` | TLV helper functions | Clears/copies/tests TLV structures and Renesas values with the Renesas memory helper macros.                                                                  |
| `my_alloc`, `my_free` | `osal_malloc`, `osal_free` | Allocates/frees CSMP memory from the heap.                                                                                                                    |
| `clock_seconds` | Trickle timer logic, uptime, and metrics | Reads the Renesas uptime counter used by CSMP timers and uptime-related TLVs.                                                                                 |
| `random32` | Trickle timer logic | Generates randomized trickle timer offsets.                                                                                                                   |
| `R_LOG_DBG`, `R_LOG_WARN`, `R_LOG_ERR` | OSAL and TLV helper functions | Emits Renesas logging messages for CSMP integration diagnostics.                                                                                              |

## CC-RX Compiler Notes

The Renesas Wi-SUN FAN integration is intended to be built with the Renesas C/C++ Compiler Package for RX MCUs, also known as CC-RX. 

The integration keeps a small number of explicit casts for CC-RX compatibility.
The following casts are intentional:

- `ufield->wire_type = (ProtobufCWireType) scanned_member->wire_type;`

  CC-RX reports `Enumerated type mixed with another type`

- `xfer_req->status != (uint32_t) FWHDR_STATUS_DOWNLOAD`

  CC-RX reports `Integer conversion resulted in a change of sign`

These casts are the expected type conversion for the Renesas CC-RX compiler and keep the Renesas build warning-clean.

Reference: [Renesas C/C++ Compiler Package for RX Family [CC-RX]](https://www.renesas.com/en/software-tool/c-cplusplus-compiler-package-rx-family-cc-rx)

## Relevant Renesas resources

- [Renesas Sub-GHz/Wi-SUN FAN Transceivers](https://www.renesas.com/en/products/wireless-connectivity/sub-ghz-Wi-SUN-transceivers)

- [Renesas Sub-GHz/Wi-SUN FAN Protocol Stack](https://www.renesas.com/en/software-tool/sub-ghzWi-SUN-protocol-stack)
