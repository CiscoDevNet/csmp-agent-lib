#include "osal_common.h"

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
static osal_sem sem;

static int32_t m_remaining = (1UL << 31) - 1; /* max int32_t */
static bool m_timert_isrunning = false;

static void osal_update_timer();
static void osal_alarm_fired();

/****************************************************************************
 * @fn        osal_task_create
 *
 * @brief     create new task/thread. 
 * input parameters
 *
 *  @param[in] name name of the task
 *  @param[in] priotiy of task 
 *  @param[in] stacksize size of the thread stack.
 *  @param[in] entry the task is created executing entry 
 *  @param[in] arg entry routine arguments 
 *
 * output parameters
 * @return    osal_task_t ID of the created task/thread
 *****************************************************************************/
osal_task_t osal_task_create (
   const char * name,
   uint32_t priority,
   size_t stacksize,
   void *(*entry) (void * arg),
   void * arg)
{
    /* Silence compiler warnings about unused parameters. */
   (void) name;
   (void) priority;
   int ret;
   pthread_t thread;
   pthread_attr_t attr;
   pthread_attr_init (&attr);
   pthread_attr_setstacksize (&attr, PTHREAD_STACK_MIN + stacksize);

   ret = pthread_create (&thread, &attr, (void *)entry, arg);
   if (ret != 0){
       return (-ret);
   }
   ret = pthread_detach(thread);
   if (ret != 0){
       return (-ret);
   }

   return thread;
}

/****************************************************************************
 * @fn        osal_task_cancel
 *
 * @brief     function requests that thread be canceled. 
 * input parameters
 *  @param[in] thread thread ID to be canceled
 *
 * output parameters
 * @return   on success return 0 otherwise error value 
 *****************************************************************************/
int osal_task_cancel(osal_task_t thread)
{
    return (pthread_cancel(thread));
}

/****************************************************************************
 * @fn        osal_task_setcanceltype()
 *
 * @brief     function atomically both sets the calling thread's cancelability 
 * 	      state and returns oldstate 
 * input parameters
 *  @param[in] type cancelability state to be set
 *  @param[out] oldtype old cancelability state to be returned
 *
 * output parameters
 * @return   on success return 0 otherwise error value 
 *****************************************************************************/
int osal_task_setcanceltype(int type, int *oldtype){
    return (pthread_setcanceltype(type, oldtype));
}

/****************************************************************************
 * @fn        osal_task_sigmask
 *
 * @brief    function examines and/or changes the calling thread's signal mask.
 *
 * input parameters
 *  @param[in] how specifies what to set the signal mask to
 *  @param[in] set of signals to be modified
 *  @param[out] oldset previous signal mask is stored in the location pointed
 *
 * output parameters
 * @return   on success return 0 otherwise error value 
 *****************************************************************************/
int osal_task_sigmask(int how, const sigset_t *set, sigset_t *oldset)
{
    return (pthread_sigmask(how, set, oldset));
}

/****************************************************************************
 * @fn        osal_sem_create()
 *
 * @brief   initialize an unnamed semaphore. 
 *
 * input parameters
 *  @param[in] sem address to store semaphore 
 *  @param[in] value initial value of semaphore
 *
 * output parameters
 * @return 0 on success; on error, -1 is returned 
 *****************************************************************************/
int osal_sem_create (osal_sem * sem, uint16_t value)
{
    return (sem_init(sem, 0, value));
}

/****************************************************************************
 * @fn        osal_sem_post()
 *
 * @brief   increments (unlocks) the semaphore pointed to by sem 
 *
 * input parameters
 *  @param[in] sem semaphore 
 *
 * output parameters
 * @return 0 on success; on error, -1 is returned 
 *****************************************************************************/
int osal_sem_post (osal_sem * sem)
{
    return (sem_post(sem));
}

/****************************************************************************
 * @fn        osal_sem_wait()
 *
 * @brief   decrements (locks) the semaphore pointed to by sem
 *
 * input parameters
 *  @param[in] sem semaphore 
 *  @param[in] timeout value to wait if semaphore is not availbel.
 *
 * output parameters
 * @return 0 on success; on error, -1 is returned 
 *****************************************************************************/
int osal_sem_wait (osal_sem * sem, osal_time_t timeout)
{
    /* Silence compiler warnings about unused parameters. */
    (void) timeout;
    return(sem_wait(sem));
}

/****************************************************************************
 * @fn        osal_sem_destroy()
 *
 * @brief  destroys the unnamed semaphore at the address pointed to by sem 
 *
 * input parameters
 *  @param[in] sem semaphore 
 *
 * output parameters
 * @return 0 on success; on error, -1 is returned 
 *****************************************************************************/
int osal_sem_destroy(osal_sem *sem)
{
    return (sem_destroy(sem)); 
}

/****************************************************************************
 * @fn        osal_socket
 *
 * @brief   creates an endpoint for communication
 *
 * input parameters
 *  @param[in] domain communication domain
 *  @param[in] type communication semantics
 *  @param[in] protocol specifies a particular protocol to be used with the socket.
 *
 * output parameters
 * @return On success, a file descriptor for the new socket is returned.  
 *         On error, -1 is returned
 *****************************************************************************/
osal_socket_handle_t osal_socket(osal_basetype_t domain, osal_basetype_t type, osal_basetype_t protocol)
{
    return(socket(domain, type, protocol));
}

/****************************************************************************
 * @fn        osal_bind
 *
 * @brief   assigns the address specified by osal_addr to the socket 
 *          referred to by the file descriptor osal_sockfd.
 *
 * input parameters
 *  @param[in] osal_sockfd socket file descriptor 
 *  @param[in] osal_addr network address
 *  @param[in] addrlen size in bytes of the address structure 
 *
 * output parameters
 * @return On success 0 is returned.  On error, -1 is returned 
 *****************************************************************************/
osal_basetype_t osal_bind(osal_socket_handle_t osal_sockfd, osal_sockaddr *osal_addr, osal_socklen addrlen)
{
    return (bind(osal_sockfd, (const struct sockaddr *)(osal_addr), addrlen));
}

/****************************************************************************
 * @fn      osal_recvfrom
 *
 * @brief  used to receive messages from a socket. 
 *
 * input parameters
 *  @param[in] sockfd socket file descriptor 
 *  @param[out] buf contains received messages 
 *  @param[in] len size of buffer 
 *  @param[in] flags argument is formed by ORing one or many socket options 
 *  @param[in] src_addr source address 
 *  @param[in] addrlen size in bytes of the address structure
 *
 * output parameters
 * @return On success 0 is returned.  On error, -1 is returned 
 *****************************************************************************/
osal_basetype_t osal_recvfrom(osal_socket_handle_t sockfd, void *buf, size_t len, osal_basetype_t flags,
                        osal_sockaddr *src_addr, osal_socklen *addrlen)
{
    return (recvfrom(sockfd, buf, len, flags, (struct sockaddr*)(src_addr), addrlen));

}

/****************************************************************************
 * @fn      osal_sendmsg
 *
 * @brief  used to transmit a message to another socket. 
 *
 * input parameters
 *  @param[in] sockfd socket file descriptor 
 *  @param[in] msghdr structure contains message meta data.
 *  @param[in] flags argument is formed by ORing one or many socket options 
 *
 * output parameters
 * @return On success return the number of bytes sent on error -1 is returned 
 *****************************************************************************/
osal_ssize_t osal_sendmsg(int sockfd, const struct msghdr msg, int flags)
{
    return(sendmsg(sockfd, &msg, flags));
}

/****************************************************************************
 * @fn      osal_sendto
 *
 * @brief  used to transmit a message to another socket. 
 *
 * input parameters
 *  @param[in] sockfd socket file descriptor 
 *  @param[in] buf message buffer
 *  @param[in] len data buffer len 
 *  @param[in] flags argument is formed by ORing one or many socket options 
 *  @param[in] dest_addr destination address 
 *  @param[in] addrlen address length
 *
 * output parameters
 * @return On success return the number of bytes sent on error -1 is returned 
 *****************************************************************************/
osal_ssize_t osal_sendto(osal_basetype_t sockfd, const void *buf, size_t len, int flags,
                         const osal_sockaddr *dest_addr, osal_socklen addrlen)
{
    return(sendto(sockfd, buf, len, flags, (struct sockaddr*)(dest_addr), addrlen));
}

/****************************************************************************
 * @fn      osal_inet_pton
 *
 * @brief  This function converts the character string src into a network 
 *         address structure in the af address family 
 *
 * input parameters
 *  @param[in] af address family 
 *  @param[in] src character string 
 *  @param[out] dst network address structure
 *
 * output parameters
 * @return 1 on success
 *         0 if src does not contain a character string representing a valid
 *           network address in the specified address family
 *        -1 if af does not contain a valid address family
 *****************************************************************************/
int osal_inet_pton(int af, const char *src, void *dst)
{
    return(inet_pton(af, src, dst));
}

/****************************************************************************
 * @fn      osal_select
 *
 * @brief Allows a program to monitor multiple file descriptors, waiting until
 *        one or more of the file descriptors become "ready" for some class of 
 *        I/O operatio 
 *
 * input parameters
 *  @param[in] nfds This argument should be set to the highest-numbered file 
 *                  descriptor plus one
 *  @param[in] readfds The file descriptors in this set are watched to see if 
 *                     they are ready for reading 
 *  @param[in] writefds The file descriptors in this set are watched to see 
 *                      if they are ready for writing.
 *  @param[in] exceptfds The  file descriptors in this set are watched for 
 *                     "exceptional conditions". 
 *  @param[in] timeout The interval that osal_select() should block waiting 
 *                     for a file descriptor to become ready
 *
 * output parameters
 * @return on success returns the number of file descriptors contained in the 
 *         three returned descriptor sets 
 *         0 if timeout expired  before any file descriptors became ready. 
 *        -1 on error
 *****************************************************************************/
int osal_select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout)
{
    return(select(nfds, readfds, writefds, exceptfds, timeout));
}

/****************************************************************************
 * @fn     osal_update_sockaddr 
 *
 * @brief  update the ipv6 socket address structure
 *
 * input parameters
 *  @param[in] listen_addr address of sockaddr structure 
 *  @param[in] sport source port 
 * output parameters
 * @return none
 *****************************************************************************/
void osal_update_sockaddr(osal_sockaddr *listen_addr, uint16_t sport)
{
    listen_addr->sin6_family = AF_INET6;
    listen_addr->sin6_addr = in6addr_any;
    listen_addr->sin6_port = htons(sport);
}

/****************************************************************************
 * @fn     osal_fd_zero 
 *
 * @brief clears (removes all file descriptors from) set 
 *
 * input parameters
 *  @param[in] set file descriptor set 
 * output parameters
 * @return none
 *****************************************************************************/
void osal_fd_zero(fd_set *set)
{
    FD_ZERO(set);
}

/****************************************************************************
 * @fn     osal_fd_set 
 *
 * @brief  This  macro  adds the file descriptor fd to set
 *
 * input parameters
 *  @param[in] fd file descriptor 
 *  @param[in] set file descriptor set 
 * output parameters
 * @return none
 *****************************************************************************/
void osal_fd_set(int fd, fd_set *set)
{
    FD_SET(fd, set);
}

/****************************************************************************
 * @fn     osal_fd_isset 
 *
 * @brief  modifies  the  contents of the sets
 *
 * input parameters
 *  @param[in] fd file descriptor 
 *  @param[in] set file descriptor set 
 * output parameters
 * @return returns nonzero if the file  descriptor fd is present in set, 
 *         and zero if it is not 
 *****************************************************************************/
int osal_fd_isset(int fd, fd_set *set)
{
    return(FD_ISSET(fd, set));
}

/****************************************************************************
 * @fn     osal_gettimeofday
 *
 * @brief get time as well as a timezone 
 *
 * input parameters
 *  @param[in] tv is a struct timeval 
 *  @param[in] tz is a struct timezone
 * output parameters
 * @return 0 for success, or -1 for failure 
 *****************************************************************************/
int osal_gettimeofday(struct timeval *tv, struct timezone *tz)
{
    return(gettimeofday(tv, tz));
}

/****************************************************************************
 * @fn     osal_settimeofday
 *
 * @brief set time as well as a timezone 
 *
 * input parameters
 *  @param[in] tv is a struct timeval 
 *  @param[in] tz is a struct timezone
 * output parameters
 * @return 0 for success, or -1 for failure 
 *****************************************************************************/
int osal_settime(struct timeval *tv, struct timezone *tz)
{
    return(settimeofday(tv, tz));
}

/****************************************************************************
 * @fn    osal_signal 
 *
 * @brief  sets the disposition of the signal signum to handler
 *
 * input parameters
 *  @param[in] signum is delivered to the process 
 *  @param[in] handler programmer-de‐fined function (a "signal handler")
 * output parameters
 * @return returns the previous value of the signal handler, or SIG_ERR on error 
 *****************************************************************************/
osal_sighandler_t osal_signal(int signum, osal_sighandler_t handler)
{
    return(signal(signum, handler));
}

/****************************************************************************
 * @fn    osal_sigprocmask 
 *
 * @brief fetch and/or change the signal mask of the calling thread 
 *
 * input parameters
 *  @param[in] how define behavior of the call SIG_BLOCK, SIG_UNBLOCK, SIG_SETMASK
 *  @param[in] set set of signals
 *  @param[in] oldset the previous value of the signal mask
 * output parameters
 * @return returns 0 on success and -1 on error 
 *****************************************************************************/
int osal_sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    return(sigprocmask(how, set, oldset));
}

/****************************************************************************
 * @fn    osal_sigemptyset
 *
 * @brief initializes the signal set given by set to empty, 
 *        with all signals excluded from the set 
 *
 * input parameters
 *  @param[in] set set of signals
 * output parameters
 * @return returns 0 on success and -1 on error 
 *****************************************************************************/
int osal_sigemptyset(sigset_t *set)
{
     return(sigemptyset(set));
}

/****************************************************************************
 * @fn    osal_sigaddset
 *
 * @brief add and delete respectively signal signum from set.
 *
 * input parameters
 *  @param[in] set set of signals
 * output parameters
 * @return returns 0 on success and -1 on error 
 *****************************************************************************/
int osal_sigaddset(sigset_t *set, int signum)
{
    return(sigaddset(set, signum));
}

/****************************************************************************
 * @fn    osal_print_formatted_ip
 *
 * @brief print formatted ip v6 address from sockadd for debugging purpose
 *
 * input parameters
 *  @param[in] sockadd socket address structure
 * output parameters
 * @return none 
 *****************************************************************************/
void osal_print_formatted_ip(const osal_sockaddr *sockadd)
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
  sigset_t set;
  /* Silence compiler warnings about unused parameters. */
  (void)arg;
  osal_sigemptyset(&set);
  osal_sigaddset(&set, SIGALRM);
  osal_task_sigmask(SIG_UNBLOCK, &set, NULL);

  osal_signal(SIGALRM, osal_alarm_fired);
  while(1) {
    osal_sem_wait(&sem, 0);  //Suspend thread until sem_post()
    if (m_remaining <= 0)
      osal_alarm_fired(SIGALRM);
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

    osal_gettimeofday(&tv, NULL);
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
  osal_update_timer();
}

static void osal_update_timer() {
  uint32_t now;
  struct timeval tv = {0};
  uint8_t i;
  bool flag = false;

  m_remaining = (1UL << 31) - 1; /* max int32_t */
  osal_gettimeofday(&tv, NULL);
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

/****************************************************************************
 * @fn   osal_trickle_timer_start
 *
 * @brief starts trickle timer
 *
 * input parameters
 *  @param[in] timerid timer identification value
 *  @param[in] imin minimum timer value
 *  @param[in] imax maximum timer value
 *  @param[in] trickle_timer_fired  timer fired 
 * output parameters
 * @return none 
 *****************************************************************************/
void osal_trickle_timer_start(timerid_t timerid, uint32_t imin, uint32_t imax, trickle_timer_fired_t trickle_timer_fired)
{
  uint32_t min;
  struct timeval tv = {0};
  uint32_t seed = 0;
  sigset_t set;

  if(!m_timert_isrunning) {
    osal_sigemptyset(&set);
    osal_sigaddset(&set, SIGALRM);
    osal_sigprocmask(SIG_BLOCK, &set, NULL);

    osal_sem_create(&sem, 0);
    timer_id_task = osal_task_create(NULL, 0, 0, osal_timer_thread, NULL);
    m_timert_isrunning = true;
  }

  if(timerid == reg_timer) {
    DPRINTF("register trickle timer start\n");
  }
  else if(timerid == rpt_timer) {
    DPRINTF("metrics report trickle timer start\n");
  }

  osal_gettimeofday(&tv, NULL);

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

/****************************************************************************
 * @fn   osal_trickle_timer_stop
 *
 * @brief stop trickle timer
 *
 * input parameters
 *  @param[in] timerid timer identification value
 * output parameters
 * @return none 
 *****************************************************************************/
void osal_trickle_timer_stop(timerid_t timerid)
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

  osal_task_cancel(timer_id_task);
  osal_sem_destroy(&sem);

  osal_sigemptyset(&set);
  osal_sigaddset(&set, SIGALRM);
  osal_task_sigmask(SIG_UNBLOCK, &set, NULL);
  m_timert_isrunning = false;
}
