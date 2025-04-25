/*
 *  Copyright 2023-2025 Cisco Systems, Inc.
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

 #include <stdlib.h>
 #include <string.h>
 #include "csmp.h"
 #include "csmpinfo.h"
 #include "osal.h"
 #include "csmptlv.h"
 #include "csmpagent.h"
 #include "CsmpTlvs.pb-c.h"
 
 // TLV32 REBOOT_REQUEST_TLVID
 // GET: unsupported
 // PUT: supported
 
 int csmp_put_rebootRequest(tlvid_t tlvid, const uint8_t *buf, size_t len,
                              uint8_t *out_buf, size_t out_size, size_t *out_len,
                              int32_t tlvindex)
 {
     (void)out_buf; (void)out_size; (void)out_len; (void)tlvindex;
     DPRINTF("** csmpagent_rebootRequest: POST for TLV %d.\n", tlvid.type);
   
     tlvid_t tlvid_in;
     uint32_t tlvlen;
     const uint8_t *pbuf = buf;
     size_t rv;
     int used = 0;
 
     RebootRequest *RebootRequestMsg = NULL;
     Reboot_Request reboot_req = REBOOT_REQUEST_INIT;

     rv = csmptlv_readTL(pbuf, len, &tlvid_in, &tlvlen);
     if ((rv == 0) || (tlvid_in.type != REBOOT_REQUEST_TLVID)) {
         DPRINTF("csmpagent_rebootRequest: csmptlv %d read error!\n", tlvid.type);
         return CSMP_OP_TLV_RD_ERROR;
     }
     pbuf += rv; used += rv;
 
     rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&RebootRequestMsg, 
                        &reboot_request__descriptor);
     if (rv == 0) {
         DPRINTF("csmpagent_rebootRequest: csmptlv %d read error!\n", tlvid.type);
         csmptlv_free((ProtobufCMessage*)RebootRequestMsg);
         return CSMP_OP_TLV_RD_ERROR;
     }
     pbuf += rv; used += rv;
 
     DPRINTF("Reboot Request: POST\n");
     DPRINTF("Reboot flag=%d\n", RebootRequestMsg->flag);
     if(RebootRequestMsg->flag_present_case == REBOOT_REQUEST__FLAG_PRESENT_FLAG){
        reboot_req.has_flag = true;
        reboot_req.flag = RebootRequestMsg->flag;
        switch(RebootRequestMsg->flag){
            case REBOOT:
                g_csmptlvs_post(tlvid, &reboot_req);
            break;
            default:
                csmptlv_free((ProtobufCMessage*)RebootRequestMsg);
                return CSMP_OP_UNSUPPORTED;
        }
     }
     else{
       DPRINTF("csmp_rebootRequest: Reboot request flag not present\n");
     }
     //Never called for other platforms?
     DPRINTF("** csmpagent_rebootRequest: POST for TLV %d done.\n", tlvid.type);
     csmptlv_free((ProtobufCMessage*)RebootRequestMsg);
     return used;
 }
 
 
