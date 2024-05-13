#ifndef __OSAL_COMMON_H
#define __OSAL_COMMON_H

#include "osal.h"


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
 timer_num = 2   /**< max amount of timers */
}timerid_t;

typedef void (*trickle_timer_fired_t) ();

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
                                 void* (*entry) (void * arg),
                                 void * arg);

/****************************************************************************
 * @fn osal_task_cancel
 *
 * @brief function requests that thread be canceled. 
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
 * @brief function atomically both sets the calling thread's cancelability 
 *        state and returns oldstate
 * 
 * input parameters
 * @param[in] type cancelability state to be set
 * @param[out] oldtype old cancelability state to be returned
 *
 * output parameters
 * @return on success return 0 otherwise error value 
 *****************************************************************************/
osal_basetype_t osal_task_setcanceltype(int type, int *oldtype);

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
osal_basetype_t osal_task_sigmask(int how, const sigset_t *set, sigset_t *oldset);

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
osal_basetype_t osal_sem_create(osal_sem * sem, uint16_t value);

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
osal_basetype_t osal_sem_post(osal_sem * sem);

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
osal_basetype_t osal_sem_wait(osal_sem * sem, osal_time_t timeout);

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
osal_basetype_t osal_sem_destroy(osal_sem *sem);

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
 * @param[in] sockfd socket file descriptor 
 * @param[out] buf contains received messages 
 * @param[in] len size of buffer 
 * @param[in] flags argument is formed by ORing one or many socket options 
 * @param[in] src_addr source address 
 * @param[in] addrlen size in bytes of the address structure
 *
 * output parameters
 * @return On success 0 is returned. On error, -1 is returned 
 *****************************************************************************/
osal_ssize_t osal_recvfrom(osal_socket_handle_t sockfd, void *buf, size_t len, osal_basetype_t flags,
                           osal_sockaddr *src_addr, osal_socklen *addrlen);

/****************************************************************************
 * @fn osal_sendmsg
 *
 * @brief used to transmit a message to another socket. 
 *
 * input parameters
 * @param[in] sockfd socket file descriptor 
 * @param[in] msghdr structure contains message meta data.
 * @param[in] flags argument is formed by ORing one or many socket options 
 *
 * output parameters
 * @return On success return the number of bytes sent on error -1 is returned 
 *****************************************************************************/
osal_ssize_t osal_sendmsg(int sockfd, const struct msghdr msg, int flags);

/****************************************************************************
 * @fn osal_bind
 *
 * @brief bind a name to a socket. 
 *
 * input parameters
 * @param[in] osal_sockfd socket file descriptor 
 * @param[in] osal_addr address to be binded
 * @param[in] addrlen address length
 *
 * output parameters
 * @return On success 0 is returned. On error, -1 is returned 
 *****************************************************************************/
osal_basetype_t osal_bind(int osal_sockfd, osal_sockaddr *osal_addr, osal_socklen addrlen);

/****************************************************************************
 * @fn osal_sendto
 *
 * @brief used to transmit a message to another socket. 
 *
 * input parameters
 * @param[in] sockfd socket file descriptor 
 * @param[in] buf message buffer
 * @param[in] len data buffer length
 * @param[in] flags argument is formed by ORing one or many socket options 
 * @param[in] dest_addr destination address 
 * @param[in] addrlen address length
 *
 * output parameters
 * @return On success return the number of bytes sent on error -1 is returned 
 *****************************************************************************/
osal_ssize_t osal_sendto(osal_basetype_t sockfd, const void *buf, size_t len, int flags,
                         const osal_sockaddr *dest_addr, osal_socklen addrlen);

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
osal_basetype_t osal_inet_pton(int af, const char *src, void *dst);

/****************************************************************************
 * @fn osal_select
 *
 * @brief Allows a program to monitor multiple file descriptors, waiting until
 *        one or more of the file descriptors become "ready" for some class of 
 *        I/O operation.
 *        Currently only used for blocking and reading socket file descriptors.
 *
 * input parameters
 * @param[in] nfds This argument should be set to the highest-numbered file 
 *                 descriptor plus one
 * @param[in] readfds The file descriptors in this set are watched to see if 
 *                    they are ready for reading 
 * @param[in] writefds The file descriptors in this set are watched to see 
 *                     if they are ready for writing.
 * @param[in] exceptfds The file descriptors in this set are watched for 
 *                      "exceptional conditions". 
 * @param[in] timeout The interval that osal_select() should block waiting 
 *                    for a file descriptor to become ready
 *
 * output parameters
 * @return on success returns the number of file descriptors contained in the 
 *         three returned descriptor sets 
 *         0 if timeout expired before any file descriptors became ready. 
 *        -1 on error
 *****************************************************************************/
osal_basetype_t osal_select(int nfds, fd_set *readfds, fd_set *writefds,
                            fd_set *exceptfds, struct timeval *timeout);

/****************************************************************************
 * @fn osal_fd_zero 
 *
 * @brief Removes all file descriptors from set
 *
 * input parameters
 * @param[in] set file descriptor set
 * 
 * output parameters
 * @return none
 *****************************************************************************/
void osal_fd_zero(fd_set *set);

/****************************************************************************
 * @fn osal_fd_set 
 *
 * @brief Adds the file descriptor fd to set
 *
 * input parameters
 * @param[in] fd file descriptor 
 * @param[in] set file descriptor set
 * 
 * output parameters
 * @return none
 *****************************************************************************/
void osal_fd_set(int fd, fd_set *set);

/****************************************************************************
 * @fn osal_fd_isset 
 *
 * @brief Checks whether the file descriptor fd is present in set
 *
 * input parameters
 * @param[in] fd file descriptor 
 * @param[in] set file descriptor set
 * 
 * output parameters
 * @return returns nonzero if the file descriptor fd is present in set, 
 *         and zero if it is not 
 *****************************************************************************/
osal_basetype_t osal_fd_isset(int fd, fd_set *set);

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
void osal_update_sockaddr(osal_sockaddr *listen_addr, uint16_t sport);

/****************************************************************************
 * @fn osal_gettimeofday
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
osal_basetype_t osal_gettimeofday(struct timeval *tv, struct timezone *tz);

/****************************************************************************
 * @fn osal_settimeofday
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
osal_sighandler_t osal_signal(int signum, osal_sighandler_t handler);

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
osal_basetype_t osal_sigprocmask(int how, const sigset_t *set, sigset_t *oldset);

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
osal_basetype_t osal_sigemptyset(sigset_t *set);

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
osal_basetype_t osal_sigaddset(sigset_t *set, int signum);

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
void osal_print_formatted_ip(const osal_sockaddr *sockAdd);

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
void osal_trickle_timer_start(timerid_t timerid, uint32_t imin, uint32_t imax, trickle_timer_fired_t trickle_time_fired);

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
void osal_trickle_timer_stop(timerid_t timerid);

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
 * @fn osal_calloc
 *
 * @brief allocate memory and set to zero
 *
 * input parameters
 * @param[in] num number of elements
 * @param[in] size size of memory to be allocated
 * 
 * output parameters
 * @return pointer to allocated memory on success, NULL on failure
 *****************************************************************************/
void *osal_calloc(size_t num, size_t size);

/****************************************************************************
 * @fn osal_realloc
 *
 * @brief reallocate memory
 *
 * input parameters
 * @param[in] ptr pointer to memory to be reallocated
 * @param[in] size size of memory to be allocated
 * 
 * output parameters
 * @return pointer to allocated memory on success, NULL on failure
 *****************************************************************************/
void *osal_realloc(void *ptr, size_t size);

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

#endif