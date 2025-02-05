/*
 *  Copyright 2024-2025 Cisco Systems, Inc.
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

#include "osal.h"
#include "csmpservice.h"
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

static osal_task_t timer_id_task;
static osal_sem_t sem;

static int32_t m_remaining = (1UL << 31) - 1; /* max int32_t */
static bool m_timert_isrunning = false;

static void osal_update_timer();
static void osal_alarm_fired();

void osal_kernel_start(void)
{
  (void) 0;
}

osal_basetype_t osal_task_create(osal_task_t * thread,
                                 const char * name,
                                 uint32_t priority,
                                 size_t stacksize,
                                 osal_task_fnc_t entry,
                                 void * arg)
{
    /* Silence compiler warnings about unused parameters. */
   (void) name;
   (void) priority;
   osal_basetype_t ret;
   pthread_attr_t attr;

   if (thread == NULL) {
       return OSAL_FAILURE;
   }

   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN + stacksize);

   ret = pthread_create(thread, &attr, (void *)entry, arg);
   if (ret != 0){
       DPRINTF("pthread_create failed: %d\n", ret);
       return OSAL_FAILURE;
   }
   ret = pthread_detach(*thread);
   if (ret != 0){
       DPRINTF("pthread_detach failed: %d\n", ret);
       return OSAL_FAILURE;
   }

   return OSAL_SUCCESS;
}

osal_basetype_t osal_task_cancel(osal_task_t thread)
{
    return (pthread_cancel(thread));
}

osal_basetype_t osal_task_setcanceltype(void){
    return (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL));
}

osal_basetype_t osal_task_sigmask(osal_basetype_t how, const osal_sigset_t *set, osal_sigset_t *oldset)
{
    return (pthread_sigmask(how, set, oldset));
}

osal_basetype_t osal_sem_create(osal_sem_t * sem, uint16_t value)
{
    return (sem_init(sem, 0, value));
}

osal_basetype_t osal_sem_post(osal_sem_t * sem)
{
    return (sem_post(sem));
}

osal_basetype_t osal_sem_wait(osal_sem_t * sem, osal_time_t timeout)
{
    /* Silence compiler warnings about unused parameters. */
    (void) timeout;
    return(sem_wait(sem));
}

osal_basetype_t osal_sem_destroy(osal_sem_t *sem)
{
    return (sem_destroy(sem));
}

osal_socket_handle_t osal_socket(osal_basetype_t domain, osal_basetype_t type, osal_basetype_t protocol)
{
    return(socket(domain, type, protocol));
}

osal_basetype_t osal_bind(osal_socket_handle_t sockd, osal_sockaddr_t *addr, osal_socklen_t addrlen)
{
    return (bind(sockd, (const struct sockaddr *)(addr), addrlen));
}

osal_ssize_t osal_recvfrom(osal_socket_handle_t sockd, void *buf, size_t len, osal_basetype_t flags,
                        osal_sockaddr_t *src_addr, osal_socklen_t *addrlen)
{
    return (recvfrom(sockd, buf, len, flags, (struct sockaddr*)(src_addr), addrlen));

}

osal_ssize_t osal_sendmsg(osal_socket_handle_t sockd, const struct msghdr msg, osal_basetype_t flags)
{
    return(sendmsg(sockd, &msg, flags));
}

osal_ssize_t osal_sendto(osal_socket_handle_t sockd, const void *buf, size_t len, osal_basetype_t flags,
                         const osal_sockaddr_t *dest_addr, osal_socklen_t addrlen)
{
    return(sendto(sockd, buf, len, flags, (struct sockaddr*)(dest_addr), addrlen));
}

osal_basetype_t osal_inet_pton(osal_basetype_t af, const char *src, void *dst)
{
    return(inet_pton(af, src, dst));
}

osal_basetype_t osal_select(osal_basetype_t nsds, osal_sd_set_t *readsds, osal_sd_set_t *writesds,
                  osal_sd_set_t *exceptsds, struct timeval *timeout)
{
    return(select(nsds, readsds, writesds, exceptsds, timeout));
}

void osal_update_sockaddr(osal_sockaddr_t *listen_addr, uint16_t sport)
{
    listen_addr->sin6_family = AF_INET6;
    listen_addr->sin6_addr = in6addr_any;
    listen_addr->sin6_port = htons(sport);
}

void osal_sd_zero(osal_sd_set_t *set)
{
    FD_ZERO(set);
}

void osal_sd_set(osal_socket_handle_t sd, osal_sd_set_t *set)
{
    FD_SET(sd, set);
}

osal_basetype_t osal_sd_isset(osal_socket_handle_t sd, osal_sd_set_t *set)
{
    return(FD_ISSET(sd, set));
}

osal_basetype_t osal_gettime(struct timeval *tv, struct timezone *tz)
{
    return(gettimeofday(tv, tz));
}

osal_basetype_t osal_settime(struct timeval *tv, struct timezone *tz)
{
    return(settimeofday(tv, tz));
}

osal_sighandler_t osal_signal(osal_basetype_t signum, osal_sighandler_t handler)
{
    return(signal(signum, handler));
}

osal_basetype_t osal_sigprocmask(osal_basetype_t how, const osal_sigset_t *set, osal_sigset_t *oldset)
{
    return(sigprocmask(how, set, oldset));
}

osal_basetype_t osal_sigemptyset(osal_sigset_t *set)
{
     return(sigemptyset(set));
}

osal_basetype_t osal_sigaddset(osal_sigset_t *set, osal_basetype_t signum)
{
    return(sigaddset(set, signum));
}

void osal_print_formatted_ip(const osal_sockaddr_t *sockadd)
{
    /* Silence compiler warnings about unused parameters. */
    (void)sockadd;
    DPRINTF("[%x:%x:%x:%x:%x:%x:%x:%x]:%hu\n",
      ((uint16_t)sockadd->sin6_addr.s6_addr[0] << 8) | sockadd->sin6_addr.s6_addr[1],
      ((uint16_t)sockadd->sin6_addr.s6_addr[2] << 8) | sockadd->sin6_addr.s6_addr[3],
      ((uint16_t)sockadd->sin6_addr.s6_addr[4] << 8) | sockadd->sin6_addr.s6_addr[5],
      ((uint16_t)sockadd->sin6_addr.s6_addr[6] << 8) | sockadd->sin6_addr.s6_addr[7],
      ((uint16_t)sockadd->sin6_addr.s6_addr[8] << 8) | sockadd->sin6_addr.s6_addr[9],
      ((uint16_t)sockadd->sin6_addr.s6_addr[10] << 8) |  sockadd->sin6_addr.s6_addr[11],
      ((uint16_t)sockadd->sin6_addr.s6_addr[12] << 8) |  sockadd->sin6_addr.s6_addr[13],
      ((uint16_t)sockadd->sin6_addr.s6_addr[14] << 8) |  sockadd->sin6_addr.s6_addr[15],
      ntohs(sockadd->sin6_port));
}

static void *osal_timer_thread(void* arg) {
  osal_sigset_t set;
  /* Silence compiler warnings about unused parameters. */
  (void)arg;
  osal_sigemptyset(&set);
  osal_sigaddset(&set, SIGALRM);
  osal_task_sigmask(SIG_UNBLOCK, &set, NULL);

  osal_signal(SIGALRM, osal_alarm_fired);
  while(1) {
    osal_sem_wait(&sem, 0);  //Suspend thread until sem_post()
    if (m_remaining <= 0)
      osal_alarm_fired();
    else {
      DPRINTF("trickle timer next fired time:%d sec\n", m_remaining);
      alarm(m_remaining);
    }
  }
  return NULL;
}

static void osal_alarm_fired(void)
{
  uint32_t min;
  uint8_t i;

  for (i = 0; i < timer_num; i++) {
    struct trickle_timer *timer = &timers[i];
    uint32_t now;
    struct timeval tv = {0};

    osal_gettime(&tv, NULL);
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

    switch (i) {
      case reg_timer: DPRINTF("Register trickle timer fired\n");
      break;

      case rpt_timer: DPRINTF("Periodic metrics report trickle timer fired\n");
      break;

      case lrq_timer: DPRINTF("Firmware load request timer fired\n");
      break;

      case async_timer: DPRINTF("Async CSMP Reponse timer fired\n");
      break;

      default: DPRINTF("Error: Invalid trickle timer id\n");
    }

    timer_fired[i]();
    min = timer->icur >> 1;
    timer->tfire = timer->t0 + min + (random() % (timer->icur - min));
  }
  osal_update_timer();
}

static void osal_update_timer() {
  uint32_t now;
  struct timeval tv = {0};
  uint8_t i;
  bool flag = false;

  m_remaining = (1UL << 31) - 1; /* max int32_t */
  osal_gettime(&tv, NULL);
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
    osal_sem_post(&sem);
}
void osal_trickle_timer_start(osal_timerid_t timerid, uint32_t imin, uint32_t imax, trickle_timer_fired_t trickle_timer_fired)
{
  uint32_t min;
  struct timeval tv = {0};
  uint32_t seed = 0;
  osal_sigset_t set;
  osal_basetype_t ret = 0;

  if(!m_timert_isrunning) {
    osal_sigemptyset(&set);
    osal_sigaddset(&set, SIGALRM);
    osal_sigprocmask(SIG_BLOCK, &set, NULL);

    osal_sem_create(&sem, 0);
    osal_task_create(&timer_id_task, NULL, 0, 0, osal_timer_thread, NULL);
    ret = osal_task_create(&timer_id_task, NULL, 0, 0, osal_timer_thread, NULL);
    assert(ret == OSAL_SUCCESS);

    m_timert_isrunning = true;
  }

  switch (timerid) {
    case reg_timer: DPRINTF("Register trickle timer start\n");
    break;

    case rpt_timer: DPRINTF("Periodic metrics report trickle timer start\n");
    break;

    case lrq_timer: DPRINTF("Firmware load request timer start\n");
    break;

    case async_timer: DPRINTF("Async CSMP Reponse timer start\n");
    break;

    default: DPRINTF("Error: Invalid trickle timer id\n");
  }

  osal_gettime(&tv, NULL);

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
  osal_update_timer();
}
void osal_trickle_timer_stop(osal_timerid_t timerid)
{
  uint8_t i;
  osal_sigset_t set;

  timers[timerid].is_running = false;

  switch (timerid) {
    case reg_timer: DPRINTF("Register trickle timer stop\n");
    break;

    case rpt_timer: DPRINTF("Periodic metrics report trickle timer stop\n");
    break;

    case lrq_timer: DPRINTF("Firmware load request timer stop\n");
    break;

    case async_timer: DPRINTF("Async CSMP Reponse timer stop\n");
    break;

    default: DPRINTF("Error: Invalid trickle timer id\n");
  }

  for(i = 0; i < timer_num; i++) {
    if(timers[i].is_running)
      return;
  }

  osal_task_cancel(timer_id_task);
  osal_sem_destroy(&sem);

  osal_sigemptyset(&set);
  osal_sigaddset(&set, SIGALRM);
  osal_task_sigmask(SIG_UNBLOCK, &set, NULL);
  m_timert_isrunning = false;
}

void *osal_malloc(size_t size)
{
  return malloc(size);
}

void osal_free(void *ptr)
{
  free(ptr);
}

void osal_sleep_ms(uint64_t ms)
{
  usleep(ms * 1000);
}

/****************************************************************************
 * @fn   osal_system_reboot
 *
 * @brief Reboots system
 *
 * input parameters
 *  @param[in] NMSaddr NMS IPv6 address
 *
 * output parameters
 * @return returns 1 on success and 0 on failure
 *****************************************************************************/
osal_basetype_t osal_system_reboot(struct in6_addr *NMSaddr)
{
  DPRINTF("osal_system_reboot: Reboot requested\n");
  int ret=0;
  ret = csmp_service_reboot(NMSaddr);
  if(ret)
    printf("\n\nosal_system_reboot: success!\nService registration in progress...\n\n");
  else
    printf("osal_system_reboot: csmp_service_reboot failed!\\n");
  return ret;
}
/****************************************************************************
 * @fn   osal_read_firmware
 *
 * @brief read firmware image from storage(file/flash)
 *
 * input parameters
 *  @param[in] slotid indicating RUN/UPLOAD/BACKUP slot
 *  @param[in] data pointer to uint8_t data array
 *  @param[in] size of data in bytes
 *
 * output parameters
 * @return returns 0 on success and -1 on error
 *****************************************************************************/
int osal_read_firmware(uint8_t slotid, uint8_t *data, uint32_t size) {
  FILE *file = NULL;
  switch(slotid){
    case RUN_IMAGE:
      file = fopen("opencsmp-run-slot.bin", "rb");
      break;
    case UPLOAD_IMAGE:
      file = fopen("opencsmp-upload-slot.bin", "rb");
      break;
    case BACKUP_IMAGE:
      file = fopen("opencsmp-backup-slot.bin", "rb");
      break;
    default:
      printf("osal_read_firmware: Invalid slotid\n");
      return OSAL_FAILURE;
  }
  if (file == NULL) {
      printf("osal_read_firmware: Failed to read firmware, slot-id: %u\n", slotid);
      return OSAL_FAILURE;
  }
  fread(data, sizeof(uint8_t), size, file);
  fclose(file);
  return OSAL_SUCCESS;
}

/****************************************************************************
 * @fn   osal_write_firmware
 *
 * @brief write firmware image to storage(file/flash)
 *
 * input parameters
 *  @param[in] slotid indicating RUN/UPLOAD/BACKUP slot
 *  @param[in] data pointer to uint8_t data array
 *  @param[in] size of data in bytes
 *
 * output parameters
 * @return returns 0 on success and -1 on error
 *****************************************************************************/
int osal_write_firmware(uint8_t slotid, uint8_t *data, uint32_t size) {
  FILE *file = NULL;
  size_t bytes=0;
  (void) bytes;
  switch(slotid){
    case RUN_IMAGE:
      file = fopen("opencsmp-run-slot.bin", "wb");
      break;
    case UPLOAD_IMAGE:
      file = fopen("opencsmp-upload-slot.bin", "wb");
      break;
    case BACKUP_IMAGE:
      file = fopen("opencsmp-backup-slot.bin", "wb");
      break;
    default:
      printf("osal_write_firmware: Invalid slotid\n");
      return OSAL_FAILURE;
  }
  if (file == NULL) {
      printf("osal_read_firmware: Failed to read firmware, slot-id: %u\n", slotid);
      return OSAL_FAILURE;
  }
  bytes = fwrite(data,sizeof(uint8_t),size, file);
  DPRINTF("osal_write_firmware: Wrote %ld bytes to slot-id: %u\n", bytes, slotid);
  fclose(file);
  return OSAL_SUCCESS;
}

/****************************************************************************
 * @fn   osal_write_slothdr
 *
 * @brief write firmware slot header data to storage(file/flash)
 *
 * input parameters
 *  @param[in] slotid indicating RUN/UPLOAD/BACKUP slot
 *  @param[in] slot pointer to _Csmp_Slothdr slot structure
 *
 * output parameters
 * @return returns 0 on success and -1 on error
 *****************************************************************************/
int osal_write_slothdr(uint8_t slotid, Csmp_Slothdr *slot){
FILE *file = NULL;
size_t bytes = 0;
(void) bytes;
  switch(slotid){
    case RUN_IMAGE:
      file = fopen("opencsmp-run-slothdr.bin", "wb");
      break;
    case UPLOAD_IMAGE:
      file = fopen("opencsmp-upload-slothdr.bin", "wb");
      break;
    case BACKUP_IMAGE:
      file = fopen("opencsmp-backup-slothdr.bin", "wb");
      break;
    default:
      printf("osal_write_slothdr: Invalid slotid\n");
      return OSAL_FAILURE;
  }
  if (file == NULL) {
      printf("osal_write_slothdr: Failed to write for slothdr, slot-id: %u\n", slotid);
      return OSAL_FAILURE;
  }

  // Write CSMP header + firmware to persist slot data across agent reboot
  bytes = fwrite(&(slot[slotid]), sizeof(Csmp_Slothdr), 1, file);
  DPRINTF("osal_write_slothdr: Wrote %ld bytes to slot-id: %u\n", bytes, slotid);
  fclose(file);

  return OSAL_SUCCESS;

}

/****************************************************************************
 * @fn   osal_read_slothdr
 *
 * @brief read firmware slot header data from storage(file/flash)
 *
 * input parameters
 *  @param[in] slotid indicating RUN/UPLOAD/BACKUP slot
 *  @param[in] slot pointer to _Csmp_Slothdr slot structure
 *
 * output parameters
 * @return returns 0 on success and -1 on error
 *****************************************************************************/
int osal_read_slothdr(uint8_t slotid, Csmp_Slothdr *slot){
  FILE *file = NULL;
  switch(slotid) {
    case RUN_IMAGE:
      file = fopen("opencsmp-run-slothdr.bin", "rb");
      break;
    case UPLOAD_IMAGE:
      file = fopen("opencsmp-upload-slothdr.bin", "rb");
      break;
    case BACKUP_IMAGE:
      file = fopen("opencsmp-backup-slothdr.bin", "rb");
      break;
    default:
      printf("osal_read_slothdr: Invalid slot id\n");
      return OSAL_FAILURE;
  }
  if (file == NULL) {
      printf("osal_read_slothdr: Failed to read slothdr for slot-id: %u\n", slotid);
      return OSAL_FAILURE;
  }
  fread(&(slot[slotid]), sizeof(Csmp_Slothdr), 1, file);
  fclose(file);
  return OSAL_SUCCESS;
}

/****************************************************************************
 * @fn   osal_copy_firmware
 *
 * @brief Copy firmware image and slot header data from source to dest slot
 *
 * input parameters
 *  @param[in] source_slotid and dest_slotid indicating RUN/UPLOAD/BACKUP slot
 *  @param[in] pointer to _Csmp_Slothdr slot structure
 *
 * output parameters
 * @return returns 0 on success and -1 on error
 *****************************************************************************/
int osal_copy_firmware(uint8_t source_slotid, uint8_t dest_slotid, Csmp_Slothdr *slot){

  FILE *source_fw = NULL, *dest_fw = NULL;
  uint8_t buff[1024] = {0};
  int ret=0;
  (void) ret;
  if(source_slotid == dest_slotid)
    return OSAL_SUCCESS;
  switch (source_slotid)
  {
  case RUN_IMAGE:
    source_fw = fopen("opencsmp-run-slot.bin", "rb");
    break;
  case UPLOAD_IMAGE:
    source_fw = fopen("opencsmp-upload-slot.bin", "rb");
    break;
  case BACKUP_IMAGE:
    source_fw = fopen("opencsmp-backup-slot.bin", "rb");
    break;
  default:
      printf("osal_copy_firmware: Invalid slot id\n");
      return OSAL_FAILURE;
    break;
  }
  if(source_fw == NULL){
    printf("osal_copy_firmware: Copy function failed, source file could not be opened\n");
    return OSAL_FAILURE;
  }
  switch (dest_slotid)
  {
  case RUN_IMAGE:
    dest_fw = fopen("opencsmp-run-slot.bin", "wb");
    break;
  case UPLOAD_IMAGE:
    dest_fw = fopen("opencsmp-upload-slot.bin", "wb");
    break;
  case BACKUP_IMAGE:
    dest_fw = fopen("opencsmp-backup-slot.bin", "wb");
    break;
  default:
      printf("osal_copy_firmware: Invalid slot id\n");
      return OSAL_FAILURE;
    break;
  }
  if(dest_fw == NULL){
    printf("osal_copy_firmware: Copy function failed, dest file could not be opened\n");
    fclose(source_fw);
    return OSAL_FAILURE;
  }

  while ((ret = fread(buff, 1, sizeof(buff), source_fw)) > 0) {
    fwrite(buff, 1, ret, dest_fw);
  }
  fclose(source_fw);
  fclose(dest_fw);
  DPRINTF("osal_copy_firmware: Copied Firmware Successfully\n");
  memcpy(&(slot[dest_slotid]), &(slot[source_slotid]), sizeof(Csmp_Slothdr));
  if(osal_write_slothdr(dest_slotid, slot) == OSAL_FAILURE)
    {
      printf("osal_copy_firmware: Failed to copy slothdr\n");
      return OSAL_FAILURE;
    }

  DPRINTF("osal_copy_firmware: Copied Slothdr Successfully\n");
  return OSAL_SUCCESS;
}

int osal_read_firmware(uint8_t slotid, osal_csmp_slothdr_t *slot) {
  FILE *file = NULL;

  if (slot == NULL) {
    printf("read_firmware: slot is NULL\n");
    return OSAL_FAILURE;
  }

  switch(slotid) {
    case RUN_IMAGE:
      file = fopen("opencsmp-run-slot.bin", "rb");
      break;
    case UPLOAD_IMAGE:
      file = fopen("opencsmp-upload-slot.bin", "rb");
      break;
    case BACKUP_IMAGE:
      file = fopen("opencsmp-backup-slot.bin", "rb");
      break;
    default:
      printf("read_firmware: Invalid slot id\n");
      return OSAL_FAILURE;
  }
  if (file == NULL) {
      printf("read_firmware: Failed to read firmware slot-id: %u\n", slotid);
      return OSAL_FAILURE;
  }
  fread((uint8_t*) slot, sizeof(osal_csmp_slothdr_t), 1, file);
  fclose(file);

  return OSAL_SUCCESS;
}

osal_basetype_t osal_write_firmware(uint8_t slotid, osal_csmp_slothdr_t *slot) {
  FILE *file = NULL;
  size_t bytes = 0;

  (void) bytes;
  if (slot == NULL) {
    printf("write_firmware: slot is NULL\n");
    return OSAL_FAILURE;
  }
  
  switch(slotid) {
    case RUN_IMAGE:
      file = fopen("opencsmp-run-slot.bin", "wb");
      break;
    case UPLOAD_IMAGE:
      // Write firmware w/o CSMP header to verify upload firmware transferred from FND
      file = fopen("opencsmp-upload-image.bin", "wb");
      if(file == NULL){
        printf("write_firmware: Failed to write upload firmware image\n");
        return OSAL_FAILURE;
      }
      bytes = fwrite((uint8_t*) slot->image, sizeof(uint8_t), slot->filesize, file);
      DPRINTF("write_firmware: Wrote %ld bytes of upload firmware image\n", bytes);
      fclose(file);
      
      file = fopen("opencsmp-upload-slot.bin", "wb");
      break;
    case BACKUP_IMAGE:
      file = fopen("opencsmp-backup-slot.bin", "wb");
      break;
    default:
      printf("write_firmware: Invalid slot id\n");
      return OSAL_FAILURE;
  }
  if (file == NULL) {
      printf("write_firmware: Failed to write firmware slot-id: %u\n", slotid);
      return OSAL_FAILURE;
  }
  // Write CSMP header + firmware to persist slot data across agent reboot
  bytes = fwrite((uint8_t*) slot, sizeof(uint8_t), sizeof(osal_csmp_slothdr_t), file);
  DPRINTF("write_firmware: Wrote %ld bytes to slot-id: %u\n", bytes, slotid);
  fclose(file);

  return OSAL_SUCCESS;
}
