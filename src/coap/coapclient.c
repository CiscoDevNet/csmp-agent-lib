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
#include "coapclient.h"

enum {
  MAX_OPTION_LEN = 128,
};

static response_handler_t m_response_handler = NULL;
static int m_sock = 0;
static bool m_client_opened = false;
static uint16_t m_transaction_id = 0;
static pthread_t recvt_id;

void *recv_fn(void *arg);
int write_option( uint8_t *buf, uint16_t buf_len, coap_option_t this_option, coap_option_t *last_option,
    const uint8_t* option_buf, uint32_t option_len, uint32_t *written_len );
void process_response(uint8_t* data, uint16_t len, struct sockaddr_in6 *from);
void coap_option_map(uint32_t val, uint8_t *map);

int coapclient_stop()
{
  m_client_opened = false;
  pthread_cancel(recvt_id);
  return close(m_sock);
}

int coapclient_open(response_handler_t response_handler)
{
  int rv;
  int sockfd;

  if (m_client_opened) {
    DPRINTF("coaplient was already opened!\n");
    errno = EBUSY;
    return -1;
  }

  m_response_handler = response_handler;

  sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    DPRINTF("CoapClient.open - failed.\n");
    return -1;
  }

  DPRINTF("CoapClient.open - Socket opened.\n");

  m_sock = sockfd;
  m_client_opened = true;
  rv = pthread_create(&recvt_id, NULL, recv_fn, NULL);
  pthread_detach(recvt_id);
  return 0;
}

int coapclient_request (const struct sockaddr_in6 *to,
    coap_transaction_type_t tx_type,
    coap_method_t method,
    uint8_t token_length, uint8_t *token,
    const coap_uri_seg_t *url, uint32_t url_cnt,
    const coap_uri_seg_t *query, uint32_t query_cnt,
    const void *body, uint16_t body_len )
{
  coap_header_t coap_hdr;
  uint8_t opt_buf[MAX_OPTION_LEN], payload_marker = COAP_PAYLOAD_MARKER;
  uint8_t* opt_cur = opt_buf;
  uint32_t opt_buf_used = 0;
  uint32_t version = 1;
  uint32_t option_count = 0;
  uint8_t outbuf[1024];
  uint8_t *outbufp = outbuf;
  int outbuf_len = 0;
  int rv;

  coap_option_t last_option = (coap_option_t)0;
  uint32_t written_len = 0;
  int j = 0;

  if (url) {
    for (j=0; j < url_cnt; j++) {
      rv = write_option(opt_cur, sizeof(opt_buf)-opt_buf_used, COAP_URI_PATH, &last_option,
          (const uint8_t *)url[j].val, url[j].len, &written_len );
      if (rv < 0)
        return -1;
      option_count++;
      opt_cur += written_len; opt_buf_used += written_len;
    }
  }

  if (query) {
    for (j=0; j<query_cnt; j++) {
      rv = write_option( opt_cur, sizeof(opt_buf) - opt_buf_used, COAP_URI_QUERY, &last_option,
          (const uint8_t *)query[j].val, query[j].len, &written_len );
      if (rv < 0)
        return -1;
      option_count++;
      opt_cur += written_len; opt_buf_used += written_len;
    }
  }

  coap_hdr.control = (version << 6) | (tx_type << 4) | token_length;
  coap_hdr.code = method;
  coap_hdr.message_id = htons(m_transaction_id++);
  memcpy(outbufp, &coap_hdr, sizeof(coap_hdr));
  outbuf_len += sizeof(coap_hdr);
  outbufp += sizeof(coap_hdr);

  if (token_length) {
    memcpy(outbufp, token, token_length);
    outbuf_len += token_length;
    outbufp += token_length;
  }

  if (option_count) {
    memcpy(outbufp, opt_buf, opt_buf_used);
    outbuf_len += opt_buf_used;
    outbufp += opt_buf_used;
  }

  if ((body) && (method != COAP_GET)) {
    memcpy(outbufp, &payload_marker, 1);
    outbuf_len += 1;
    outbufp += 1;

    memcpy(outbufp, body, body_len);
    outbuf_len += body_len;
    outbufp += body_len;
  }

  DPRINTF("CoapClient.request - Sending %d-byte request to [%x:%x:%x:%x:%x:%x:%x:%x]:%hu\n",
      outbuf_len,
      ((uint16_t)to->sin6_addr.s6_addr[0] << 8) | to->sin6_addr.s6_addr[1],
      ((uint16_t)to->sin6_addr.s6_addr[2] << 8) | to->sin6_addr.s6_addr[3],
      ((uint16_t)to->sin6_addr.s6_addr[4] << 8) | to->sin6_addr.s6_addr[5],
      ((uint16_t)to->sin6_addr.s6_addr[6] << 8) | to->sin6_addr.s6_addr[7],
      ((uint16_t)to->sin6_addr.s6_addr[8] << 8) | to->sin6_addr.s6_addr[9],
      ((uint16_t)to->sin6_addr.s6_addr[10] << 8) |  to->sin6_addr.s6_addr[11],
      ((uint16_t)to->sin6_addr.s6_addr[12] << 8) |  to->sin6_addr.s6_addr[13],
      ((uint16_t)to->sin6_addr.s6_addr[14] << 8) |  to->sin6_addr.s6_addr[15],
      ntohs(to->sin6_port));

  rv = sendto(m_sock, outbuf, outbuf_len, 0, (const struct sockaddr *)to, sizeof(struct sockaddr_in6));
  if (rv < 0) {
    DPRINTF("CoapClient.request sendto error, errno:%d\n", errno);
    return -1;
  } else {
    DPRINTF("CoapClient.request sendto %d bytes\n", rv);
  }

  return 0;
}

int write_option(uint8_t *buf, uint16_t buf_len, coap_option_t this_option, coap_option_t *last_option,
    const uint8_t* option_buf, uint32_t option_len, uint32_t *written_len)
{
  uint8_t* cur = buf;
  uint8_t delta, len;
  uint32_t option_delta = this_option - *last_option;
  *written_len = 0;

  if (option_len + 1 > buf_len)
    return -1;

  if (option_delta > (0xffff + 269) || option_len > (0xffff + 269))
    return -1;

  coap_option_map(option_delta, &delta);
  coap_option_map(option_len, &len);

  *cur++ = (delta << 4 | len);

  switch (delta) {
  case 13:
    if (option_len + 2 > buf_len)
      return -1;
    *cur++ = option_delta - 13;
    break;
  case 14:
    if (option_len + 3 > buf_len)
      return -1;
    *cur++ = ((option_delta - 269) >> 8) & 0xff;
    *cur++ = (option_delta - 269) & 0xff;
    break;
  default:
    break;
  }

  switch (len) {
  case 13:
    if (option_len + 2 > buf_len)
      return -1;
    *cur++ = option_len - 13;
    break;
  case 14:
    if (option_len + 3 > buf_len)
      return -1;
    *cur++ = ((option_len - 269) >> 8) & 0xff;
    *cur++ = (option_len - 269) & 0xff;
    break;
  default:
    break;
  }

  memcpy( cur, option_buf, option_len );
  cur += option_len;

  *written_len = ( cur - buf );
  *last_option = this_option;

  return 0;
}

void coap_option_map(uint32_t val, uint8_t *map)
{
  if (val < 13)
    *map = val & 0xff;
  else if (val <= 0xff + 13)
    *map = 13;
  else if (val <= 0xffff + 269)
    *map = 14;
}

void *recv_fn(void *arg)
{
  DPRINTF("coapclient receive thread is serving now...\n");

  int rv;
  struct sockaddr_in6 from = {0};
  socklen_t socklen = sizeof(struct sockaddr_in6);
  uint8_t data[1024];
  int16_t len;

  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  fd_set readset;
  fd_set tempset;

  FD_ZERO(&readset);
  FD_ZERO(&tempset);
  FD_SET(m_sock, &tempset);

  while (1)
  {
    FD_ZERO(&readset);
    readset = tempset;
    rv = select(m_sock+1, &readset, NULL, NULL, NULL);

    if (rv < 0) {
    //perror("select");
      continue;
    }

    if (FD_ISSET(m_sock, &readset))
    {
      len = recvfrom(m_sock, data, sizeof(data), 0, (struct sockaddr *)(&from), &socklen);
      if (len < 0) {
        DPRINTF("coapserver_listen recv_fn recvmsg error!\n");
        continue;
      }

      DPRINTF("coapclient.Socket.recvfrom - Got %u-byte response from [%x:%x:%x:%x:%x:%x:%x:%x%%%u]:%hu\n",
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

      process_response(data, len, &from );
      continue;
   }
 }
}

void process_response(uint8_t* data, uint16_t len, struct sockaddr_in6 *from)
{
  uint8_t* cur = data;
  coap_header_t *hdr;
  uint8_t tkl;
  uint8_t status_class;
  uint8_t status_detail;
  uint16_t status, buf_used = 0;
  uint32_t option_delta, option_len;
  if ( (len - buf_used) < sizeof(coap_header_t) )
    return;

  hdr = (coap_header_t*) cur;

  tkl = hdr->control & 0xF;

  if ((len - buf_used) < (sizeof(coap_header_t) + tkl) || tkl > COAP_MAX_TKL)
    return;

  status_class = (hdr->code >> 5);
  status_detail = hdr->code & ((1 << 5) - 1);
  status = ( status_class * 100 ) + status_detail;

  cur += sizeof(coap_header_t); buf_used += sizeof(coap_header_t);

  if (tkl)
    return;

  while (len - buf_used > 0 && *cur != COAP_PAYLOAD_MARKER) {
    option_delta = (*cur & 0xf0) >> 4;
    option_len = *cur & 0x0f;
    cur++; buf_used++;

    switch (option_delta) {
    case 13:
      if (len - buf_used < 1)
        return;
      cur++; buf_used++;
      break;
    case 14:
      if (len - buf_used < 2)
        return;
      cur += 2; buf_used += 2;
      break;
    case 15:
      return;
    default:
      break;
    }

    switch (option_len) {
    case 13:
      if (len - buf_used < 1)
        return;
      option_len = *cur + 13;
      cur++; buf_used++;
      break;
    case 14:
      if (len - buf_used < 2)
        return;
      option_len = (cur[0] << 8 | cur[1]) + 269;
      cur += 2; buf_used += 2;
      break;
    case 15:
      return;
    default:
      break;
    }

    if ((len - buf_used) < option_len)
      return;

    cur += option_len; buf_used += option_len;
  }

  if (len - buf_used > 0) {
    cur++; buf_used++;
    if (len - buf_used == 0)
      return;
  }


  m_response_handler(from, status, cur, len-(cur-data));
}
