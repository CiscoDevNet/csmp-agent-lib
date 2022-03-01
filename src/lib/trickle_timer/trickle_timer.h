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

#ifndef _TRICKLETIMER_H
#define _TRICKLETIMER_H

/*! \file
 *
 * Timer functions
 */

/** timer types
 *
 */
typedef enum {
 reg_timer = 0,  /**< register timer */
 rpt_timer = 1,  /**< reporting timer */
 timer_num = 2   /**< max amount of timers */
}timerid_t;

/**
 * @brief callback function prototype
 *
 */
typedef void (*trickle_timer_fired_t) ();

/**
 * @brief start the timer
 *
 * @param timerid timer type
 * @param imin minimum of the timer interval
 * @param imax maximum of the timer interval
 * @param trickle_time_fired callback
 */
void trickle_timer_start(timerid_t timerid, uint32_t imin, uint32_t imax, trickle_timer_fired_t trickle_time_fired);

/**
 * @brief stop the timer
 *
 * @param timerid the timer id
 */
void trickle_timer_stop(timerid_t timerid);

#endif
