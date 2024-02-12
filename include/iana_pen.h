/*
 *  Copyright 2023, 2024 Cisco Systems, Inc.
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

#ifndef _IANA_PEN_H
#define _IANA_PEN_H

/*! \file
 *
 * IANA Private Enterprise Numbers (PENs)
 *
 * PENs created and maintained by the Internet Assigned Numbers Authority (IANA)
 * are commonly used to identify private enterprises/orgranizations.
 * 
 * RFC 9371
 * URL: https://www.iana.org/assignments/enterprise-numbers/
 */

/** Set Vendor-ID to Vendor's IANA PEN-ID. Refer include/iana_pen.h */
#define VENDOR_ID CISCO

/** IANA PEN assignments */
enum {
  RESERVED = 0,
  CISCO = 5771,
  SILABS = 39873,
  ITRON = 1233,
  DUKE_ENERGY = 2981,
  FIRST_ENERGY = 23818,
  BC_HYDRO = 39480,
  LANDIS_GYR = 42830,
  TELLABS = 1397,
  EXEGIN = 39682
  //RENESAS
  //ROHM
};

#endif
