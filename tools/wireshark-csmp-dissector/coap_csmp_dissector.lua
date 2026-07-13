
---------------------------------------------------------------------------------------------------
--
-- COAP_CSMP_Dissector.lua
--
-- Author: Manojna CSL (mcsl@cisco.com, manojnacsl@gmail.com)
--
-- The CSMP lua dissector is built based on Wireshark's ProtoBuf dissector.
-- This dissector uses the csmp.proto ProtoBuf definitions of the TLVs which must be loaded
-- into Wireshark during startup.  
---------------------------------------------------------------------------------------------------

-- Load required fields and setup
local CSMP_PORT_RFC = '61628'
local CSMP_PORT_CLC = '61624'
fUdpDstPort = Field.new("udp.dstport")
fUdpSrcPort = Field.new("udp.srcport")

-- Create the CSMP Proto and add TLV fields to it
local oProtoCsmp = Proto('csmp', 'CSMP')
local pfTlvId     = ProtoField.uint32('csmp.tlvid',  'TLV ID',     base.DEC)
local pfTlvLength = ProtoField.uint32('csmp.tlvlen', 'TLV Length', base.DEC)
local pfTlvData   = ProtoField.bytes('csmp.tlvdata', 'TLV Raw Data')
oProtoCsmp.fields = {pfTlvId, pfTlvLength, pfTlvData}

-- Lookup table for readable TLV names
local tTlvIdStr = {  [1]='TLV Index',
                     [2]='Device ID',
                     [7]='Session ID',
                     [8]='Description Request',
                    [11]='Hardware Description',
                    [12]='Interface Description',
                    [13]='Report/Subscribe',
                    [16]='IP Address',
                    [17]='IP Route',
                    [18]='Current Time',
                    [22]='Uptime',
                    [23]='Interface Metrics',
                    [25]='IP Route RPL Metrics',
                    [32]='Reboot Request',
                    [35]='WPAN Status',
                    [37]='Radio Link Stats',
                    [43]='CGMS Status',
                    [45]='CGMS Stats',
                    [52]='802.15.4 Neighbors',
                    [53]='RPL Instance',
                    [54]='RPL Parent',
                    [55]='Group Assign',
                    [57]='Group Match',
                    [58]='Group Info',
                    [61]='LoWPAN PHY Stats',
                    [65]='Transfer Request',
                    [67]='Image Block',
                    [68]='Load Request',
                    [69]='Cancel Load Request',
                    [70]='Set Backup Request',
                    [71]='Transfer Response',
                    [72]='Load Response',
                    [73]='Cancel Load Response',
                    [74]='Set Backup Response',
                    [75]='Firmware Image Info',
                    [76]='Signature Validity',
                    [77]='Signature',
                   [172]='Certificate Bundle',
                   [313]='RPL Stats',
                   [340]='Stack Mode Settings',
                   [350]='Power Outage Notification (PON)',
                   [351]='Power Restoration Notification (PRN)' }

-- Add Protobuf message dissector
-- Note, message names in protobufMessageMap must match exactly (case-sensitive) as in .proto file.
-- All message names begin with "csmp." because the .proto file starts with "package csmp;"
local oProtoProtobuf = Dissector.get("protobuf")
local protobufMessageMap = {
    [1]   = "csmp.TlvIndex",
    [2]   = "csmp.DeviceID",
    [7]   = "csmp.SessionID",
    [8]   = "csmp.DescriptionRequest",
    [11]  = "csmp.HardwareDesc",
    [12]  = "csmp.InterfaceDesc",
    [13]  = "csmp.ReportSubscribe",
    [16]  = "csmp.IPAddress",
    [17]  = "csmp.IPRoute",
    [18]  = "csmp.CurrentTime",
    [22]  = "csmp.Uptime",
    [23]  = "csmp.InterfaceMetrics",
    [25]  = "csmp.IPRouteRPLMetrics",
    [32]  = "csmp.RebootRequest",
    [35]  = "csmp.WPANStatus",
    [37]  = "csmp.RadioLinkStats",
    [43]  = "csmp.CGMSStatus",
    [45]  = "csmp.CGMSStats",
    [52]  = "csmp.Neighbor802154G",
    [53]  = "csmp.RplInstance",
    [54]  = "csmp.RPLParent",
    [55]  = "csmp.GroupAssign",
    [57]  = "csmp.GroupMatch",
    [58]  = "csmp.GroupInfo",
    [61]  = "csmp.LowpanPhyStats",
    [65]  = "csmp.TransferRequest",
    [67]  = "csmp.ImageBlock",
    [68]  = "csmp.LoadRequest",
    [69]  = "csmp.CancelLoadRequest",
    [70]  = "csmp.SetBackupRequest",
    [71]  = "csmp.TransferResponse",
    [72]  = "csmp.LoadResponse",
    [73]  = "csmp.CancelLoadResponse",
    [74]  = "csmp.SetBackupResponse",
    [75]  = "csmp.FirmwareImageInfo",
    [76]  = "csmp.SignatureValidity",
    [77]  = "csmp.Signature",
    [172] = "csmp.CertBundle",
    [313] = "csmp.RplStats",
    [340] = "csmp.StackModeSettings",
    [350] = "csmp.PON", -- Power Outage Notification
    [351] = "csmp.PRN", -- Power Restoration Notification
}

-- Get VarInt helper
local function getVarInt(buf)
   local i = 0
   local value = 0
   while( buf(i,1):uint() > 128 ) do
      value = value + ((buf(i, 1):uint()-128) << (7*i))
      i = i + 1
   end
   value = value + (buf(i, 1):uint() << (7*i))
   i = i + 1
   return i, value
end

function oProtoCsmp.dissector(oTvbData, oPinfo, oTreeItemRoot)
   
   -- Do not parse this CoAP payload if not on CSMP port
   if tostring(fUdpDstPort()) ~= CSMP_PORT_RFC and tostring(fUdpSrcPort()) ~= CSMP_PORT_RFC then return end

   --oPinfo.cols.protocol = 'CSMP' -- Comment out and leave column protocol as CoAP/PB
   local oSubtree = oTreeItemRoot:add(oProtoCsmp, oTvbData(), 'CSMP TLV Data')
   local uiIndex = 0

   -- Loop through data and add the TLV(s)
   while( uiIndex < oTvbData:len()-2 ) do
      local typeLen, typeValue = getVarInt( oTvbData(uiIndex) ) -- Get the TLV ID
      local lengthLen, lengthValue = getVarInt( oTvbData(uiIndex+typeLen) ) -- Get the TLV Length
      local dataOffset = uiIndex + typeLen + lengthLen
      local dataBuf = oTvbData(dataOffset, lengthValue) -- Get the TLV Data

      -- Add the TLV subtree
      local oTlvTree = oSubtree:add(oProtoCsmp, oTvbData(uiIndex, typeLen + lengthLen + lengthValue), 
         string.format("TLV ID %d (%s)", typeValue, tTlvIdStr[typeValue] or "Unknown"))

      oTlvTree:add(pfTlvId, oTvbData(uiIndex, typeLen), typeValue)
      oTlvTree:add(pfTlvLength, oTvbData(uiIndex + typeLen, lengthLen), lengthValue)
      oTlvTree:add(pfTlvData, dataBuf)

      -- Attempt to decode with ProtoBuf
      if protobufMessageMap[typeValue] then
         oPinfo.private["pb_msg_type"] = "message," .. protobufMessageMap[typeValue]
         oProtoProtobuf:call(dataBuf:tvb(), oPinfo, oTlvTree)
      end

      uiIndex = dataOffset + lengthValue
   end
end

-- Add dissector to CoAP Data Media Type Dissector Table
DissectorTable.get("media_type"):add("application/octet-stream", oProtoCsmp)

