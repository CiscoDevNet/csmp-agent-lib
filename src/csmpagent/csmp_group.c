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
#include <string.h>
#include "csmp.h"
#include "csmptlv.h"
#include "csmpagent.h"
#include "CsmpTlvs.pb-c.h"

static uint32_t m_GroupIds[CSMP_GROUP_NUM_TYPES] = {0};
static bool m_bLastMatchValid;

bool checkGroup(const uint8_t *buf, uint32_t len, struct sockaddr_in6 *srcaddr) {
  uint32_t msglen;
  tlvid_t tlvid = {0,GROUP_MATCH_TLVID};
  const uint8_t *pctlv = csmptlv_find(buf,len,tlvid,&msglen);
  int rv;

  if (pctlv) {
    rv = csmpagent_post(tlvid, pctlv, msglen,NULL,0,NULL,0,srcaddr);
    if (rv == 0) {
      return false;
    }
    if (!m_bLastMatchValid) {
      DPRINTF("CsmpServer: POST - Group Match FALSE\n");
      return false;
    }
    DPRINTF("CsmpServer: POST - Group Match TRUE\n");
  }
  else {
    DPRINTF("CsmpServer: POST - Group Match not present\n");
  }

  return true;
}

int csmp_get_groupAssign(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  GroupAssign GroupAssignMsg = GROUP_ASSIGN__INIT;
  uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;
  int i;

  DPRINTF("csmpagent_groupAssign: start working.\n");
  GroupAssignMsg.type_present_case = GROUP_ASSIGN__TYPE_PRESENT_TYPE;
  GroupAssignMsg.id_present_case = GROUP_ASSIGN__ID_PRESENT_ID;

  for (i=1;i < CSMP_GROUP_NUM_TYPES;i++) {
    if (m_GroupIds[i] == 0)
      continue;

    GroupAssignMsg.type = i;
    GroupAssignMsg.id = m_GroupIds[i];

    rv = csmptlv_write(pbuf, len - used, tlvid, (ProtobufCMessage *)&GroupAssignMsg);
    if(rv == 0) {
      DPRINTF("csmpagent_groupAssign: csmptlv_write error!\n");
      return -1;
    }
    pbuf += rv; used += rv;
  }

  DPRINTF("csmpagent_groupAssign: csmptlv_write [%u] bytes to buffer!\n", used);
  return used;
}

int csmp_put_groupAssign(tlvid_t tlvid, const uint8_t *buf, size_t len, uint8_t *out_buf,
                         size_t out_size, size_t *out_len, int32_t tlvindex)
{
  GroupAssign *GroupAssignMsg = NULL;
  tlvid_t tlvid0;
  uint32_t tlvlen;
  const uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;

  DPRINTF("Received POST groupAssign TLV\n");

  rv = csmptlv_readTL(pbuf, len, &tlvid0, &tlvlen);
  if ((rv == 0) || (tlvid.type != GROUP_ASSIGN_TLVID)) {
    return -1;
  }
  pbuf += rv; used += rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&GroupAssignMsg, &group_assign__descriptor);
  if (rv == 0) {
    return -1;
  }
  pbuf += rv; used += rv;

  if (GroupAssignMsg->type_present_case && GroupAssignMsg->id_present_case) {
    switch (GroupAssignMsg->type) {
    case CSMP_GROUP_TYPE_CONF:
    case CSMP_GROUP_TYPE_FW:
      if (m_GroupIds[GroupAssignMsg->type] != GroupAssignMsg->id)
        m_GroupIds[GroupAssignMsg->type] = GroupAssignMsg->id;
      break;
    default:
      break;
    }
    DPRINTF("Processed POST groupAssign TLV with value:[type=%d, id=%d]\n",
             (int)GroupAssignMsg->type,(int)GroupAssignMsg->id);
  }

  csmptlv_free((ProtobufCMessage *)GroupAssignMsg);
  return used;
}

int csmp_put_groupMatch(tlvid_t tlvid, const uint8_t *buf, size_t len, uint8_t *out_buf,
                         size_t out_size, size_t *out_len, int32_t tlvindex)
{
  GroupMatch *GroupMatchMsg = NULL;
  tlvid_t tlvid0;
  uint32_t tlvlen;
  const uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;

  DPRINTF("Received POST groupMatch TLV\n");

  rv = csmptlv_readTL(pbuf,len,&tlvid0,&tlvlen);
  if ((rv == 0) || (tlvid.type != GROUP_MATCH_TLVID)) {
    return -1;
  }
  pbuf += rv; used += rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&GroupMatchMsg, &group_match__descriptor);
  if (rv == 0) {
    return -1;
  }
  pbuf += rv; used += rv;

  if (GroupMatchMsg->type_present_case && GroupMatchMsg->id_present_case) {
    switch (GroupMatchMsg->type) {
    case CSMP_GROUP_TYPE_CONF:
    case CSMP_GROUP_TYPE_FW:
      if (m_GroupIds[GroupMatchMsg->type] == GroupMatchMsg->id) {
        m_bLastMatchValid = true;
        DPRINTF("Matched Group [type=%d, id=%d]\n",(int)GroupMatchMsg->type,(int)GroupMatchMsg->id);
        }
      break;
    default:
      break;
    }
  }

  return used;

}

int csmp_get_groupInfo(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  GroupInfo GroupInfoMsg = GROUP_INFO__INIT;
  uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;
  int i;

  DPRINTF("csmpagent_groupInfo: start working.\n");
  GroupInfoMsg.type_present_case = GROUP_INFO__TYPE_PRESENT_TYPE;
  GroupInfoMsg.id_present_case = GROUP_INFO__ID_PRESENT_ID;

  for (i=1;i < CSMP_GROUP_NUM_TYPES;i++) {
    if (m_GroupIds[i] == 0)
      continue;

    GroupInfoMsg.type = i;
    GroupInfoMsg.id = m_GroupIds[i];

    rv = csmptlv_write(pbuf,len - used,tlvid,(ProtobufCMessage *)&GroupInfoMsg);
    if (rv == 0) {
      return -1;
    }
    pbuf += rv; used += rv;
  }

  DPRINTF("csmpagent_groupInfo: csmptlv_write [%u] bytes to buffer!\n", used);
  return used;
}
