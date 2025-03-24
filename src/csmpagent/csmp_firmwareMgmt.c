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
extern bool g_downloadbusy;

// TLV65 TRANSFER_REQUEST_TLVID
// GET: supported
// PUT: supported
int csmp_get_transferRequest(tlvid_t tlvid, uint8_t *buf, size_t len,
                             int32_t tlvindex)
{
    DPRINTF("** csmpagent_firmwaremgmt: GET for TLV %d.\n", tlvid.type);
   
    (void)tlvindex;
    size_t rv = 0;
    uint32_t num;

    TransferRequest TransferRequestMsg = TRANSFER_REQUEST__INIT;
    HardwareInfo HardwareInfoMsg = HARDWARE_INFO__INIT;
    Transfer_Request *xfer_req = NULL;
    
    xfer_req = g_csmptlvs_get(tlvid, &num);

    if (xfer_req == NULL) {
      DPRINTF("csmpagent_firmwaremgmt: csmptlv %d GET failed!\n", tlvid.type);
      return CSMP_OP_TLV_RD_EMPTY;
    }

    if(xfer_req) {
        // Firmware status check
        if (!xfer_req->has_status || xfer_req->status != FWHDR_STATUS_DOWNLOAD) {
          DPRINTF("csmpagent_firmwaremgmt: Transfer request status error! (%x)\n", 
                  xfer_req->status);  
          return CSMP_OP_TLV_RD_EMPTY;
        }
        DPRINTF("Transfer Request: GET\n");
        DPRINTF("has_status=%d, status=%x\n", xfer_req->has_status, xfer_req->status);

        // Hardware Info
        if(xfer_req->has_hwinfo) {
            TransferRequestMsg.hwinfo = &HardwareInfoMsg;
            if(xfer_req->hwinfo.has_hwid) {
                HardwareInfoMsg.hw_id_present_case = HARDWARE_INFO__HW_ID_PRESENT_HW_ID;
                HardwareInfoMsg.hwid = xfer_req->hwinfo.hwid;
                DPRINTF("hwid=%s ", xfer_req->hwinfo.hwid);
            }
            if(xfer_req->hwinfo.has_vendorhwid) {
                HardwareInfoMsg.vendor_hw_id_present_case = HARDWARE_INFO__VENDOR_HW_ID_PRESENT_VENDOR_HW_ID;
                HardwareInfoMsg.vendorhwid = xfer_req->hwinfo.vendorhwid;
                DPRINTF("vendorhwid=%s", xfer_req->hwinfo.vendorhwid);
            }
        }
        // File details
        if(xfer_req->has_filehash) {
            TransferRequestMsg.file_hash_present_case = TRANSFER_REQUEST__FILE_HASH_PRESENT_FILE_HASH;
            TransferRequestMsg.filehash.len = xfer_req->filehash.len;
            TransferRequestMsg.filehash.data = xfer_req->filehash.data;
            DPRINTF("filehash=[len=%lu, data=(0x%02x,0x%02x,0x%02x...0x%02x,0x%02x,0x%02x)]\n",
                    xfer_req->filehash.len,
                    xfer_req->filehash.data[0],
                    xfer_req->filehash.data[1],
                    xfer_req->filehash.data[2],
                    xfer_req->filehash.data[17],
                    xfer_req->filehash.data[18],
                    xfer_req->filehash.data[19]);
        }
        if(xfer_req->has_filename) {
            TransferRequestMsg.file_name_present_case = TRANSFER_REQUEST__FILE_NAME_PRESENT_FILE_NAME;
            TransferRequestMsg.filename = xfer_req->filename;
            DPRINTF("filename=%s\n", xfer_req->filename);
        }
        if(xfer_req->has_version) {
            TransferRequestMsg.version_present_case = TRANSFER_REQUEST__VERSION_PRESENT_VERSION;
            TransferRequestMsg.version = xfer_req->version;
            DPRINTF("version=%s\n", xfer_req->version);
        }
        if(xfer_req->has_filesize) {
            TransferRequestMsg.file_size_present_case = TRANSFER_REQUEST__FILE_SIZE_PRESENT_FILE_SIZE;
            TransferRequestMsg.filesize = xfer_req->filesize;
            DPRINTF("filesize=%u\n", xfer_req->filesize);
        }
        if(xfer_req->has_blocksize) {
            TransferRequestMsg.block_size_present_case = TRANSFER_REQUEST__BLOCK_SIZE_PRESENT_BLOCK_SIZE;
            TransferRequestMsg.blocksize = xfer_req->blocksize;
            DPRINTF("blocksize=%u\n", xfer_req->blocksize);
        }
        // Report interval
        if(xfer_req->has_report_int_min) {
            TransferRequestMsg.report_interval_min_present_case = TRANSFER_REQUEST__REPORT_INTERVAL_MIN_PRESENT_REPORT_INTERVAL_MIN;
            TransferRequestMsg.reportintervalmin = xfer_req->report_int_min;
            DPRINTF("reportintervalMin=%u\n", xfer_req->report_int_min);
        }
        if(xfer_req->has_report_int_max) {
            TransferRequestMsg.report_interval_max_present_case = TRANSFER_REQUEST__REPORT_INTERVAL_MAX_PRESENT_REPORT_INTERVAL_MAX;
            TransferRequestMsg.reportintervalmax = xfer_req->report_int_max;
            DPRINTF("reportintervalMax=%u\n", xfer_req->report_int_max);
        }

        rv = csmptlv_write(buf, len, tlvid, (ProtobufCMessage *)&TransferRequestMsg);
        if (rv == 0) {
            DPRINTF("csmpagent_firmwaremgmt: csmptlv %d write error!\n", tlvid.type);
            return CSMP_OP_TLV_WR_ERROR;
        }
    }

    DPRINTF("csmpagent_firmwaremgmt: csmptlv %d wrote [%ld] bytes to buffer!\n", tlvid.type, rv);
    DPRINTF("** csmpagent_firmwaremgmt: GET for TLV %d done.\n", tlvid.type);
    return rv;
}

int csmp_put_transferRequest(tlvid_t tlvid, const uint8_t *buf, size_t len,
                             uint8_t *out_buf, size_t out_size, size_t *out_len,
                             int32_t tlvindex)
{
    DPRINTF("** csmpagent_firmwaremgmt: POST for TLV %d.\n", tlvid.type);
  
    (void)tlvindex;
    tlvid_t tlvid_in;
    tlvid_t tlvid_out;
    uint32_t tlvlen;
    const uint8_t *pbuf = buf;
    size_t rv;
    int used = 0;
    uint8_t filehash_data[OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE] = {0};

    TransferRequest *TransferRequestMsg = NULL;
    TransferResponse TransferResponseMsg = TRANSFER_RESPONSE__INIT;
    Transfer_Request xfer_req = TRANSFER_REQUEST_INIT;
    TransferResponseMsg.filehash.data = filehash_data;    
    rv = csmptlv_readTL(pbuf, len, &tlvid_in, &tlvlen);
    if ((rv == 0) || (tlvid_in.type != TRANSFER_REQUEST_TLVID)) {
        DPRINTF("csmpagent_firmwaremgmt: csmptlv %d read error!\n", tlvid.type);
        return CSMP_OP_TLV_RD_ERROR;
    }
    pbuf += rv; used += rv;

    rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&TransferRequestMsg, 
                       &transfer_request__descriptor);
    if (rv == 0) {
        DPRINTF("csmpagent_firmwaremgmt: csmptlv %d read error!\n", tlvid.type);
        return CSMP_OP_TLV_RD_ERROR;
    }
    pbuf += rv; used += rv;

    DPRINTF("Transfer Request: POST\n");
    DPRINTF("hwid=%s\n", TransferRequestMsg->hwinfo->hwid);
    DPRINTF("filehash=");
    DPRINTF("[len=%lu, data=(0x%02x,0x%02x,0x%02x...0x%02x,0x%02x,0x%02x)]\n",
            TransferRequestMsg->filehash.len,
            TransferRequestMsg->filehash.data[0],
            TransferRequestMsg->filehash.data[1],
            TransferRequestMsg->filehash.data[2],
            TransferRequestMsg->filehash.data[17],
            TransferRequestMsg->filehash.data[18],
            TransferRequestMsg->filehash.data[19]);
    DPRINTF("filename=%s\n", TransferRequestMsg->filename); 
    DPRINTF("version=%s\n", TransferRequestMsg->version); 
    DPRINTF("filesize=%u\n", TransferRequestMsg->filesize); 
    DPRINTF("blocksize=%u\n", TransferRequestMsg->blocksize); 
    DPRINTF("reportintervalMin(%c)=%u\n", (TransferRequestMsg->report_interval_min_present_case) ?
            'y' : 'n', TransferRequestMsg->reportintervalmin);
    DPRINTF("reportintervalMax(%c)=%u\n", (TransferRequestMsg->report_interval_max_present_case) ?
            'y' : 'n', TransferRequestMsg->reportintervalmax);

    memset(&xfer_req, 0, sizeof(xfer_req));

    // Hardware Info
    if (TransferRequestMsg->hwinfo->hw_id_present_case ||
            TransferRequestMsg->hwinfo->vendor_hw_id_present_case) {
        xfer_req.hwinfo.has_hwid = true;
    }
    if (TransferRequestMsg->hwinfo->hw_id_present_case) {
        xfer_req.has_hwinfo = true;
        memcpy(xfer_req.hwinfo.hwid, TransferRequestMsg->hwinfo->hwid,
               sizeof(xfer_req.hwinfo.hwid));
    }
    if (TransferRequestMsg->hwinfo->vendor_hw_id_present_case) {
        xfer_req.hwinfo.has_vendorhwid = true;
        memcpy(xfer_req.hwinfo.vendorhwid, TransferRequestMsg->hwinfo->vendorhwid,
               sizeof(xfer_req.hwinfo.vendorhwid));
    }
    // File details
    if (TransferRequestMsg->file_hash_present_case) {
        xfer_req.has_filehash = true;
        xfer_req.filehash.len = TransferRequestMsg->filehash.len;
        memcpy(xfer_req.filehash.data, TransferRequestMsg->filehash.data, TransferRequestMsg->filehash.len);
    }
    if (TransferRequestMsg->file_name_present_case) {
        xfer_req.has_filename = true;
        strncpy(xfer_req.filename, TransferRequestMsg->filename, OSAL_CSMP_SLOTHDR_FILE_NAME_SIZE);
    }
    if (TransferRequestMsg->version_present_case) {
        xfer_req.has_version = true;
        strncpy(xfer_req.version, TransferRequestMsg->version, sizeof(xfer_req.version));
    }
    if (TransferRequestMsg->file_size_present_case) {
        xfer_req.has_filesize = true;
        xfer_req.filesize = TransferRequestMsg->filesize;
    }
    if (TransferRequestMsg->block_size_present_case) {
        xfer_req.has_blocksize = true;
        xfer_req.blocksize = TransferRequestMsg->blocksize;
    }
    // Report interval
    if (TransferRequestMsg->report_interval_min_present_case) {
        xfer_req.has_report_int_min = true;
        xfer_req.report_int_min = TransferRequestMsg->reportintervalmin;
    }
    if (TransferRequestMsg->report_interval_max_present_case) {
        xfer_req.has_report_int_max = true;
        xfer_req.report_int_max = TransferRequestMsg->reportintervalmax;
    }

    g_csmptlvs_post(tlvid, &xfer_req);

    if(xfer_req.has_response) {
        switch (xfer_req.response) {
            case RESPONSE_INCOMPATIBLE_HW:
                DPRINTF("csmpagent_firmwaremgmt: Incompatible hardware.\n");
                TransferResponseMsg.response = xfer_req.response;
                break;
            case RESPONSE_INVALID_REQ:
                DPRINTF("csmpagent_firmwaremgmt: Invalid request.\n");
                TransferResponseMsg.response = xfer_req.response;
                break;
            case RESPONSE_FILE_SIZE_TOO_BIG:
                DPRINTF("csmpagent_firmwaremgmt: Transfer request too big.\n");
                TransferResponseMsg.response = xfer_req.response;
                break;
            case RESPONSE_INVALID_BLOCK_SIZE:
                DPRINTF("csmpagent_firmwaremgmt: Transfer blockSize not valid.\n");
                TransferResponseMsg.response = xfer_req.response;
                break;
            case RESPONSE_PENDING_REBOOT:
                DPRINTF("csmpagent_firmwaremgmt: Load pending reboot.\n");
                TransferResponseMsg.response = xfer_req.response;
                break;
            case RESPONSE_DUP_XFER:
                DPRINTF("csmpagent_firmwaremgmt: Duplicate transfer request.\n");
                TransferResponseMsg.response = xfer_req.response;
                break;
            case RESPONSE_MATCH_RUN_XFER:
                DPRINTF("csmpagent_firmwaremgmt: Transfer request matches Run image.\n");
                TransferResponseMsg.response = xfer_req.response;
                break;
            case RESPONSE_MATCH_BAK_XFER:
                DPRINTF("csmpagent_firmwaremgmt: Transfer request matches Backup image.\n");
                TransferResponseMsg.response = xfer_req.response;
                break;
            case RESPONSE_OK:
                TransferResponseMsg.response = xfer_req.response;
                TransferResponseMsg.file_hash_present_case = TRANSFER_RESPONSE__FILE_HASH_PRESENT_FILE_HASH;
                TransferResponseMsg.filehash.len = TransferRequestMsg->filehash.len;
                memcpy(TransferResponseMsg.filehash.data, TransferRequestMsg->filehash.data,
                        TransferRequestMsg->filehash.len);
                DPRINTF("csmpagent_firmwaremgmt: Response Ok, Starting new transfer.\n");
                break;

            default:
                DPRINTF("csmpagent_firmwaremgmt: Invalid request.\n");
                TransferResponseMsg.response = RESPONSE_INVALID_REQ;
        }
        if (TransferResponseMsg.response != RESPONSE_OK) {
            TransferResponseMsg.response_present_case = TRANSFER_RESPONSE__RESPONSE_PRESENT_RESPONSE;

            tlvid_out.vendor = 0;
            tlvid_out.type = TRANSFER_RESPONSE_TLVID;
            rv = csmptlv_write(out_buf, out_size, tlvid_out, (ProtobufCMessage *)&TransferResponseMsg);
            if (rv == 0) {
              DPRINTF("csmpagent_firmwaremgmt: csmptlv %d write error!\n", tlvid.type);
              return CSMP_OP_TLV_WR_ERROR;
            }
            *out_len = rv;
            DPRINTF("csmpagent_firmwaremgmt: csmptlv %d wrote [%ld] bytes to buffer!\n", tlvid_out.type, rv);
        }
    }
    DPRINTF("** csmpagent_firmwaremgmt: POST for TLV %d done.\n", tlvid.type);
    return used;
}

// TLV67 IMAGE_BLOCK_TLVID
// GET: un-supported
// PUT: supported
int csmp_put_imageBlock(tlvid_t tlvid, const uint8_t *buf, size_t len,
                        uint8_t *out_buf, size_t out_size, size_t *out_len,
                        int32_t tlvindex)
{
  DPRINTF("** csmpagent_firmwaremgmt: POST for TLV %d.\n", tlvid.type);

  (void)out_buf; (void)out_size; (void)out_len; (void)tlvindex;
  tlvid_t tlvid_in;
  uint32_t tlvlen;
  const uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;
  
  ImageBlock *CurrentBlkMsg = NULL;
  Image_Block image_blk = IMAGE_BLOCK_INIT;

  rv = csmptlv_readTL(pbuf, len, &tlvid_in, &tlvlen);
  if ((rv == 0) || (tlvid_in.type != IMAGE_BLOCK_TLVID)) {
    DPRINTF("csmpagent_firmwaremgmt: csmptlv %d read error!\n", tlvid.type);
    return CSMP_OP_TLV_RD_ERROR;
  }
  pbuf += rv; used += rv;

  // Check for ongoing downloads
  if (g_downloadbusy == true) {
    DPRINTF("csmpagent_firmwaremgmt: Received image block but busy or not ready.\n");
    rv = tlvlen;
    goto done;
  }
  
  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&CurrentBlkMsg,
                     &image_block__descriptor);
  if (rv == 0) {
    DPRINTF("csmpagent_firmwaremgmt: csmptlv %d read error!\n", tlvid.type);
    csmptlv_free(CurrentBlkMsg);
    return CSMP_OP_TLV_RD_ERROR;
  }

  DPRINTF("Image Block: POST\n");
  DPRINTF("filehash=");
  DPRINTF("[len=%lu, data=(0x%02x,0x%02x,0x%02x...0x%02x,0x%02x,0x%02x)]\n",
          CurrentBlkMsg->filehash.len,
          CurrentBlkMsg->filehash.data[0],
          CurrentBlkMsg->filehash.data[1],
          CurrentBlkMsg->filehash.data[2],
          CurrentBlkMsg->filehash.data[17],
          CurrentBlkMsg->filehash.data[18],
          CurrentBlkMsg->filehash.data[19]);
  DPRINTF("blocknum=%u\n", CurrentBlkMsg->blocknum);
  DPRINTF("blockdata=");
  DPRINTF("[len=%lu, data=(0x%02x...)]\n",
          CurrentBlkMsg->blockdata.len,
          CurrentBlkMsg->blockdata.data[0]);
  DPRINTF("g_downloadbusy(before)=%d\n", g_downloadbusy);

  if (CurrentBlkMsg->file_hash_present_case) {
    g_downloadbusy = true;
    memset(&image_blk, 0, sizeof(image_blk));
    image_blk.has_filehash = true;
    image_blk.filehash.len = (CurrentBlkMsg->filehash.len <= sizeof(image_blk.filehash.data)) ?
                              CurrentBlkMsg->filehash.len : sizeof(image_blk.filehash.data);
    memcpy(image_blk.filehash.data, CurrentBlkMsg->filehash.data, image_blk.filehash.len);

    image_blk.has_blocknum = true;
    image_blk.blocknum = CurrentBlkMsg->blocknum;
    
    image_blk.has_blockdata = true;
    image_blk.blockdata.len = (CurrentBlkMsg->blockdata.len <= sizeof(image_blk.blockdata.data)) ?
                              CurrentBlkMsg->blockdata.len : sizeof(image_blk.blockdata.data);
    memcpy(image_blk.blockdata.data, CurrentBlkMsg->blockdata.data, image_blk.blockdata.len);

    g_csmptlvs_post(tlvid, &image_blk);

    if (image_blk.retval == true) {
      goto done;
    }
  }
  g_downloadbusy = false;
  DPRINTF("csmpagent_firmwaremgmt: Received image block checks failed!\n");

done:
  pbuf += rv; used += rv;
  DPRINTF("g_downloadbusy(after)=%d\n", g_downloadbusy);
  DPRINTF("** csmpagent_firmwaremgmt: POST for TLV %d done.\n", tlvid.type);
  csmptlv_free(CurrentBlkMsg);
  return used;
}

// TLV68 LOAD_REQUEST_TLVID
// GET: supported
// PUT: supported
int csmp_get_loadRequest(tlvid_t tlvid, uint8_t *buf, size_t len, 
                        int32_t tlvindex)
{
  DPRINTF("** csmpagent_firmwaremgmt: GET for TLV %d.\n", tlvid.type);

  (void)tlvindex;
  size_t rv = 0;
  uint32_t num;
  
  LoadRequest LoadRequestMsg = LOAD_REQUEST__INIT;
  Load_Request *lrc = NULL;

  lrc = g_csmptlvs_get(tlvid, &num);

  if(lrc == NULL) {
    DPRINTF("csmpagent_firmwaremgmt: csmptlv %d GET failed!\n", tlvid.type);
    return CSMP_OP_TLV_RD_EMPTY;
  }
  
  DPRINTF("Load Request: GET\n");
  DPRINTF("filehash=");
  DPRINTF("[len=%lu, data=(0x%02x,0x%02x,0x%02x...0x%02x,0x%02x,0x%02x)]\n",
          lrc->filehash.len,
          lrc->filehash.data[0],
          lrc->filehash.data[1],
          lrc->filehash.data[2],
          lrc->filehash.data[17],
          lrc->filehash.data[18],
          lrc->filehash.data[19]);
  DPRINTF("loadtime=%u\n", lrc->loadtime);

  LoadRequestMsg.file_hash_present_case = LOAD_REQUEST__FILE_HASH_PRESENT_FILE_HASH;
  LoadRequestMsg.filehash.len = OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE;
  LoadRequestMsg.filehash.data = lrc->filehash.data;
  LoadRequestMsg.load_time_present_case = LOAD_REQUEST__LOAD_TIME_PRESENT_LOAD_TIME;
  LoadRequestMsg.loadtime = lrc->loadtime;

  rv = csmptlv_write(buf, len, tlvid, (ProtobufCMessage *)&LoadRequestMsg);
  if (rv == 0) {
    DPRINTF("csmpagent_firmwaremgmt: csmptlv %d write error!\n", tlvid.type);
    return CSMP_OP_TLV_WR_ERROR;
  }

  DPRINTF("csmpagent_firmwaremgmt: csmptlv %d write [%ld] bytes to buffer!\n", tlvid.type, rv);
  DPRINTF("** csmpagent_firmwaremgmt: GET for TLV %d done.\n", tlvid.type);
  return rv;
}

int csmp_put_loadRequest(tlvid_t tlvid, const uint8_t *buf, size_t len,
                        uint8_t *out_buf, size_t out_size, size_t *out_len,
                        int32_t tlvindex)
{
  DPRINTF("** csmpagent_firmwaremgmt: POST for TLV %d.\n", tlvid.type);

  (void)tlvindex;
  tlvid_t tlvid_in;
  tlvid_t tlvid_out;
  uint32_t tlvlen;
  const uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;
  uint8_t filehash_data[OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE]={0};
  LoadRequest *LoadRequestMsg = NULL;
  LoadResponse LoadResponseMsg = LOAD_RESPONSE__INIT;
  Load_Request lrc = LOAD_REQUEST_INIT;
  LoadResponseMsg.filehash.data = filehash_data;
  rv = csmptlv_readTL(pbuf, len, &tlvid_in, &tlvlen);
  if ((rv == 0) || (tlvid_in.type != LOAD_REQUEST_TLVID)) {
    DPRINTF("csmpagent_firmwaremgmt: csmptlv %d read error!\n", tlvid.type);
    return CSMP_OP_TLV_RD_ERROR;
  }
  pbuf += rv; used += rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&LoadRequestMsg,
                          &load_request__descriptor);
  if (rv == 0) {
    DPRINTF("csmpagent_firmwaremgmt: csmptlv %d read error!\n", tlvid.type);
    return CSMP_OP_TLV_RD_ERROR;
  }
  pbuf += rv; used += rv;

  DPRINTF("Load Request: PUT\n");
  DPRINTF("filehash=");
  DPRINTF("[len=%lu, data=(0x%02x,0x%02x,0x%02x...0x%02x,0x%02x,0x%02x)]\n",
          LoadRequestMsg->filehash.len,
          LoadRequestMsg->filehash.data[0],
          LoadRequestMsg->filehash.data[1],
          LoadRequestMsg->filehash.data[2],
          LoadRequestMsg->filehash.data[17],
          LoadRequestMsg->filehash.data[18],
          LoadRequestMsg->filehash.data[19]);
  DPRINTF("loadtime=%u\n", (LoadRequestMsg->load_time_present_case) ?
          LoadRequestMsg->loadtime : 0);

  if ((!LoadRequestMsg->file_hash_present_case) ||
      (LoadRequestMsg->filehash.len != OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE)
     ) {
    LoadResponseMsg.response = RESPONSE_INVALID_REQ;
    goto respond;
  }

  LoadResponseMsg.response = RESPONSE_OK;
  LoadResponseMsg.file_hash_present_case = LOAD_RESPONSE__FILE_HASH_PRESENT_FILE_HASH;
  LoadResponseMsg.filehash.len = LoadRequestMsg->filehash.len;
  memcpy(LoadResponseMsg.filehash.data, LoadRequestMsg->filehash.data,
         LoadRequestMsg->filehash.len);

  memset(&lrc, 0, sizeof(lrc));
  lrc.has_filehash = true;
  lrc.filehash.len = (LoadRequestMsg->filehash.len <= sizeof(lrc.filehash.data)) ?
                      LoadRequestMsg->filehash.len : sizeof(lrc.filehash.data);
  memcpy(&lrc.filehash.data, LoadRequestMsg->filehash.data, lrc.filehash.len);
  lrc.loadtime = (LoadRequestMsg->load_time_present_case) ?
                  LoadRequestMsg->loadtime : 0;
  lrc.response = RESPONSE_OK;

  g_csmptlvs_post(tlvid, &lrc);

  LoadResponseMsg.response = lrc.response;
  if (LoadResponseMsg.response == RESPONSE_OK) {
    LoadResponseMsg.load_time_present_case = LOAD_RESPONSE__LOAD_TIME_PRESENT_LOAD_TIME;
    LoadResponseMsg.loadtime = LoadRequestMsg->loadtime;
  }

respond:
  LoadResponseMsg.response_present_case = LOAD_RESPONSE__RESPONSE_PRESENT_RESPONSE;
  
  tlvid_out.vendor = 0;
  tlvid_out.type = LOAD_RESPONSE_TLVID;
  rv = csmptlv_write(out_buf, out_size, tlvid_out, (ProtobufCMessage *)&LoadResponseMsg);
  if (rv == 0) {
    DPRINTF("csmpagent_firmwaremgmt: csmptlv %d write error!\n", tlvid.type);
    return CSMP_OP_TLV_WR_ERROR;
  }

  *out_len = rv;
  DPRINTF("response=%u\n", LoadResponseMsg.response);
  DPRINTF("csmpagent_firmwaremgmt: csmptlv %d wrote [%ld] bytes to buffer!\n", tlvid_out.type, rv);
  DPRINTF("** csmpagent_firmwaremgmt: POST for TLV %d done.\n", tlvid.type);
  return used;
}

// TLV69 CANCEL_LOAD_REQUEST_TLVID
// GET: un-supported
// PUT: supported
int csmp_put_cancelLoadRequest(tlvid_t tlvid, const uint8_t *buf, size_t len,
                        uint8_t *out_buf, size_t out_size, size_t *out_len,
                        int32_t tlvindex)
{
  DPRINTF("** csmpagent_firmwaremgmt: POST for TLV %d.\n", tlvid.type);

  (void)tlvindex;
  tlvid_t tlvid_in;
  tlvid_t tlvid_out;
  uint32_t tlvlen;
  const uint8_t *pbuf = buf;
  size_t rv;
  int used = 0;
  uint8_t filehash_data[OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE] = {0};
  CancelLoadRequest *XLoadRequestMsg = NULL;
  CancelLoadResponse XLoadResponseMsg = CANCEL_LOAD_RESPONSE__INIT;
  Cancel_Load_Request clrc = CANCEL_LOAD_REQUEST_INIT;
  XLoadResponseMsg.filehash.data = filehash_data;
  rv = csmptlv_readTL(pbuf, len, &tlvid_in, &tlvlen);
  if ((rv == 0) || (tlvid_in.type != CANCEL_LOAD_REQUEST_TLVID)) {
    DPRINTF("csmpagent_firmwaremgmt: csmptlv %d read error!\n", tlvid.type);
    return CSMP_OP_TLV_RD_ERROR;
  }
  pbuf += rv; used += rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&XLoadRequestMsg,
                          &cancel_load_request__descriptor);
  if (rv == 0) {
    DPRINTF("csmpagent_firmwaremgmt: csmptlv %d read error!\n", tlvid.type);
    return CSMP_OP_TLV_RD_ERROR;
  }
  pbuf += rv; used += rv;
  
  DPRINTF("Cancel Load Request: PUT\n");
  DPRINTF("filehash=");
  DPRINTF("[len=%lu, data=(0x%02x,0x%02x,0x%02x...0x%02x,0x%02x,0x%02x)]\n",
          XLoadRequestMsg->filehash.len,
          XLoadRequestMsg->filehash.data[0],
          XLoadRequestMsg->filehash.data[1],
          XLoadRequestMsg->filehash.data[2],
          XLoadRequestMsg->filehash.data[17],
          XLoadRequestMsg->filehash.data[18],
          XLoadRequestMsg->filehash.data[19]);

  if (!XLoadRequestMsg->file_hash_present_case) {
    DPRINTF("csmpagent_firmwaremgmt: Received unspecific cancel load request.\n");
    XLoadResponseMsg.response = RESPONSE_INVALID_REQ;
    goto respond;
  }

  if (XLoadRequestMsg->file_hash_present_case &&
      (XLoadRequestMsg->filehash.len != OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE)) {
    DPRINTF("csmpagent_firmwaremgmt: Received invalid cancel load request.\n");
    XLoadResponseMsg.response = RESPONSE_INVALID_REQ;
    goto respond;
  }

  XLoadResponseMsg.response = RESPONSE_OK;
  XLoadResponseMsg.file_hash_present_case = CANCEL_LOAD_RESPONSE__FILE_HASH_PRESENT_FILE_HASH;
  XLoadResponseMsg.filehash.len = XLoadRequestMsg->filehash.len;
  memcpy(XLoadResponseMsg.filehash.data, XLoadRequestMsg->filehash.data, 
         XLoadRequestMsg->filehash.len);

  memset(&clrc, 0, sizeof(clrc));
  clrc.has_filehash = true;
  clrc.filehash.len = (XLoadRequestMsg->filehash.len <= sizeof(clrc.filehash.data)) ? 
                       XLoadRequestMsg->filehash.len : sizeof(clrc.filehash.data);
  memcpy(&clrc.filehash.data, XLoadRequestMsg->filehash.data, clrc.filehash.len);
  clrc.response = RESPONSE_OK;

  g_csmptlvs_post(tlvid, &clrc);

  XLoadResponseMsg.response = clrc.response;

respond:
  XLoadResponseMsg.response_present_case = CANCEL_LOAD_RESPONSE__RESPONSE_PRESENT_RESPONSE;
  
  tlvid_out.vendor = 0;
  tlvid_out.type = CANCEL_LOAD_RESPONSE_TLVID;
  rv = csmptlv_write(out_buf, out_size, tlvid_out, (ProtobufCMessage *)&XLoadResponseMsg);
  if (rv == 0) {
    DPRINTF("csmpagent_firmwaremgmt: csmptlv %d write error!\n", tlvid.type);
    return CSMP_OP_TLV_WR_ERROR;
  }

  *out_len = rv; 
  DPRINTF("response=%u\n", XLoadResponseMsg.response);
  DPRINTF("csmpagent_firmwaremgmt: csmptlv %d wrote [%ld] bytes to buffer!\n", tlvid_out.type, rv);
  DPRINTF("** csmpagent_firmwaremgmt: POST for TLV %d done.\n", tlvid.type);
  return used;
}

// TLV70 SET_BACKUP_REQUEST_TLVID
// GET: un-supported
// PUT: supported
int csmp_put_setBackupRequest(tlvid_t tlvid, const uint8_t *buf, size_t len,
                        uint8_t *out_buf, size_t out_size, size_t *out_len,
                        int32_t tlvindex)
{
    DPRINTF("** csmpagent_firmwaremgmt: POST for TLV %d.\n", tlvid.type);
   
    (void)tlvindex;
    tlvid_t tlvid_in;
    tlvid_t tlvid_out;
    uint32_t tlvlen;
    const uint8_t *pbuf = buf;
    size_t rv;
    int used = 0;
    uint8_t filehash_data[OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE] = {0};
    SetBackupRequest *SetBackupRequestMsg = NULL;
    SetBackupResponse SetBackupResponseMsg = SET_BACKUP_RESPONSE__INIT;
    Set_Backup_Request sbrc = SET_BACKUP_REQUEST_INIT;
    SetBackupResponseMsg.filehash.data = filehash_data;
    rv = csmptlv_readTL(pbuf, len, &tlvid_in, &tlvlen);
    if ((rv == 0) || (tlvid_in.type != SET_BACKUP_REQUEST_TLVID)) {
        DPRINTF("csmpagent_firmwaremgmt: csmptlv %d read error!\n", tlvid.type);
        return CSMP_OP_TLV_RD_ERROR;
    }
    pbuf += rv; used += rv;

    rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&SetBackupRequestMsg, 
                       &set_backup_request__descriptor);
    if (rv == 0) {
        DPRINTF("csmpagent_firmwaremgmt: csmptlv %d read error!\n", tlvid.type);
        return CSMP_OP_TLV_RD_ERROR;
    }
    pbuf += rv; used += rv;

    DPRINTF("Set Backup Request: PUT\n");
    DPRINTF("filehash=");
    DPRINTF("[len=%lu, data=(0x%02x,0x%02x,0x%02x...0x%02x,0x%02x,0x%02x)]\n",
            SetBackupRequestMsg->filehash.len,
            SetBackupRequestMsg->filehash.data[0],
            SetBackupRequestMsg->filehash.data[1],
            SetBackupRequestMsg->filehash.data[2],
            SetBackupRequestMsg->filehash.data[17],
            SetBackupRequestMsg->filehash.data[18],
            SetBackupRequestMsg->filehash.data[19]);

    if ((!SetBackupRequestMsg->file_hash_present_case) || 
        (SetBackupRequestMsg->filehash.len != OSAL_CSMP_SLOTHDR_SHA256_HASH_SIZE)
       ) {
        SetBackupResponseMsg.response = RESPONSE_INVALID_REQ;
        DPRINTF("csmpagent_firmwaremgmt: Received invalid set backup request.\n");
        goto respond;
    }

    SetBackupResponseMsg.response = RESPONSE_OK;
    SetBackupResponseMsg.file_hash_present_case = SET_BACKUP_RESPONSE__FILE_HASH_PRESENT_FILE_HASH;
    SetBackupResponseMsg.filehash.len = SetBackupRequestMsg->filehash.len;
    memcpy(SetBackupResponseMsg.filehash.data, SetBackupRequestMsg->filehash.data,
           SetBackupRequestMsg->filehash.len);
    
    memset(&sbrc, 0, sizeof(sbrc));
    sbrc.has_filehash = true;
    sbrc.filehash.len = (SetBackupRequestMsg->filehash.len <= sizeof(sbrc.filehash.data)) ?
                         SetBackupRequestMsg->filehash.len : sizeof(sbrc.filehash.data);
    memcpy(&sbrc.filehash.data, SetBackupRequestMsg->filehash.data, sbrc.filehash.len);
    sbrc.response = RESPONSE_OK;

    g_csmptlvs_post(tlvid, &sbrc);

    if (sbrc.has_response) {
      SetBackupResponseMsg.response = sbrc.response;
      switch (sbrc.response) {
        case RESPONSE_UNKNOWN_HASH:
          DPRINTF("csmpagent_firmwaremgmt: Received set backup request with unknown hash.\n");
          break;
        case RESPONSE_PENDING_REBOOT:
          DPRINTF("csmpagent_firmwaremgmt: Received set backup request with pending load on backup slot\n");
          break;
      }
    }

respond:
    SetBackupResponseMsg.response_present_case = SET_BACKUP_RESPONSE__RESPONSE_PRESENT_RESPONSE;

    tlvid_out.vendor = 0;
    tlvid_out.type = SET_BACKUP_RESPONSE_TLVID;
    rv = csmptlv_write(out_buf, out_size, tlvid_out, (ProtobufCMessage *)&SetBackupResponseMsg);
    if (rv == 0) {
      DPRINTF("csmpagent_firmwaremgmt: csmptlv %d write error!\n", tlvid.type);
      return CSMP_OP_TLV_WR_ERROR;
    }

    *out_len = rv;
    DPRINTF("response=%u\n", SetBackupResponseMsg.response);
    DPRINTF("csmpagent_firmwaremgmt: csmptlv %d wrote [%ld] bytes to buffer!\n", tlvid_out.type, rv);
    DPRINTF("** csmpagent_firmwaremgmt: POST for TLV %d done.\n", tlvid.type);
    return used;
}

// TLV75 FIRMWARE_IMAGE_INFO_TLVID
// GET: supported
// PUT: un-supported
int csmp_get_firmwareImageInfo(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  DPRINTF("** csmpagent_firmwaremgmt: GET for TLV %d.\n", tlvid.type);
  
  size_t rv = 0, used = 0;
  uint8_t *pbuf = buf;
  uint32_t num;
  int idx = (tlvindex > 0) ? (tlvindex - 1) : 0;

  //Firmware_Image_Info fii[OSAL_CSMP_FWMGMT_ACTIVE_SLOTS] = {FIRMWARE_IMAGE_INFO_INIT};
  Firmware_Image_Info *fii = NULL;

  fii = g_csmptlvs_get(tlvid, &num);

  if (fii == NULL) {
    DPRINTF("csmpagent_firmwaremgmt: csmptlv %d GET failed!\n", tlvid.type);
    return CSMP_OP_TLV_RD_EMPTY;
  }

  while (used < len) {
    FirmwareImageInfo FirmwareImageInfoMsg = FIRMWARE_IMAGE_INFO__INIT;
    HardwareInfo HardwareInfoMsg = HARDWARE_INFO__INIT;

    DPRINTF("Firmware Image Info: GET (tlvindex=%d, slotid=%d)\n", tlvindex, idx);
    DPRINTF("index=%u\n", fii[idx].index);
    DPRINTF("filehash=");
    DPRINTF("[len=%lu, data=(0x%02x,0x%02x,0x%02x...0x%02x,0x%02x,0x%02x)]\n",
            fii[idx].filehash.len,
            fii[idx].filehash.data[0],
            fii[idx].filehash.data[1],
            fii[idx].filehash.data[2],
            fii[idx].filehash.data[17],
            fii[idx].filehash.data[18],
            fii[idx].filehash.data[19]);
    DPRINTF("filename=%s\n", fii[idx].filename);
    DPRINTF("version=%s\n", fii[idx].version);
    DPRINTF("filesize=%u\n", fii[idx].filesize);
    DPRINTF("blocksize=%u\n", fii[idx].blocksize);
    // TBD: Bitmap data
    DPRINTF("bitmap=[len=%lu, data=]\n", fii[idx].bitmap.len);
    DPRINTF("isdefault=%d\n", fii[idx].isdefault);
    DPRINTF("isrunning=%d\n", fii[idx].isrunning);
    DPRINTF("loadtime=%u\n", fii[idx].loadtime);
    DPRINTF("hwid=%s, vendorhwid=%s\n", fii[idx].hwinfo.hwid, fii[idx].hwinfo.vendorhwid);
    DPRINTF("kernelversion=%s, subkernelversion=%s\n", fii[idx].kernelversion,
            fii[idx].subkernelversion);

    if(fii[idx].has_index) {
      FirmwareImageInfoMsg.index_present_case = FIRMWARE_IMAGE_INFO__INDEX_PRESENT_INDEX;
      FirmwareImageInfoMsg.index = fii[idx].index;
    }
    if(fii[idx].has_filehash) {
      FirmwareImageInfoMsg.file_hash_present_case = FIRMWARE_IMAGE_INFO__FILE_HASH_PRESENT_FILE_HASH;
      FirmwareImageInfoMsg.filehash.len = fii[idx].filehash.len;
      FirmwareImageInfoMsg.filehash.data = fii[idx].filehash.data;
    }
    if(fii[idx].has_filename) {
      FirmwareImageInfoMsg.file_name_present_case = FIRMWARE_IMAGE_INFO__FILE_NAME_PRESENT_FILE_NAME;
      FirmwareImageInfoMsg.filename = fii[idx].filename;
    }
    if(fii[idx].has_version) {
      FirmwareImageInfoMsg.version_present_case = FIRMWARE_IMAGE_INFO__VERSION_PRESENT_VERSION;
      FirmwareImageInfoMsg.version = fii[idx].version;
    }
    if(fii[idx].has_filesize) {
      FirmwareImageInfoMsg.file_size_present_case = FIRMWARE_IMAGE_INFO__FILE_SIZE_PRESENT_FILE_SIZE;
      FirmwareImageInfoMsg.filesize = fii[idx].filesize;
    }
    if(fii[idx].has_blocksize) {
      FirmwareImageInfoMsg.block_size_present_case = FIRMWARE_IMAGE_INFO__BLOCK_SIZE_PRESENT_BLOCK_SIZE;
      FirmwareImageInfoMsg.blocksize = fii[idx].blocksize;
    }
    if(fii[idx].has_bitmap) {
      FirmwareImageInfoMsg.bitmap_present_case = FIRMWARE_IMAGE_INFO__BITMAP_PRESENT_BITMAP;
      FirmwareImageInfoMsg.bitmap.len = fii[idx].bitmap.len;
      FirmwareImageInfoMsg.bitmap.data = fii[idx].bitmap.data;
    }
    if(fii[idx].has_isdefault) {
      FirmwareImageInfoMsg.is_default_present_case = FIRMWARE_IMAGE_INFO__IS_DEFAULT_PRESENT_IS_DEFAULT;
      FirmwareImageInfoMsg.isdefault = fii[idx].isdefault;
    }
    if(fii[idx].has_isrunning) {
      FirmwareImageInfoMsg.is_running_present_case = FIRMWARE_IMAGE_INFO__IS_RUNNING_PRESENT_IS_RUNNING;
      FirmwareImageInfoMsg.isrunning = fii[idx].isrunning;
    }
    if(fii[idx].has_loadtime) {
      FirmwareImageInfoMsg.load_time_present_case = FIRMWARE_IMAGE_INFO__LOAD_TIME_PRESENT_LOAD_TIME;
      FirmwareImageInfoMsg.loadtime = fii[idx].loadtime;
    }
    if(fii[idx].has_hwinfo) {
      FirmwareImageInfoMsg.hwinfo = &HardwareInfoMsg;
      if(fii[idx].hwinfo.has_hwid) {
        HardwareInfoMsg.hw_id_present_case = HARDWARE_INFO__HW_ID_PRESENT_HW_ID;
        HardwareInfoMsg.hwid = fii[idx].hwinfo.hwid;
      }
      if(fii[idx].hwinfo.has_vendorhwid) {
        HardwareInfoMsg.vendor_hw_id_present_case = HARDWARE_INFO__VENDOR_HW_ID_PRESENT_VENDOR_HW_ID;
        HardwareInfoMsg.vendorhwid = fii[idx].hwinfo.vendorhwid;
      }
    }
    /*
    if(fii[idx].has_kernelversion) {
      FirmwareImageInfoMsg.kernel_version_present_case = FIRMWARE_IMAGE_INFO__KERNEL_VERSION_PRESENT_KERNEL_VERSION;
      FirmwareImageInfoMsg.kernelversion = fii[idx].kernelversion;
    }
    if(fii[idx].has_subkernelversion) {
      FirmwareImageInfoMsg.sub_kernel_version_present_case = FIRMWARE_IMAGE_INFO__SUB_KERNEL_VERSION_PRESENT_SUB_KERNEL_VERSION;
      FirmwareImageInfoMsg.subkernelversion = fii[idx].subkernelversion;
    }
    */
    rv = csmptlv_write(pbuf, len - used, tlvid, (ProtobufCMessage *)&FirmwareImageInfoMsg);
    DPRINTF("csmpagent_firmwaremgmt: TLV75 id=%d rv=%ld len=%ld used=%ld\n", idx, rv, len, used);
    if (rv == 0) {
      DPRINTF("csmpagent_firmwaremgmt: csmptlv %d write error!\n", tlvid.type);
      return CSMP_OP_TLV_WR_ERROR;
    }
    DPRINTF("csmpagent_firmwaremgmt: csmptlv %d wrote [%ld] bytes to buffer!\n", tlvid.type, rv);
    pbuf += rv;
    used += rv;
    idx++;
    if (tlvindex > 0) {
      DPRINTF("csmpagent_firmwaremgmt: Firmware Image Info tlvindex > 0\n");
      break;
    }
    if (idx >= OSAL_CSMP_FWMGMT_ACTIVE_SLOTS) {
      DPRINTF("csmpagent_firmwaremgmt: Firmware Image Info slotid >= max active slots\n");
      break;
    }
  }

  DPRINTF("csmpagent_firmwaremgmt: csmptlv %d wrote [%ld] bytes to buffer!\n", tlvid.type, used);
  DPRINTF("** csmpagent_firmwaremgmt: GET for TLV %d done.\n", tlvid.type);
  return used;
}

// TLV8 DESCRIPTION_REQUEST_TLVID
// GET: unsupported
// PUT: supported
int csmp_put_descriptionRequest(tlvid_t tlvid, const uint8_t *buf, size_t len,
                             uint8_t *out_buf, size_t out_size, size_t *out_len,
                             int32_t tlvindex)
{
  DescriptionRequest *DescriptionRequestMsg = NULL;
  tlvid_t tlvid0;
  uint32_t tlvlen;
  const uint8_t *pbuf = buf;
  int rv;
  int used = 0;
  long unsigned int i;
  (void) tlvid; // Suppress unused param compiler warning.
  (void) out_buf; // Suppress unused param compiler warning.
  (void) out_size; // Suppress unused param compiler warning.
  (void) out_len; // Suppress unused param compiler warning.
  (void) tlvindex; // Suppress unused param compiler warning.

  DPRINTF("Received POST descriptionRequest TLV8 \n");

  rv = csmptlv_readTL(pbuf, len, &tlvid0, &tlvlen);
  if ((rv == 0) || (tlvid0.type != DESCRIPTION_REQUEST_TLVID)) {
    return -1;
  }
  pbuf += rv; used+=rv;

  rv = csmptlv_readV(pbuf, tlvlen, (ProtobufCMessage **)&DescriptionRequestMsg,&description_request__descriptor);
  if (rv == 0) {
    return -1;
  }
  pbuf += rv; used+=rv;
  
  DPRINTF("DescriptionRequest: n_tlvids=%lu \n",DescriptionRequestMsg->n_tlvid);
  for(i=0; i< DescriptionRequestMsg->n_tlvid; i++){
    tlvid_t tlvData;
    int result;
    result = csmptlv_str2id(DescriptionRequestMsg->tlvid[i],&tlvData);
    DPRINTF("DescrptionRequest: GET e%u.%u \n",tlvData.vendor,tlvData.type);
    if (result == 0)
      continue;
    rv = csmpagent_get(tlvData, out_buf, out_size-(*out_len), tlvindex);
    if (rv < 0) {
      DPRINTF("DescriptionRequest: Unable to write TLV %u.%u\n",tlvData.vendor,tlvData.type);
      if (rv == CSMP_OP_TLV_WR_ERROR) {
        DPRINTF("DescriptionRequest: Current Buffer is insufficient. TLV %u.%u\n",tlvData.vendor,tlvData.type);
        break;
      } else if (rv == CSMP_OP_UNSUPPORTED) {
        DPRINTF("DescriptionRequest: Unsupported TLV %u.%u\n",tlvData.vendor,tlvData.type);
        rv = 0;  // ignore unsupported tlvs
      } else {
        DPRINTF("DescriptionRequest: Unable to get TLV %u.%u\n",tlvData.vendor,tlvData.type);
        rv = 0;
      }
    }
    out_buf += rv; (*out_len)+= rv; 
    
  }

  DPRINTF("Processed POST %s TLV with size=%lu\n", DescriptionRequestMsg->base.descriptor->name, *out_len);

  csmptlv_free((ProtobufCMessage *)DescriptionRequestMsg);
  return used;
}
