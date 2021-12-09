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

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "coap.h"
#include "coapserver.h"

enum {
  MAX_PATH_ELEMENTS = 10,
  MAX_QUERY_ELEMENTS = 10
};

static pthread_t recvt_id;
static int m_sockfd = 0;
static bool m_server_opened = false;
static recv_handler_t m_recv_handler = NULL;

void process_datagram(void *data, uint16_t len, struct sockaddr_in6 *from );
void send_internal_response(const struct sockaddr_in6 *from, uint16_t tx_id,
                            uint8_t token_length, uint8_t *token, uint16_t status);

void *recv_thread(void *arg);

int coapserver_stop()
{
  m_server_opened = false;
  pthread_cancel(recvt_id);
  return close(m_sockfd);
}

int coapserver_listen(uint16_t sport, recv_handler_t recv_handler)
{
  int sockfd;
  struct sockaddr_in6 listen_addr;
  int rv;

  if (m_server_opened) {
    DPRINTF("coapserver_listen coapserver was already opened!\n");
    errno = EBUSY;
    return -1;
  }

  if (!recv_handler) {
    DPRINTF("coapserver_listen Invaid recv_handler!\n");
    errno = EINVAL;
    return -1;
  } else {
    m_recv_handler = recv_handler;
  }

  sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    return -1;
  }

  listen_addr.sin6_family = AF_INET6;
  listen_addr.sin6_addr = in6addr_any;
  listen_addr.sin6_port = htons(sport);

  if (bind(sockfd, (const struct sockaddr *)(&listen_addr), sizeof(listen_addr)) < 0) {
    DPRINTF("coapserver_listen bind error!\n");
    close(sockfd);
    return -1;
  }

  DPRINTF("Listening on port %d\n", ntohs(listen_addr.sin6_port));

  m_sockfd = sockfd;
  m_server_opened = true;
  rv = pthread_create(&recvt_id, NULL, recv_thread, NULL);
  pthread_detach(recvt_id);
  return 0;
}

void *recv_thread(void *arg)
{
  DPRINTF("coapserver receive thread is serving now...\n");

  int rv;
  struct sockaddr_in6 from = {0};
  socklen_t socklen = sizeof(struct sockaddr_in6);
  uint8_t data[1024];
  int32_t len;

  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  fd_set readset;
  fd_set tempset;

  FD_ZERO(&readset);
  FD_ZERO(&tempset);
  FD_SET(m_sockfd, &tempset);

  while (1)
  {
    FD_ZERO(&readset);
    readset = tempset;
    rv = select(m_sockfd+1, &readset, NULL, NULL, NULL);

    if (rv < 0) {
    //perror("select");
      continue;
    }

    if (FD_ISSET(m_sockfd, &readset))
    {
      len = recvfrom(m_sockfd, data, sizeof(data), 0, (struct sockaddr *)(&from), &socklen);
      if (len < 0) {
        DPRINTF("coapserver_listen recv_fn recvmsg error!\n");
        continue;
      }

      DPRINTF("coapserver.Socket.recvfrom - Got %u-byte request from [%x:%x:%x:%x:%x:%x:%x:%x%%%u]:%hu\n",
          len,
          ((uint16_t)from.sin6_addr.s6_addr[0] << 8) | from.sin6_addr.s6_addr[1],
          ((uint16_t)from.sin6_addr.s6_addr[2] << 8) | from.sin6_addr.s6_addr[3],
          ((uint16_t)from.sin6_addr.s6_addr[4] << 8) | from.sin6_addr.s6_addr[5],
          ((uint16_t)from.sin6_addr.s6_addr[6] << 8) | from.sin6_addr.s6_addr[7],
          ((uint16_t)from.sin6_addr.s6_addr[8] << 8) | from.sin6_addr.s6_addr[9],
          ((uint16_t)from.sin6_addr.s6_addr[10] << 8) | from.sin6_addr.s6_addr[11],
          ((uint16_t)from.sin6_addr.s6_addr[12] << 8) | from.sin6_addr.s6_addr[13],
          ((uint16_t)from.sin6_addr.s6_addr[14] << 8) | from.sin6_addr.s6_addr[15],
          from.sin6_scope_id, ntohs(from.sin6_port));

      process_datagram(data, len, &from );
      continue;
    }
  }
}

int coapserver_response(const struct sockaddr_in6 *to,
    coap_transaction_type_t tx_type,
    uint16_t tx_id,
    uint8_t token_length, uint8_t *token,
    uint16_t status,
    const void* body, uint16_t body_len)
{
  coap_header_t coap_hdr;
  uint32_t version = 1;
  int rv;
  uint8_t payload_marker = COAP_PAYLOAD_MARKER;

  struct msghdr msg_hdr = {0};
  struct iovec iov[4] = {{0}};

  msg_hdr.msg_name = (struct sockaddr_in6 *)to;
  msg_hdr.msg_namelen = sizeof(struct sockaddr_in6);
  msg_hdr.msg_iov = iov;
  msg_hdr.msg_iovlen = 1;

  // don't encode any options
  coap_hdr.control = ( version << 6 ) | ( tx_type << 4 ) | token_length;
  coap_hdr.code = COAP_RESPONSE_CODE(status);
  coap_hdr.message_id = tx_id;

  iov[0].iov_base = &coap_hdr;
  iov[0].iov_len = sizeof(coap_hdr);
  if (token_length) {
    iov[1].iov_base = token;
    iov[1].iov_len = token_length;
    msg_hdr.msg_iovlen++;

    if ( body && body_len ) {
      iov[2].iov_base = &payload_marker;
      iov[2].iov_len = 1;

      iov[3].iov_base = (void *) body;
      iov[3].iov_len = body_len;
      msg_hdr.msg_iovlen += 2;
    }
  } else {
    if ( body && body_len ) {
      iov[1].iov_base = &payload_marker;
      iov[1].iov_len = 1;

      iov[2].iov_base = (void *) body;
      iov[2].iov_len = body_len;
      msg_hdr.msg_iovlen += 2;
    }
  }

  DPRINTF("coapserver.response - Sending %d-byte response to [%x:%x:%x:%x:%x:%x:%x:%x%%%u]:%hu\n",
      (int)(iov[0].iov_len + iov[1].iov_len + iov[2].iov_len + iov[3].iov_len),
      ((uint16_t)to->sin6_addr.s6_addr[0] << 8) | to->sin6_addr.s6_addr[1],
      ((uint16_t)to->sin6_addr.s6_addr[2] << 8) | to->sin6_addr.s6_addr[3],
      ((uint16_t)to->sin6_addr.s6_addr[4] << 8) | to->sin6_addr.s6_addr[5],
      ((uint16_t)to->sin6_addr.s6_addr[6] << 8) | to->sin6_addr.s6_addr[7],
      ((uint16_t)to->sin6_addr.s6_addr[8] << 8) | to->sin6_addr.s6_addr[9],
      ((uint16_t)to->sin6_addr.s6_addr[10] << 8) | to->sin6_addr.s6_addr[11],
      ((uint16_t)to->sin6_addr.s6_addr[12] << 8) | to->sin6_addr.s6_addr[13],
      ((uint16_t)to->sin6_addr.s6_addr[14] << 8) | to->sin6_addr.s6_addr[15],
      to->sin6_scope_id,ntohs(to->sin6_port));

  rv = sendmsg(m_sockfd, &msg_hdr, 0);
  if (rv < 0) {
    return -1;
  }

  return 0;
}

void process_datagram(void *data, uint16_t len, struct sockaddr_in6 *from )
{
  uint8_t* cur = data;
  uint16_t buf_used = 0;
  coap_header_t *hdr;

  coap_transaction_type_t tx_type = COAP_CON;
  uint16_t tx_id = 0;
  coap_method_t method;

  uint8_t token_length = 0;
  uint8_t token[COAP_MAX_TKL] = {0};

  uint32_t option_code;
  uint32_t previous_delta = 0;
  uint32_t option_delta, option_len;

  coap_uri_seg_t path[MAX_PATH_ELEMENTS] = {{0}};
  uint32_t path_seg_cnt = 0;
  //char* path_ptr = path;

  coap_uri_seg_t query[MAX_QUERY_ELEMENTS] = {{0}};
  uint32_t query_seg_cnt = 0;
  //char* query_ptr = query;

  if ( (len - buf_used) < sizeof(coap_header_t) )
    goto short_msg;

  hdr = (coap_header_t*) cur;

  tx_type = (coap_transaction_type_t)((hdr->control >> 4) & 0x3);
  token_length = hdr->control & 0xF;

  if ((len - buf_used) < (sizeof(coap_header_t) + token_length) || token_length > COAP_MAX_TKL)
    goto short_msg;

  method = (coap_method_t)hdr->code;
  tx_id = hdr->message_id;

  cur += sizeof(coap_header_t); buf_used += sizeof(coap_header_t);

  if (token_length) {
    memcpy(token, cur, token_length);
    cur += token_length; buf_used += token_length;
  }

  while (len - buf_used > 0 && *cur != COAP_PAYLOAD_MARKER) {
    option_delta = (*cur & 0xf0) >> 4;
    option_len = *cur & 0x0f;
    cur++; buf_used++;

    switch (option_delta) {
    case 13:
      if (len - buf_used < 1)
        goto short_msg;
      option_delta = *cur + 13;
      cur++; buf_used++;
      break;
    case 14:
      if (len - buf_used < 2)
        goto short_msg;
      option_delta = (cur[0] << 8 | cur[1]) + 269;
      cur += 2; buf_used += 2;
      break;
    case 15:
      goto short_msg;
    default:
      break;
    }

    switch (option_len) {
    case 13:
      if (len - buf_used < 1)
        goto short_msg;
      option_len = *cur + 13;
      cur++; buf_used++;
      break;
    case 14:
      if (len - buf_used < 2)
        goto short_msg;
      option_len = (cur[0] << 8 | cur[1]) + 269;
      cur += 2; buf_used += 2;
      break;
    case 15:
      goto short_msg;
    default:
      break;
    }

    option_code = option_delta + previous_delta;

    if ((len - buf_used) < option_len)
      goto short_msg;

    switch (option_code) {
    case COAP_URI_PATH:
      if (path_seg_cnt < MAX_PATH_ELEMENTS) {
        path[path_seg_cnt].val = cur;
        path[path_seg_cnt].len = option_len;
        path_seg_cnt++;
      }
      break;
    case COAP_URI_QUERY:
      if (query_seg_cnt < MAX_QUERY_ELEMENTS) {
        query[query_seg_cnt].val = cur;
        query[query_seg_cnt].len = option_len;
        query_seg_cnt++;
      }
      break;
    default:
      break;
    }

    previous_delta = option_code;
    cur += option_len; buf_used += option_len;
  }

  if (len - buf_used > 0) {
    cur++; buf_used++;
    if (len - buf_used == 0)
      goto short_msg;
  }

  m_recv_handler(from, tx_type, tx_id, token_length, token, method,
      path, path_seg_cnt, query, query_seg_cnt,
      cur, len-(cur-(uint8_t *)data));

  return;

short_msg:
  if (tx_type == COAP_CON)
    send_internal_response(from, tx_id, token_length, token, COAP_CODE_BAD_REQ);
}

void send_internal_response(const struct sockaddr_in6 *from, uint16_t tx_id,
                            uint8_t token_length, uint8_t *token, uint16_t status)
{
  coapserver_response(from, COAP_ACK, tx_id, token_length, token, status, NULL, 0);
}
