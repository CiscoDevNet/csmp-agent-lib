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

#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

#include "trickle_timer.h"
#include "debug.h"

struct trickle_timer {
  uint32_t t0;
  uint32_t tfire;
  uint32_t icur;
  uint32_t imin;
  uint32_t imax;
  uint8_t is_running :1;
};
extern uint8_t g_csmplib_eui64[8];

static struct trickle_timer timers[timer_num];
static trickle_timer_fired_t timer_fired[timer_num];

static pthread_t timert_id;
static sem_t sem;

static int32_t m_remaining = (1UL << 31) - 1; /* max int32_t */
static bool m_timert_isrunning = false;

void update_timer();
void alarm_fired();

void *timer_thread(void*) {
  sigset_t set;

  sigemptyset(&set);
  sigaddset(&set, SIGALRM);
  pthread_sigmask(SIG_UNBLOCK, &set, NULL);

  signal(SIGALRM, alarm_fired);
  while(1) {
    sem_wait(&sem);  //suspend thread until sem_post()
    if (m_remaining <= 0)
      alarm_fired(SIGALRM);
    else {
      DPRINTF("trickle timer next fired time:%d sec\n", m_remaining);
      alarm(m_remaining);
    }
  }
}

void alarm_fired(void)
{
  uint32_t min;
  uint8_t i;

  for (i = 0; i < timer_num; i++) {
    struct trickle_timer *timer = &timers[i];
    uint32_t now;
    struct timeval tv = {0};

    gettimeofday(&tv, NULL);
    now = tv.tv_sec;

    if (timer->is_running == false)
      continue;

    if ((int32_t)(timer->tfire - now) > 0)
      continue;

    // update t0 to next interval
    timer->t0 += timer->icur;

    // double interval size
    timer->icur <<= 1;
    if (timer->icur > timer->imax)
      timer->icur = timer->imax;

    if(i == reg_timer) {
      DPRINTF("register trickle timer fired\n");
    }
    else if(i == rpt_timer) {
      DPRINTF("metrics report trickle timer fired\n");
    }

    timer_fired[i]();
    min = timer->icur >> 1;
    timer->tfire = timer->t0 + min + (random() % (timer->icur - min));
  }
  update_timer();
}

void update_timer() {
  uint32_t now;
  struct timeval tv = {0};
  uint8_t i;
  bool flag = false;

  m_remaining = (1UL << 31) - 1; /* max int32_t */
  gettimeofday(&tv, NULL);
  now = tv.tv_sec;

  for (i = 0; i < timer_num; i++) {
    struct trickle_timer *timer = &timers[i];
    int32_t remaining;

    if (timer->is_running == false)
      continue;
    remaining = timer->tfire - now;
    if (remaining < m_remaining) {
      m_remaining = remaining;
      flag = true;
    }
  }
  if(flag)
    sem_post(&sem);
}

void trickle_timer_start(timerid_t timerid, uint32_t imin, uint32_t imax, trickle_timer_fired_t trickle_timer_fired)
{
  uint32_t min;
  struct timeval tv = {0};
  uint32_t seed = 0;
  sigset_t set;

  if(!m_timert_isrunning) {
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    sigprocmask(SIG_BLOCK, &set, NULL);

    sem_init(&sem, 0, 0);
    pthread_create(&timert_id, NULL, timer_thread, NULL);
    pthread_detach(timert_id);
    m_timert_isrunning = true;
  }

  if(timerid == reg_timer) {
    DPRINTF("register trickle timer start\n");
  }
  else if(timerid == rpt_timer) {
    DPRINTF("metrics report trickle timer start\n");
  }

  gettimeofday(&tv, NULL);

  seed = (((uint16_t)g_csmplib_eui64[6] << 8) | g_csmplib_eui64[7]);
  srand(seed);
  timers[timerid].t0 = tv.tv_sec + (random()%imin);
  timers[timerid].icur = imin;
  timers[timerid].imin = imin;
  timers[timerid].imax = imax;
  timers[timerid].is_running = true;
  timer_fired[timerid] = trickle_timer_fired;
  min = timers[timerid].icur >> 1;
  timers[timerid].tfire = timers[timerid].t0 + min + (random() % (timers[timerid].icur - min));
  update_timer();
}

void trickle_timer_stop(timerid_t timerid)
{
  uint8_t i;
  sigset_t set;

  timers[timerid].is_running = false;
  if(timerid == reg_timer) {
    DPRINTF("register trickle timer stop\n");
  }
  else if(timerid == rpt_timer) {
    DPRINTF("metrics report trickle timer stop\n");
  }
  for(i = 0; i < timer_num; i++) {
    if(timers[i].is_running)
      return;
  }

  pthread_cancel(timert_id);
  sem_destroy(&sem);

  sigemptyset(&set);
  sigaddset(&set, SIGALRM);
  pthread_sigmask(SIG_UNBLOCK, &set, NULL);
  m_timert_isrunning = false;
}
