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

#include "csmp.h"
#include "csmpinfo.h"
#include "csmptlv.h"
#include "csmpagent.h"
#include "CsmpTlvs.pb-c.h"

int csmp_get_rplInstance(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  size_t rv = 0;
  uint32_t i, num;
  uint8_t *pbuf = buf;
  uint32_t used = 0;

  DPRINTF("csmpagent_rplInstance: start working.\n");


  RPL_Instance *rpl_instance = NULL;
  rpl_instance = g_csmptlvs_get(tlvid, &num);

  if(rpl_instance) {
    for(i = 0; i < num; i++) {
      RPLInstance RPLInstanceMsg = RPLINSTANCE__INIT;

      if(rpl_instance[i].has_instanceindex) {
        RPLInstanceMsg.instance_index_present_case = RPLINSTANCE__INSTANCE_INDEX_PRESENT_INSTANCE_INDEX;
        RPLInstanceMsg.instanceindex = rpl_instance[i].instanceindex;
      }
      if(rpl_instance[i].has_instanceid) {
        RPLInstanceMsg.instance_id_present_case = RPLINSTANCE__INSTANCE_ID_PRESENT_INSTANCE_ID;
        RPLInstanceMsg.instanceid = rpl_instance[i].instanceid;
      }
      if(rpl_instance[i].has_dodagid) {
        RPLInstanceMsg.do_dag_id_present_case = RPLINSTANCE__DO_DAG_ID_PRESENT_DO_DAG_ID;
        RPLInstanceMsg.dodagid.len = rpl_instance[i].dodagid.len;
        RPLInstanceMsg.dodagid.data = rpl_instance[i].dodagid.data;
      }
      if(rpl_instance[i].has_dodagversionnumber) {
        RPLInstanceMsg.do_dag_version_number_present_case = RPLINSTANCE__DO_DAG_VERSION_NUMBER_PRESENT_DO_DAG_VERSION_NUMBER;
        RPLInstanceMsg.dodagversionnumber = rpl_instance[i].dodagversionnumber;
      }
      if(rpl_instance[i].has_rank) {
        RPLInstanceMsg.rank_present_case = RPLINSTANCE__RANK_PRESENT_RANK;
        RPLInstanceMsg.rank = rpl_instance[i].rank;
      }
      if(rpl_instance[i].has_parentcount) {
        RPLInstanceMsg.parent_count_present_case = RPLINSTANCE__PARENT_COUNT_PRESENT_PARENT_COUNT;
        RPLInstanceMsg.parentcount = rpl_instance[i].parentcount;
      }
      if(rpl_instance[i].has_dagsize) {
        RPLInstanceMsg.dag_size_present_case = RPLINSTANCE__DAG_SIZE_PRESENT_DAG_SIZE;
        RPLInstanceMsg.dagsize = rpl_instance[i].dagsize;
      }
      rv = csmptlv_write(pbuf, len-used, tlvid, (ProtobufCMessage *)&RPLInstanceMsg);
      if (rv == 0) {
        DPRINTF("csmpagent_rplInstance: csmptlv_write error!\n");
        return -1;
      }
      pbuf += rv; used += rv;
    }
  }
  DPRINTF("csmpagent_rplInstance: csmptlv_write [%u] bytes to buffer!\n", used);
  return used;
}
