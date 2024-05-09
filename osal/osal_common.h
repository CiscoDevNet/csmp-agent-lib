#ifndef __OSAL_COMMON_H
#define __OSAL_COMMON_H

#include "osal.h"

#define OSAL_SUCCESS 0
#define OSAL_FAILURE -1

void osal_print_formatted_ip(const osal_sockaddr *sockAdd);
osal_basetype_t osal_task_create(
   osal_task_t * thread,
   const char * name,
   uint32_t priority,
   size_t stacksize,
   void* (*entry) (void * arg),
   void * arg);
osal_basetype_t osal_task_cancel(osal_task_t thread);
osal_basetype_t osal_task_setcanceltype(int type, int *oldtype);
osal_basetype_t osal_task_sigmask(int how, const sigset_t *set, sigset_t *oldset);
osal_basetype_t osal_sem_create(osal_sem * sem, uint16_t value);
osal_basetype_t osal_sem_post(osal_sem * sem);
osal_basetype_t osal_sem_wait(osal_sem * sem, osal_time_t timeout);
osal_basetype_t osal_sem_destroy(osal_sem *sem);
osal_socket_handle_t osal_socket(osal_basetype_t domain, osal_basetype_t type, osal_basetype_t protocol);
osal_ssize_t osal_recvfrom(osal_socket_handle_t sockfd, void *buf, size_t len, osal_basetype_t flags,
                        osal_sockaddr *src_addr, osal_socklen *addrlen);
osal_ssize_t osal_sendmsg(int sockfd, const struct msghdr msg, int flags);
osal_basetype_t osal_bind(int osal_sockfd, osal_sockaddr *osal_addr, 
	osal_socklen addrlen);

osal_ssize_t osal_sendto(osal_basetype_t sockfd, const void *buf, size_t len, int flags,
                      const osal_sockaddr *dest_addr, osal_socklen addrlen);
osal_basetype_t osal_inet_pton(int af, const char *src, void *dst);
osal_basetype_t osal_select(int nfds, fd_set *readfds, fd_set *writefds,
              fd_set *exceptfds, struct timeval *timeout);
void osal_fd_zero(fd_set *set);
void osal_fd_set(int fd, fd_set *set);
osal_basetype_t osal_fd_isset(int fd, fd_set *set);

void osal_update_sockaddr(osal_sockaddr *listen_addr, uint16_t sport);

osal_basetype_t osal_gettimeofday(struct timeval *tv, struct timezone *tz);
osal_basetype_t osal_settime(struct timeval *tv, struct timezone *tz);
osal_sighandler_t osal_signal(int signum, osal_sighandler_t handler);
osal_basetype_t osal_sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
osal_basetype_t osal_sigemptyset(sigset_t *set);
osal_basetype_t osal_sigaddset(sigset_t *set, int signum);

/* 
 * trickle_timer defines
 */
typedef enum {
 reg_timer = 0,  /**< register timer */
 rpt_timer = 1,  /**< reporting timer */
 timer_num = 2   /**< max amount of timers */
}timerid_t;

typedef void (*trickle_timer_fired_t) ();
void osal_trickle_timer_start(timerid_t timerid, uint32_t imin, uint32_t imax, trickle_timer_fired_t trickle_time_fired);
void osal_trickle_timer_stop(timerid_t timerid);

void *osal_malloc(size_t size);
void *osal_calloc(size_t num, size_t size);
void *osal_realloc(void *ptr, size_t size);
void osal_free(void *ptr); 

#endif