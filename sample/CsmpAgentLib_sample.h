/*
 *  Copyright 2021-2025 Cisco Systems, Inc.
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

#ifndef _SAMPLE_H
#define _SAMPLE_H

#include "csmp_info.h"
#include "osal.h"

/*! \file
 *
 * CSMP global variable definitions
 *
 * including assigning the initial values.

 * \mainpage
 *
 * \section intro_sec Introduction
 * CSMP Device side library.
 *
 * The CSMP Device Library exist of the CSMP Server and the CSMP Agent
 *
 * - The CSMP Server acts on requests of the Network Management System (NMS) 
 * - The CSMP Agent sends requests to the NMS
 *
 * The typical flow initiated by the Agent:
 * 1. Register with NMS
 * 2. Provide Device Capabilities to NMS
 * 3. Obtain configuration from NMS
 * 4. Periodically report performance metrics to the NMS.
 *
 * The CSMP server (after registration)
 * 1. responds on requests of the NMS which can be:
 *      - Respond to subsequent configuration readsand updates.
 *      - Respond to on-demand performance metric requests from the NMS
 *      - Report asynchronousevents to the NMS as they occur.
 *      - Respond to logged event requests from the NMS.
 *      - Act on command requests from the NMS, including firmware transfer and activation.
 *
 *
 * \section install_sec Configuration
 * The configuration of the example is done via the csmp_info.h file.
 * This file contains the global variables that match the device responses on NMS requests.
 * These values are fixed in the example.
 * Real implementations needs to make sure that the values reported are actual values from the CSMP device.
 *
 *
 * \section struct_sec Code Structure
 * The code is build up:
 * \verbatim
   -----------------------------    ----
  |       Example Application   |     |
  +--------------+--------------+     |
  | CSMP Server  |  CSMP Agent  |     |
  +--------------+--------------+     |  C code
  |   Server     |   Client     |     |
  +-----------------------------+     |
  |            CoAP             |     |
  +-----------------------------+   ----
  |  pthread, semaphores        |     Linux
  -------------------------------
  \endverbatim
 *
 * The CSMP server and Agent are using global variables (structs).
 * The global variables are encoded/decoded to the protobuf TLV.
 * The TLVs are used in the request and response payloads.
 *
 * The example application has a callback handler csmptlvs_get() for the received GET CoAP Method call.
 * The example application has a callback handler csmptlvs_post() for the received POST CoAP method call.
 * Each handler function will call according to the supplied TLV the appropriate GET/POST TLV function.
 * This function sets the appropriate global variable on the wanted value, e.g. this call can be extended to read/write from the actual hardware.
 * The global variable is then passed down the stack and will be converted in to the protobuf TLV before passing it into the CoAP stack.
 *
 * \section struct_endpoints End points in the system
 *
 * NMS endpoints, e.g. where the CSMP Agent interacts with:
 * - <base-url>/r - registration
 * - <base-url>/c - configuration
 *
 * Device configuration endpoints, e.g. where the NMS Client interacts with:
 * - <base-url>/c
 * - <base-url>/c/<tlvid>
 * - <base-url>/c/<tlvid>/<index>
 */

/************************ start ********************************************/
/*  modify the information before compile */

/** \brief Set the device type */
enum {
  GENERIC_HW = 0,
  OPENCSMP = 1,
  CISCO_IR510 = 2,
  ITRON_METER = 3
};
#define DEVICE_TYPE OPENCSMP

#define CSMP_TASK_STACK_SIZE 2*1024 //Stack size of 2KB allocated to main thread to handle general/file operations to prevent SEGFAULT

/** \brief Min Registration inetval in seconds*/
#define CSMP_AGENT_REG_INTERVAL_MIN   10U

/** \brief Max Registration inetval in seconds*/
#define CSMP_AGENT_REG_INTERVAL_MAX   100U

/** \brief EUI64 Address of the Agent*/
#define CSMP_AGENT_EUI64_ADDRESS      "00173bab00100302"

/** \brief NMS Address*/
#define CSMP_AGENT_NMS_ADDRESS        "fd12:3456::e119:7a97:d047:fe1a"

/** \brief Enable/Disable Signature Settings*/
// #define CSMP_AGENT_SIGNATURE_SETTINGS 1
/************************ end *********************************************/

/** \brief eui64, should come from the HW*/
extern uint8_t g_eui64[8];

/** \brief Agent IPv6 address*/
extern char g_ipv6[46];

/** \brief  minium registation interval*/
#define reg_interval_min 10
/** \brief  maximum registation interval*/
#define reg_interval_max 20

/** \brief max number of interfaces*/
#define interface_max_num 2
/** \brief max number of ip addresses*/
#define ipaddress_max_num 3
/** \brief max number of neighbor*/
#define neighbor_max_num 2

/** \brief the hardware information */
extern Hardware_Desc g_hardwareDesc;

/** \brief the interface information */
extern Interface_Desc g_interfaceDesc[interface_max_num];

/** \brief the ipaddress information */
extern IP_Address g_ipAddress[ipaddress_max_num];

/** \brief the ip route information */
extern IP_Route g_ipRoute;

/** \brief the current information */
extern Current_Time g_currentTime;

/** \brief the up time information */
extern Up_Time g_upTime;

/** \brief the interface metrics data */
extern Interface_Metrics g_interfaceMetrics[interface_max_num];

/** \brief the ip route rpl metrics data */
extern IPRoute_RPLMetrics g_iprouteRplmetrics;

/** \brief the wpan status data */
extern WPAN_Status g_wpanStatus;

/** \brief the rpl data */
extern RPL_Instance g_rplInstance;

/** \brief the transfer requst data */
extern Transfer_Request g_transferRequest;

/** \brief the image block data */
extern Image_Block g_imageBlock;

/** \brief the load request data */
extern Load_Request g_loadRequest;

/** \brief the cancel load request data */
extern Cancel_Load_Request g_cancelLoadRequest;

/** \brief the set backup request data */
extern Set_Backup_Request setBackupRequest;

/** \brief the firmware info data */
extern Firmware_Image_Info g_firmwareImageInfo[CSMP_FWMGMT_ACTIVE_SLOTS];

/** \brief the signature settings data */
extern Signature_Settings g_SignatureSettings;

/** \brief Vendor tlv data */
extern Vendor_Tlv g_vendorTlv[VENDOR_MAX_SUBTYPES];

/** \brief Csmp Slot Header Array for the 3 FW slots */
extern osal_csmp_slothdr_t g_slothdr[CSMP_FWMGMT_ACTIVE_SLOTS];

/** \brief Flag to track ongoing reboot request */
extern bool g_reboot_request;

/**
 * @brief Initialize sample data before CSMP service start
 *
 * @param void
 * @return void
 */
extern void sample_data_init();

/**
 * @brief Initialize sample data before CSMP service start
 *
 * @param void
 * @return void
 */
extern void sample_app_reboot();

int str2addr(char *str, uint8_t *addr);

#endif
