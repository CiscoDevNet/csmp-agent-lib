/*
 *  Copyright 2024 Cisco Systems, Inc.
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

#ifndef __OSAL_H
#define __OSAL_H

#include "osal_platform_types.h"


/// OSAL Success return value
#define OSAL_SUCCESS 0

/// OSAL Failure return value
#define OSAL_FAILURE -1

/* 
 * trickle_timer defines
 */
typedef enum {
 reg_timer = 0,  /**< register timer */
 rpt_timer = 1,  /**< reporting timer */
 lrq_timer = 2,  /**< firmware load request timer */
 async_timer = 3,  /**< async response timer for description/load/backup request */
 timer_num = 4   /**< max amount of timers */
} osal_timerid_t;

typedef void (*trickle_timer_fired_t) ();

/****************************************************************************
 * @fn        osal_kernel_start
 * @brief     Initialization code for starting kernel. The function implementation
 *            should contain context initialisation for threads, 
 *            like timer and other OS object initialisation.
*****************************************************************************/
void osal_kernel_start(void);

/****************************************************************************
 * @fn osal_task_create
 *
 * @brief create new task/thread. 
 * 
 * input parameters
 * @param[in] thread address to store thread ID
 * @param[in] name name of the task
 * @param[in] priority of task. Higher number means higher priority.
 *            This value should be added to the base priority of the task (implementation specific)
 * @param[in] stacksize size of the thread stack. 
 *            This value is added to the minimum default stack size of the task (implementation specific)
 * @param[in] entry the task is created executing entry 
 * @param[in] arg entry routine arguments 
 *
 * output parameters
 * @return osal_basetype_t on success return 0 otherwise error value
 *****************************************************************************/
osal_basetype_t osal_task_create(osal_task_t * thread,
                                 const char * name,
                                 uint32_t priority,
                                 size_t stacksize,
                                 osal_task_fnc_t entry,
                                 void * arg);

/****************************************************************************
 * @fn osal_task_cancel
 *
 * @brief function requests that thread be canceled.
 * 
 * input parameters
 * @param[in] thread thread ID to be canceled
 *
 * output parameters
 * @return on success return 0 otherwise error value 
 *****************************************************************************/
osal_basetype_t osal_task_cancel(osal_task_t thread);

/****************************************************************************
 * @fn osal_task_setcanceltype
 *
 * @brief sets the cancelability of the calling thread. 
 *        This function usually used for pthreads.
 * 
 * input parameters
 *
 * output parameters
 * @return on success return 0 otherwise error value 
 *****************************************************************************/
osal_basetype_t osal_task_setcanceltype(void);

/****************************************************************************
 * @fn osal_task_sigmask
 *
 * @brief function examines and/or changes the calling thread's signal mask.
 *
 * input parameters
 * @param[in] how specifies what to set the signal mask to
 * @param[in] set of signals to be modified
 * @param[out] oldset previous signal mask is stored in the location pointed
 *
 * output parameters
 * @return on success return 0 otherwise error value 
 *****************************************************************************/
osal_basetype_t osal_task_sigmask(osal_basetype_t how, const osal_sigset_t *set, osal_sigset_t *oldset);

/****************************************************************************
 * @fn osal_sem_create
 *
 * @brief initialize an unnamed semaphore. 
 *
 * input parameters
 * @param[in] sem address to store semaphore 
 * @param[in] value initial value of semaphore
 *
 * output parameters
 * @return 0 on success; on error, -1 is returned 
 *****************************************************************************/
osal_basetype_t osal_sem_create(osal_sem_t * sem, uint16_t value);

/****************************************************************************
 * @fn osal_sem_post
 *
 * @brief increment the value of the semaphore. 
 *
 * input parameters
 * @param[in] sem address of semaphore
 *
 * output parameters
 * @return 0 on success; on error, -1 is returned 
 *****************************************************************************/
osal_basetype_t osal_sem_post(osal_sem_t * sem);

/****************************************************************************
 * @fn osal_sem_wait
 *
 * @brief decrement the value of the semaphore. 
 *
 * input parameters
 * @param[in] sem address of semaphore
 * @param[in] timeout time to wait for semaphore
 * 
 * output parameters
 * @return 0 on success; on error, -1 is returned
 * *****************************************************************************/
osal_basetype_t osal_sem_wait(osal_sem_t * sem, osal_time_t timeout);

/****************************************************************************
 * @fn osal_sem_destroy
 *
 * @brief destroy the semaphore. 
 *
 * input parameters
 * @param[in] sem address of semaphore
 *
 * output parameters
 * @return 0 on success; on error, -1 is returned
 *****************************************************************************/
osal_basetype_t osal_sem_destroy(osal_sem_t *sem);

/****************************************************************************
 * @fn osal_socket
 *
 * @brief create an endpoint for communication. 
 *
 * input parameters
 * @param[in] domain specifies a communication domain
 * @param[in] type specifies the communication semantics
 * @param[in] protocol specifies a particular protocol to be used with the socket
 *
 * output parameters
 * @return 0 on success; on error, -1 is returned
 *****************************************************************************/
osal_socket_handle_t osal_socket(osal_basetype_t domain, osal_basetype_t type, osal_basetype_t protocol);

/****************************************************************************
 * @fn osal_recvfrom
 *
 * @brief used to receive messages from a socket. 
 *
 * input parameters
 * @param[in] sockd socket descriptor 
 * @param[out] buf contains received messages 
 * @param[in] len size of buffer 
 * @param[in] flags argument is formed by ORing one or many socket options 
 * @param[in] src_addr source address 
 * @param[in] addrlen size in bytes of the address structure
 *
 * output parameters
 * @return On success 0 is returned. On error, -1 is returned 
 *****************************************************************************/
osal_ssize_t osal_recvfrom(osal_socket_handle_t sockd, void *buf, size_t len, osal_basetype_t flags,
                           osal_sockaddr_t *src_addr, osal_socklen_t *addrlen);

/****************************************************************************
 * @fn osal_sendmsg
 *
 * @brief used to transmit a message to another socket. 
 *
 * input parameters
 * @param[in] sockd socket descriptor 
 * @param[in] msghdr structure contains message meta data.
 * @param[in] flags argument is formed by ORing one or many socket options 
 *
 * output parameters
 * @return On success return the number of bytes sent on error -1 is returned 
 *****************************************************************************/
osal_ssize_t osal_sendmsg(osal_socket_handle_t sockd, const struct msghdr msg, osal_basetype_t flags);

/****************************************************************************
 * @fn osal_bind
 *
 * @brief bind a name to a socket. 
 *
 * input parameters
 * @param[in] sockd socket descriptor 
 * @param[in] addr address to be binded
 * @param[in] addrlen address length
 *
 * output parameters
 * @return On success 0 is returned. On error, -1 is returned 
 *****************************************************************************/
osal_basetype_t osal_bind(osal_socket_handle_t sockd, osal_sockaddr_t *addr, osal_socklen_t addrlen);

/****************************************************************************
 * @fn osal_sendto
 *
 * @brief used to transmit a message to another socket. 
 *
 * input parameters
 * @param[in] sockd socket descriptor 
 * @param[in] buf message buffer
 * @param[in] len data buffer length
 * @param[in] flags argument is formed by ORing one or many socket options 
 * @param[in] dest_addr destination address 
 * @param[in] addrlen address length
 *
 * output parameters
 * @return On success return the number of bytes sent on error -1 is returned 
 *****************************************************************************/
osal_ssize_t osal_sendto(osal_socket_handle_t sockd, const void *buf, size_t len, osal_basetype_t flags,
                         const osal_sockaddr_t *dest_addr, osal_socklen_t addrlen);

/****************************************************************************
 * @fn osal_inet_pton
 *
 * @brief This function converts the character string src into a network 
 *        address structure in the af address family 
 *
 * input parameters
 * @param[in] af address family 
 * @param[in] src character string 
 * @param[out] dst network address structure
 *
 * output parameters
 * @return 1 on success
 *         0 if src does not contain a character string representing a valid
 *           network address in the specified address family
 *        -1 if af does not contain a valid address family
 *****************************************************************************/
osal_basetype_t osal_inet_pton(osal_basetype_t af, const char *src, void *dst);

/****************************************************************************
 * @fn osal_select
 *
 * @brief Allows a program to monitor multiple socket descriptors, waiting until
 *        one or more of the socket descriptors become "ready" for some class of 
 *        I/O operation.
 *        Currently only used for blocking and reading socket descriptors.
 *
 * input parameters
 * @param[in] nsds This argument should be set to the highest-numbered socket 
 *                 descriptor plus one
 * @param[in] readsds The socket descriptors in this set are watched to see if 
 *                    they are ready for reading 
 * @param[in] writesds The socket descriptors in this set are watched to see 
 *                     if they are ready for writing.
 * @param[in] exceptsds The socket descriptors in this set are watched for 
 *                      "exceptional conditions". 
 * @param[in] timeout The interval that osal_select() should block waiting 
 *                    for a socket descriptor to become ready
 *
 * output parameters
 * @return on success returns the number of socket descriptors contained in the 
 *         three returned descriptor sets 
 *         0 if timeout expired before any socket descriptors became ready. 
 *        -1 on error
 *****************************************************************************/
osal_basetype_t osal_select(osal_basetype_t nsds, osal_sd_set_t *readsds, osal_sd_set_t *writesds,
                            osal_sd_set_t *exceptsds, struct timeval *timeout);

/****************************************************************************
 * @fn osal_sd_zero 
 *
 * @brief Removes all socket descriptors from set
 *
 * input parameters
 * @param[in] set socket descriptor set
 * 
 * output parameters
 * @return none
 *****************************************************************************/
void osal_sd_zero(osal_sd_set_t *set);

/****************************************************************************
 * @fn osal_sd_set 
 *
 * @brief Adds the socket descriptor sd to set
 *
 * input parameters
 * @param[in] sd socket descriptor 
 * @param[in] set socket descriptor set
 * 
 * output parameters
 * @return none
 *****************************************************************************/
void osal_sd_set(osal_socket_handle_t sd, osal_sd_set_t *set);

/****************************************************************************
 * @fn osal_sd_isset 
 *
 * @brief Checks whether the socket descriptor sd is present in set
 *
 * input parameters
 * @param[in] sd socket descriptor 
 * @param[in] set socket descriptor set
 * 
 * output parameters
 * @return returns nonzero if the socket descriptor sd is present in set, 
 *         and zero if it is not 
 *****************************************************************************/
osal_basetype_t osal_sd_isset(osal_socket_handle_t sd, osal_sd_set_t *set);

/****************************************************************************
 * @fn osal_update_sockaddr 
 *
 * @brief update the IPv6 socket address structure
 *
 * input parameters
 * @param[in] listen_addr address of sockaddr structure 
 * @param[in] sport source port
 * 
 * output parameters
 * @return none
 *****************************************************************************/
void osal_update_sockaddr(osal_sockaddr_t *listen_addr, uint16_t sport);

/****************************************************************************
 * @fn osal_gettime
 *
 * @brief get time as well as a timezone 
 *
 * input parameters
 * @param[in] tv is a struct timeval 
 * @param[in] tz is a struct timezone
 * 
 * output parameters
 * @return 0 for success, or -1 for failure 
 *****************************************************************************/
osal_basetype_t osal_gettime(struct timeval *tv, struct timezone *tz);

/****************************************************************************
 * @fn osal_settime
 *
 * @brief set time as well as a timezone 
 *
 * input parameters
 * @param[in] tv is a struct timeval 
 * @param[in] tz is a struct timezone
 * 
 * output parameters
 * @return 0 for success, or -1 for failure 
 *****************************************************************************/
osal_basetype_t osal_settime(struct timeval *tv, struct timezone *tz);

/****************************************************************************
 * @fn osal_signal 
 *
 * @brief sets the disposition of the signal signum to handler
 *
 * input parameters
 * @param[in] signum is delivered to the process 
 * @param[in] handler programmer-de‐fined function (a "signal handler")
 * 
 * output parameters
 * @return returns the previous value of the signal handler, or SIG_ERR on error 
 *****************************************************************************/
osal_sighandler_t osal_signal(osal_basetype_t signum, osal_sighandler_t handler);

/****************************************************************************
 * @fn osal_sigprocmask 
 *
 * @brief fetch and/or change the signal mask of the calling thread 
 *
 * input parameters
 * @param[in] how define behavior of the call SIG_BLOCK, SIG_UNBLOCK, SIG_SETMASK
 * @param[in] set set of signals
 * @param[in] oldset the previous value of the signal mask
 * 
 * output parameters
 * @return returns 0 on success and -1 on error 
 *****************************************************************************/
osal_basetype_t osal_sigprocmask(osal_basetype_t how, const osal_sigset_t *set, osal_sigset_t *oldset);

/****************************************************************************
 * @fn osal_sigemptyset
 *
 * @brief initializes the signal set given by set to empty, 
 *        with all signals excluded from the set 
 *
 * input parameters
 * @param[in] set set of signals
 * 
 * output parameters
 * @return returns 0 on success and -1 on error 
 *****************************************************************************/
osal_basetype_t osal_sigemptyset(osal_sigset_t *set);

/****************************************************************************
 * @fn osal_sigaddset
 *
 * @brief add and delete respectively signal signum from set.
 *
 * input parameters
 * @param[in] set set of signals
 * 
 * output parameters
 * @return returns 0 on success and -1 on error 
 *****************************************************************************/
osal_basetype_t osal_sigaddset(osal_sigset_t *set, osal_basetype_t signum);

/****************************************************************************
 * @fn osal_print_formatted_ip
 *
 * @brief print formatted IPv6 address from sockadd for debugging purpose
 *
 * input parameters
 * @param[in] sockadd socket address structure
 * 
 * output parameters
 * @return none 
 *****************************************************************************/
void osal_print_formatted_ip(const osal_sockaddr_t *sockadd);

/****************************************************************************
 * @fn osal_trickle_timer_start
 *
 * @brief starts trickle timer
 *
 * input parameters
 * @param[in] timerid timer identification value
 * @param[in] imin minimum timer value
 * @param[in] imax maximum timer value
 * @param[in] trickle_timer_fired timer fired
 * 
 * output parameters
 * @return none 
 *****************************************************************************/
void osal_trickle_timer_start(osal_timerid_t timerid, uint32_t imin, uint32_t imax, 
                              trickle_timer_fired_t trickle_timer_fired);

/****************************************************************************
 * @fn osal_trickle_timer_stop
 *
 * @brief stop trickle timer
 *
 * input parameters
 * @param[in] timerid timer identification value
 * 
 * output parameters
 * @return none 
 *****************************************************************************/
void osal_trickle_timer_stop(osal_timerid_t timerid);

/****************************************************************************
 * @fn osal_malloc
 *
 * @brief allocate memory
 *
 * input parameters
 * @param[in] size size of memory to be allocated
 * 
 * output parameters
 * @return pointer to allocated memory on success, NULL on failure
 *****************************************************************************/
void *osal_malloc(size_t size);

/****************************************************************************
 * @fn osal_free
 *
 * @brief free memory
 *
 * input parameters
 * @param[in] ptr pointer to memory to be freed
 * 
 * output parameters
 * @return none
 *****************************************************************************/
void osal_free(void *ptr); 

/****************************************************************************
 * @fn   osal_sleep_ms
 *
 * @brief sleep for given time in milliseconds
 *
 * input parameters
 *  @param[in] ms time in milliseconds to sleep
 * 
 * output parameters
 * @return none
 *****************************************************************************/
void osal_sleep_ms(uint64_t ms);

/****************************************************************************
 * @fn   osal_read_firmware
 *
 * @brief read firmware image from storage(file/flash)
 *
 * input parameters
 *  @param[in] slotid indicating RUN/UPLOAD/BACKUP slot
 *  @param[in] pointer to _Csmp_Slothdr slot structure
 *  @param[in] size of _Csmp_Slothdr slot structure
 *
 * output parameters
 * @return returns 0 on success and -1 on error
 *****************************************************************************/
osal_basetype_t osal_read_firmware(uint8_t slotid, void* slot, uint32_t size);

/****************************************************************************
 * @fn   osal_write_firmware
 *
 * @brief write firmware image to storage(file/flash)
 *
 * input parameters
 *  @param[in] slotid indicating RUN/UPLOAD/BACKUP slot
 *  @param[in] pointer to _Csmp_Slothdr slot structure
 *  @param[in] size of _Csmp_Slothdr slot structure
 *
 * output parameters
 * @return returns 0 on success and -1 on error
 *****************************************************************************/
osal_basetype_t osal_write_firmware(uint8_t slotid, void* slot, uint32_t size);

#endif
