/*
 *  Copyright 2021 Cisco Systems, Inc.
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

#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <ifaddrs.h>
#include <unistd.h>

#include "csmp_service.h"
#include "csmp_info.h"
#include "signature_verify.h"
#include "trickle_timer.h"
#include "iana_pen.h"

#ifdef PRINTDEBUG
  #define DPRINTF printf
#else
  #define DPRINTF(format, ...)
#endif

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
/** \brief NMS ip address*/
#define NMS_IP "2001:a:b:c::ffaa"

/** \brief eui64, should come from the HW */
uint8_t g_eui64[8] = {0x0a, 0x00, 0x27, 0xff, 0xfe, 0x3b, 0x2a, 0xb2};

/** \brief Agent's IPv6 addr, can also be set via cli param -ip */
char g_ipv6[46]="1:1:1::202";

/************************ end *********************************************/

/** \brief  minium registation interval*/
#define reg_interval_min 10
/** \brief  maximum registation interval*/
#define reg_interval_max 20

/** \brief max number of interfaces*/
#define interface_max_num 2
/** \brief max number of ip addresses*/
#define ipaddress_max_num 3
/** \brief max number of neighbors*/
#define neighbor_max_num 2

#define nexthop_IP "fe80::a00:27ff:fe3b:2ab1"

#define DEVICE_TYPE OPENCSMP
enum {
  GENERIC_HW = 0,
  OPENCSMP = 1,
  CISCO_IR510 = 2,
  ITRON_METER = 3
};
// Sample running slot csmp slot hdr which is used by FND to identify running fw on the node.
#define CSMP_SLOTHDR_RUN_IMAGE {{0x61,0xe7,0xe1,0x76,0xe2,0xfb,0xcc,0x3e,0x1c,0xc8,0x5b,0xb1,0xf4,\
0x99,0xa4,0x02,0x6d,0x28,0xcf,0x1d,0x66,0x16,0x76,0x91,0x91,0x3f,0xd9,0x80,0x5b,0xe5,0x5b,0xa1},\
"opencsmp-node","6.2.99", "OPENCSMP", 27904, 0, 0, 0, 0, 0, 0, {0},0, 0, {0}}

char *SSID = "CISCO";
char vendorhwid[32] = "Vendor Hardware-ID";
uint32_t g_init_time;
uint8_t neighbor_eui64[2][8] = {{0x0a, 0x00, 0x27, 0xff, 0xfe, 0x3b, 0x2a, 0xb1},
                             {0x0a, 0x00, 0x27, 0xff, 0xfe, 0x3b, 0x2a, 0xb0}};
dev_config_t g_devconfig;
csmp_handle_t g_csmp_handle;
bool g_reboot_request = false; //Flag to track ongoing reboot request
// Firmware upgrade globals
bool g_downloadbusy       = false;  // Track ongoing download
bool g_initxfer           = false;  // Track transfer request
bool g_initload           = false;  // Track load request
uint32_t g_curloadtime    = 0;      // Track current loadtime
uint32_t g_curloadslot    = 0xFFU;  // Track current load slot
uint32_t g_curbackupslot  = 0xFFU;  // Track current backup slot

// Firmware image slots (Slot-id: 0-RUN, 1-UPLOAD, 2-BACKUP)
Csmp_Slothdr g_slothdr[CSMP_FWMGMT_ACTIVE_SLOTS] = {0};

/* public key */
//new key
static const char pubkey[PUBLIC_KEY_LEN] = {
  0x04, 0x23, 0xD2, 0x83, 0x45, 0xE8, 0xD5, 0xDF, 0x86, 0x9D,
  0x6E, 0xE7, 0x58, 0x0D, 0xC1, 0x8F, 0x35, 0x9D, 0x57, 0xB1,
  0x3D, 0x50, 0x4A, 0x16, 0x01, 0x15, 0xC4, 0x81, 0x19, 0xB0,
  0xE6, 0x60, 0xB8, 0x64, 0x14, 0x01, 0x5D, 0x56, 0x83, 0xBE,
  0xE1, 0x85, 0x98, 0xCB, 0x90, 0xE1, 0xF7, 0x9B, 0xF4, 0x33,
  0x5A, 0x4B, 0x29, 0xAD, 0x35, 0x69, 0x9B, 0x4F, 0xDC, 0x42,
  0x7F, 0xEB, 0xC2, 0x99, 0xA5
};


/** \brief the hardware information */
Hardware_Desc g_hardwareDesc = HARDWARE_DESC_INIT;

/** \brief the interface information */
Interface_Desc g_interfaceDesc[interface_max_num] = {INTERFACE_DESC_INIT};

/** \brief the ipaddress information */
IP_Address g_ipAddress[ipaddress_max_num] = {IPADDRESS_INIT};

/** \brief the ip route information */
IP_Route g_ipRoute = IPROUTE_INIT;

/** \brief the current information */
Current_Time g_currentTime = CURRENT_TIME_INIT;

/** \brief the up time information */
Up_Time g_upTime = UPTIME_INIT;

/** \brief the interface metrics data */
Interface_Metrics g_interfaceMetrics[interface_max_num] = {INTERFACE_METRICS_INIT};

/** \brief the ip route rpl metrics data */
IPRoute_RPLMetrics g_iprouteRplmetrics = IPROUTE_RPLMETRICS_INIT;

/** \brief the wpan status data */
WPAN_Status g_wpanStatus = WPANSTATUS_INIT;

/** \brief the rpl data */
RPL_Instance g_rplInstance = RPLINSTANCE_INIT;

/** \brief the transfer requst data */
Transfer_Request g_transferRequest = TRANSFER_REQUEST_INIT;

/** \brief the image block data */
Image_Block g_imageBlock = IMAGE_BLOCK_INIT;

/** \brief the load request data */
Load_Request g_loadRequest = LOAD_REQUEST_INIT;

/** \brief the cancel load request data */
Cancel_Load_Request g_cancelLoadRequest = CANCEL_LOAD_REQUEST_INIT;

/** \brief the set backup request data */
Set_Backup_Request setBackupRequest = SET_BACKUP_REQUEST_INIT;

/** \brief the firmware info data */
Firmware_Image_Info g_firmwareImageInfo[CSMP_FWMGMT_ACTIVE_SLOTS] = {FIRMWARE_IMAGE_INFO_INIT};

/** \brief the signature settings data */
Signature_Settings g_SignatureSettings = SIGNATURE_SETTINGS_INIT;

/** \brief Max number of vendor subtypes */
#define VENDOR_MAX_SUBTYPES 5

/** \brief Vendor tlv data */
Vendor_Tlv g_vendorTlv[VENDOR_MAX_SUBTYPES] = {VENDOR_TLV_INIT};

int write_fw_img(uint8_t slotid);

#endif
