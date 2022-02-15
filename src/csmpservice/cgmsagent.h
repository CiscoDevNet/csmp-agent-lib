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

#ifndef _CGMSAGENT_H
#define _CGMSAGENT_H

/*! \file
 *
 * CGMS Agent
 *
 */

#include <netinet/in.h>

/**
 * @brief
 *
 * @param NMSaddr address of the NMS
 * @param update update the address
 * @return true
 * @return false
 */
bool register_start(struct in6_addr *NMSaddr, bool update);

/**
 * @brief reset timer
 *
 */
void reset_rpttimer();

/**
 * @brief stop the agent
 *
 * @return true
 * @return false
 */
bool cgmsagent_stop();

#endif
