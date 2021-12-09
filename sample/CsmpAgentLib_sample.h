/*
 * Cisco CSMP Reference Implementation Source Code License Agreement v1.0
 *
 * These terms govern your use of Cisco Systems, Inc.’s CSMP Reference
 * Implementation Source Code and associated documentation (“Licensed
 * Materials”). By accessing, downloading, copying, distributing, calling, or
 * otherwise making any use of the Licensed Materials, you accept and agree to
 * be bound by the following terms and conditions (the “License”). If you are
 * accepting the License on behalf of an entity, you represent that you have the
 * authority to do so (either you or the entity, “you”). All use of the Licensed
 * Materials must be in accordance with this License and all rights not
 * expressly granted herein are reserved.

 * 1.	LICENSE GRANT.
 * Subject to Your compliance with the terms of this License, Cisco grants you
 * a limited, non-exclusive, perpetual, revocable, non-sublicensable,
 * royalty-free, worldwide, and non-transferable license to: (a) use the
 * Licensed Materials for the purpose of internal development, testing,
 * maintenance and support of your products to interoperate with Cisco 802.15.4
 * border routers -- including without limitation the  Cisco 1000 Series
 * Connected Grid Routers – and Cisco IoT Field Network Director and any
 * successor products (collectively the “Cisco Products”), and for no other
 * purpose; (b) use and reproduce a reasonable number of copies of the Licensed
 * Materials solely for the internal development, testing, back-up, maintenance
 * and support of your products as permitted under this License; (c) make
 * corrections and bug fixes to the Licensed Materials only if such corrections
 * and bug fixes do not interfere with or inhibit the interoperability of your
 * products with the Cisco Products; and (d) distribute the CSMP Reference
 * Implementation Source Code solely in compiled, binary format with your
 * products to your customers solely to enable your products to interoperate
 * with the Cisco Products.

 * 2.	CONDITIONS.
 * You shall not use the Licensed Materials to: (a) substantially replicate and
 * compete with the Cisco Products; (b) collect, store, or transmit confidential
 * , sensitive, or protected data or information without authorization; (c)
 * disable, disrupt, circumvent, interfere with, or otherwise violate the
 * security of any Cisco product, service, or user;  (d) access any application,
 * system, service, computer, data, account, or network without authorization;
 * (e) develop features or functions in stand-alone applications that do not
 * interoperate with the Cisco Products;  or (f) sell or offer to sell the
 * Licensed Products on a standalone basis. The Cisco Products are licensed
 * under their own separate terms and you shall not use the Licensed Materials
 * in any way that violates or is inconsistent with those terms (for more
 * information, please visit: www.cisco.com/go/terms). Failure to comply with
 * any part of this section or section 2 constitutes a material breach of this
 * License, terminates the license granted in Section 1 above, and may result in
 * further repercussions and actions against you, at Cisco’s sole discretion.

 * 3.	OWNERSHIP.
 * The Licensed Materials, in whole or in part, in all forms, and any
 * corrections or bug fixes thereto that you make are the sole and exclusive
 * property of Cisco.   You irrevocably transfer, convey, and assign to Cisco
 * all of your right, title, and interest in any corrections or bug fixes you
 * make to the Licensed Materials.  You shall send the corrections or bug fixes
 * to ask-crmesh-dev@cisco.com.  This License does not grant you permission to
 * use any trade names, trademarks, service marks, or product names of Cisco.
 * If you provide any feedback to Cisco regarding the Licensed Materials, you
 * agree that Cisco, its suppliers, and its customers shall be free to use and
 * incorporate such feedback into the Licensed Materials and Cisco products and
 * services without restriction, payment, or additional consideration of any
 * kind. If you initiate or participate in any litigation against Cisco, its
 * partners, or its customers (including cross-claims and counter-claims)
 * alleging that the Licensed Materials or its use infringe any patent,
 * copyright, or other intellectual property right, then all rights granted to
 * you under this License shall terminate immediately.

 * 4.	CONFIDENTIAL INFORMATION.
 * The Licensed Materials, whether in whole or in part, contain confidential
 * information and proprietary property of Cisco.  Licensee acknowledges that
 * the Licensed Materials contain trade secrets and other confidential
 * information which are the property of Cisco.  You shall keep the Licensed
 * Materials in strict confidence and take all reasonable steps to prevent: (a)
 * unauthorized persons from having access to the Licensed Materials; (b)
 * disclosure of the Licensed Materials to any third party; and (c) unauthorized
 * copying of the Licensed Materials.

 * 5.	PROPRIETARY NOTICES.
 * You agree to maintain and reproduce all copyright, trademarks and other
 * proprietary notices on all copies of the Licensed Materials, in any form, in
 * the same form and manner that such copyright, trademarks, and other
 * proprietary notices are included on the Licensed Materials.

 * 6.	NO SUPPORT.
 * Nothing in this License shall require Cisco to provide any support, services,
 * updates, upgrades, bug fixes or any other modifications to the Licensed
 * Materials.  You shall be solely responsible for providing support to your
 * customers for any problems or issues that they encounter by using the CSMP
 * Reference Implementation Source Code in binary form that you distributed.
 * Cisco may, at its own sole discretion, update, extend, enhance, or otherwise
 * modify the Licensed Materials without notice.  If Cisco provides updates to
 * you and no separate agreement accompanies such updates, then such updates are
 * subject to this License.

 * 7.	LIMITATION OF LIABILITY.
 * CISCO SHALL HAVE NO LIABILITY IN CONNECTION WITH OR RELATING TO THIS LICENSE
 * OR USE OF THE LICENSED MATERIALS, FOR DAMAGES OF ANY KIND, INCLUDING BUT NOT
 * LIMITED TO DIRECT, INCIDENTAL, AND CONSEQUENTIAL DAMAGES, OR FOR ANY LOSS OF
 * USE, DATA, INFORMATION, PROFITS, BUSINESS, OR GOODWILL, HOWEVER CAUSED, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

 * 8.	DISCLAIMER OF WARRANTY.
 * CISCO PROVIDES THE LICENSED MATERIALS “AS IS” WITH ALL FAULTS, WITHOUT
 * WARRANTY OF ANY KIND. TO THE MAXIMUM EXTENT PERMITTED BY LAW, ALL EXPRESS AND
 * IMPLIED CONDITIONS, REPRESENTATIONS, AND WARRANTIES INCLUDING, WITHOUT
 * LIMITATION, ANY IMPLIED WARRANTY OR CONDITION OF MERCHANTABILITY, FITNESS FOR
 * A PARTICULAR PURPOSE, NON-INFRINGEMENT, SATISFACTORY QUALITY,
 * NON-INTERFERENCE, AND ACCURACY, ARE HEREBY EXCLUDED AND EXPRESSLY DISCLAIMED
 * BY CISCO. CISCO DOES NOT WARRANT THAT THE LICENSED MATERIALS ARE SUITABLE FOR
 * YOUR USE, WILL OPERATE PROPERLY WITH YOUR APPLICATION, ARE ACCURATE OR
 * COMPLETE, OR ARE WITHOUT ERROR OR DEFECT.

 * 9.	GENERAL.
 * This License shall be governed by and interpreted in accordance with the laws
 * of the State of California, excluding its conflict of law’s provisions. You
 * agree to comply with all applicable United States export laws, rules, and
 * regulations. If any provision of this License is judged illegal, invalid, or
 * otherwise unenforceable, that provision shall be severed, and the rest of the
 * License shall remain in full force and effect. No failure by Cisco to enforce
 * any of its rights related to the Licensed Materials or to a breach of this
 * License in a particular situation will act as a waiver of such rights. In the
 * event of any inconsistencies with any other terms that may apply to the
 * Licensed Materials, this License shall take precedence.
 */

#ifndef _SAMPLE_H
#define _SAMPLE_H

#include "csmp_info.h"


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

/** \brief eui64, should come from the HW*/
uint8_t g_eui64[8] = {0x0a, 0x00, 0x27, 0xff, 0xfe, 0x3b, 0x2a, 0xb2};
/************************ end *********************************************/

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

/** \brief the firmware info data */
Firmware_Image_Info g_firmwareImageInfo = FIRMWARE_IMAGE_INFO_INIT;

#endif
