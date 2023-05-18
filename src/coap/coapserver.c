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

void *recv_thread(void* arg);

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
  pthread_create(&recvt_id, NULL, recv_thread, NULL);
  pthread_detach(recvt_id);
  return 0;
}

void *recv_thread(void* arg)
{
  (void)arg; // Disable un-used argument compiler warning.
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

  if ( (len - buf_used) < (uint16_t)sizeof(coap_header_t) )
    goto short_msg;

  hdr = (coap_header_t*) cur;

  tx_type = (coap_transaction_type_t)((hdr->control >> 4) & 0x3);
  token_length = hdr->control & 0xF;

  if ((len - buf_used) < ((uint16_t)sizeof(coap_header_t) + token_length) || token_length > COAP_MAX_TKL)
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

    if ((uint32_t)(len - buf_used) < option_len)
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
