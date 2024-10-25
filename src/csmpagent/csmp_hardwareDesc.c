/*
 *  Copyright 2021-2024 Cisco Systems, Inc.
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
#include "csmpfunction.h"
#include "CsmpTlvs.pb-c.h"

int csmp_get_hardwareDesc(tlvid_t tlvid, uint8_t *buf, size_t len, int32_t tlvindex)
{
  size_t rv = 0;
  uint32_t num;

  (void)tlvindex; // Suppress un-used param warning.

  DPRINTF("** csmpagent_hardwareDesc: GET for TLV %d\n", tlvid.type);

  HardwareDesc HardwareDescMsg = HARDWARE_DESC__INIT;

  Hardware_Desc *hardware_desc = NULL;
  hardware_desc = g_csmptlvs_get(tlvid, &num);

  if(hardware_desc) {
    if(hardware_desc->has_entphysicalindex) {
      HardwareDescMsg.ent_physical_index_present_case = HARDWARE_DESC__ENT_PHYSICAL_INDEX_PRESENT_ENT_PHYSICAL_INDEX;
      HardwareDescMsg.entphysicalindex = hardware_desc->entphysicalindex;
    }
    if(hardware_desc->has_entphysicaldescr) {
      HardwareDescMsg.ent_physical_descr_present_case = HARDWARE_DESC__ENT_PHYSICAL_DESCR_PRESENT_ENT_PHYSICAL_DESCR;
      HardwareDescMsg.entphysicaldescr = hardware_desc->entphysicaldescr;
    }
    if(hardware_desc->has_entphysicalvendortype) {
      HardwareDescMsg.ent_physical_vendor_type_present_case = HARDWARE_DESC__ENT_PHYSICAL_VENDOR_TYPE_PRESENT_ENT_PHYSICAL_VENDOR_TYPE;
      HardwareDescMsg.entphysicalvendortype.len = hardware_desc->entphysicalvendortype.len;
      HardwareDescMsg.entphysicalvendortype.data = hardware_desc->entphysicalvendortype.data;
    }
    if(hardware_desc->has_entphysicalcontainedin) {
      HardwareDescMsg.ent_physical_contained_in_present_case = HARDWARE_DESC__ENT_PHYSICAL_CONTAINED_IN_PRESENT_ENT_PHYSICAL_CONTAINED_IN;
      HardwareDescMsg.entphysicalcontainedin = hardware_desc->entphysicalcontainedin;
    }
    if(hardware_desc->has_entphysicalclass) {
      HardwareDescMsg.ent_physical_class_present_case = HARDWARE_DESC__ENT_PHYSICAL_CLASS_PRESENT_ENT_PHYSICAL_CLASS;
      HardwareDescMsg.entphysicalclass = hardware_desc->entphysicalclass;
    }
    if(hardware_desc->has_entphysicalparentrelpos) {
      HardwareDescMsg.ent_physical_parent_rel_pos_present_case = HARDWARE_DESC__ENT_PHYSICAL_PARENT_REL_POS_PRESENT_ENT_PHYSICAL_PARENT_REL_POS;
      HardwareDescMsg.entphysicalparentrelpos = hardware_desc->entphysicalparentrelpos;
    }
    if(hardware_desc->has_entphysicalname) {
      HardwareDescMsg.ent_physical_name_present_case = HARDWARE_DESC__ENT_PHYSICAL_NAME_PRESENT_ENT_PHYSICAL_NAME;
      HardwareDescMsg.entphysicalname = hardware_desc->entphysicalname;
    }
    if(hardware_desc->has_entphysicalhardwarerev) {
      HardwareDescMsg.ent_physical_hardware_rev_present_case = HARDWARE_DESC__ENT_PHYSICAL_HARDWARE_REV_PRESENT_ENT_PHYSICAL_HARDWARE_REV;
      HardwareDescMsg.entphysicalhardwarerev = hardware_desc->entphysicalhardwarerev;
    }
    if(hardware_desc->has_entphysicalfirmwarerev) {
      HardwareDescMsg.ent_physical_firmware_rev_present_case = HARDWARE_DESC__ENT_PHYSICAL_FIRMWARE_REV_PRESENT_ENT_PHYSICAL_FIRMWARE_REV;
      HardwareDescMsg.entphysicalfirmwarerev = hardware_desc->entphysicalfirmwarerev;
    }
    if(hardware_desc->has_entphysicalsoftwarerev) {
      HardwareDescMsg.ent_physical_software_rev_present_case = HARDWARE_DESC__ENT_PHYSICAL_SOFTWARE_REV_PRESENT_ENT_PHYSICAL_SOFTWARE_REV;
      HardwareDescMsg.entphysicalsoftwarerev = hardware_desc->entphysicalsoftwarerev;
    }
    if(hardware_desc->has_entphysicalserialnum) {
      HardwareDescMsg.ent_physical_serial_num_present_case = HARDWARE_DESC__ENT_PHYSICAL_SERIAL_NUM_PRESENT_ENT_PHYSICAL_SERIAL_NUM;
      HardwareDescMsg.entphysicalserialnum = hardware_desc->entphysicalserialnum;
    }
    if(hardware_desc->has_entphysicalmfgname) {
      HardwareDescMsg.ent_physical_mfg_name_present_case = HARDWARE_DESC__ENT_PHYSICAL_MFG_NAME_PRESENT_ENT_PHYSICAL_MFG_NAME;
      HardwareDescMsg.entphysicalmfgname = hardware_desc->entphysicalmfgname;
    }
    if(hardware_desc->has_entphysicalmodelname) {
      HardwareDescMsg.ent_physical_model_name_present_case = HARDWARE_DESC__ENT_PHYSICAL_MODEL_NAME_PRESENT_ENT_PHYSICAL_MODEL_NAME;
      HardwareDescMsg.entphysicalmodelname = hardware_desc->entphysicalmodelname;
    }
    if(hardware_desc->has_entphysicalassetid) {
      HardwareDescMsg.ent_physical_asset_id_present_case = HARDWARE_DESC__ENT_PHYSICAL_ASSET_ID_PRESENT_ENT_PHYSICAL_ASSET_ID;
      HardwareDescMsg.entphysicalassetid = hardware_desc->entphysicalassetid;
    }
    if(hardware_desc->has_entphysicalmfgdate) {
      HardwareDescMsg.ent_physical_mfg_date_present_case = HARDWARE_DESC__ENT_PHYSICAL_MFG_DATE_PRESENT_ENT_PHYSICAL_MFG_DATE;
      HardwareDescMsg.entphysicalmfgdate = hardware_desc->entphysicalmfgdate;
    }
    if(hardware_desc->has_entphysicaluris) {
      HardwareDescMsg.ent_physical_uris_present_case = HARDWARE_DESC__ENT_PHYSICAL_URIS_PRESENT_ENT_PHYSICAL_URIS;
      HardwareDescMsg.entphysicaluris = hardware_desc->entphysicaluris;
    }
    if(hardware_desc->has_entphysicalfunction) {
      HardwareDescMsg.ent_physical_function_present_case = HARDWARE_DESC__ENT_PHYSICAL_FUNCTION_PRESENT_ENT_PHYSICAL_FUNCTION;
      HardwareDescMsg.entphysicalfunction = hardware_desc->entphysicalfunction;
    }
    if(hardware_desc->has_entphysicaloui) {
      HardwareDescMsg.ent_physical_oui_present_case = HARDWARE_DESC__ENT_PHYSICAL_OUI_PRESENT_ENT_PHYSICAL_OUI;
      HardwareDescMsg.entphysicaloui = hardware_desc->entphysicaloui;
    }

    rv = csmptlv_write(buf, len, tlvid, (ProtobufCMessage *)&HardwareDescMsg);
    if (rv == 0) {
      DPRINTF("csmpagent_hardwareDesc: csmptlv %d write error!\n", tlvid.type);
      return CSMP_OP_TLV_WR_ERROR;
    }
  }

  DPRINTF("csmpagent_hardwareDesc: csmptlv %d wrote [%ld] bytes to buffer!\n", tlvid.type, rv);
  DPRINTF("** csmpagent_hardwareDesc: GET for TLV %d done.\n", tlvid.type);
  return rv;
}
