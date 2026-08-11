---------------------------------------------------------------------------------------------------
--
-- CoAP CSMP Wireshark Dissector v1.7.0
--
-- Wireshark Lua dissector: coap_csmp_dissector.lua
-- TLV ProtoBuf definition: csmp.proto
--
-- * Enables Wireshark to dissect CSMP packet payloads into TLVs and fields based on
--   the TLV Protobuf definitions in csmp.proto file
-- * This CSMP Lua dissector leverages Wireshark's ProtoBuf dissector to implement a
--   extensible, lightweight, easy to upgrade and maintainable dissector model
-- * Refer Readme for install and usage instructions
--
--
-- Author: Manojna CSL (Engineering Tehcnical Lead, Cisco) <mcsl@cisco.com>, <manojnacsl@gmail.com>
--
-- Version history:
-- 1.0.0  Support CoAP RFC (UDP61628), Protobuf decoding
-- 1.1.0  Add selected TLV support, Update csmp.proto
-- 1.2.0  Support CoAP Draft (UDP61624)
-- 1.3.0  Add custom CoAP header decoding
-- 1.4.0  Add CoAP mode differentiation and CoAP header field parsing
-- 1.5.0  Add more TLV support, Update csmp.proto
-- 1.6.0  Use common protobufMessageMap for both TLV names and protobuf names
-- 1.7.0  VarInt overflow handling, CoAP version validation, Option parsing, Token/URI extraction
--
---------------------------------------------------------------------------------------------------

-- Set version and metadata
set_plugin_info({
    version     = "1.7.0",
    author      = "Manojna CSL <mcsl@cisco.com>, <manojnacsl@gmail.com>",
    description = "Wireshark dissector for CSMP: CoAP Simple Management Protocol",
    repository  = "https://github.com/CiscoDevNet/csmp-agent-lib/tree/main/tools/wireshark-csmp-dissector"
})

-- Lookup table for TLV names and Protobuf message dissector
-- Note: TLV message names in protobufMessageMap must match (case-sensitive) as in
-- csmp.proto file and have the package name (csmp) as their common prefix
local oProtoProtobuf = Dissector.get("protobuf")
local protobufMessageMap = {
      [1] = "csmp.TlvIndex",
      [2] = "csmp.DeviceID",
      [6] = "csmp.NMSRedirectRequest",
      [7] = "csmp.SessionID",
      [8] = "csmp.DescriptionRequest",
     [10] = "csmp.HardwareSettings",
     [11] = "csmp.HardwareDesc",
     [12] = "csmp.InterfaceDesc",
     [13] = "csmp.ReportSubscribe",
     [16] = "csmp.IPAddress",
     [17] = "csmp.IPRoute",
     [18] = "csmp.CurrentTime",
     [19] = "csmp.InterfaceSettings",
     [20] = "csmp.WPANSettings",
     [21] = "csmp.RPLSettings",
     [22] = "csmp.Uptime",
     [23] = "csmp.InterfaceMetrics",
     [24] = "csmp.InterfaceDetailMetrics",
     [25] = "csmp.IPRouteRPLMetrics",
     [28] = "csmp.EchoRequest",
     [29] = "csmp.EchoResponse",
     [30] = "csmp.PingRequest",
     [31] = "csmp.PingResponse",
     [32] = "csmp.RebootRequest",
     [33] = "csmp.Ieee8021xStatus",
     [34] = "csmp.Ieee80211iStatus",
     [35] = "csmp.WPANStatus",
     [36] = "csmp.DHCP6ClientStatus",
     [37] = "csmp.RadioLinkStats",
     [38] = "csmp.Ieee802154DevLinkStats",
     [39] = "csmp.Ieee802154LinkStats",
     [40] = "csmp.WPANLinkStats",
     [41] = "csmp.PPPLinkStats",
     [42] = "csmp.CGMSSettings",
     [43] = "csmp.CGMSStatus",
     [44] = "csmp.CGMSNotification",
     [45] = "csmp.CGMSStats",
     [46] = "csmp.PPPSettings",
     [47] = "csmp.Ieee8021xSettings",
     [48] = "csmp.Ieee802154BeaconStats",
     [49] = "csmp.SilenceRequest",
     [50] = "csmp.UDPMetrics",
     [51] = "csmp.IeeeP19012Stats",
     [52] = "csmp.Neighbor802154G",
     [53] = "csmp.RplInstance",
     [54] = "csmp.RPLParent",
     [55] = "csmp.GroupAssign",
     [56] = "csmp.GroupEvict",
     [57] = "csmp.GroupMatch",
     [58] = "csmp.GroupInfo",
     [61] = "csmp.LowpanPhyStats",
     [62] = "csmp.LowpanMacStats",
     [63] = "csmp.LowpanPhySettings",
     [65] = "csmp.TransferRequest",
     [67] = "csmp.ImageBlock",
     [68] = "csmp.LoadRequest",
     [69] = "csmp.CancelLoadRequest",
     [70] = "csmp.SetBackupRequest",
     [71] = "csmp.TransferResponse",
     [72] = "csmp.LoadResponse",
     [73] = "csmp.CancelLoadResponse",
     [74] = "csmp.SetBackupResponse",
     [75] = "csmp.FirmwareImageInfo",
     [76] = "csmp.SignatureValidity",
     [77] = "csmp.Signature",
     [78] = "csmp.SignatureCert",
     [79] = "csmp.SignatureSettings",
     [80] = "csmp.Ieee8021xAAASec",
     [81] = "csmp.Ieee8021xClientSec",
     [84] = "csmp.NetworkNeighbors",
     [86] = "csmp.SysResetStats",
     [88] = "csmp.DiffServMetrics",
     [91] = "csmp.OutageRecovery",
     [92] = "csmp.StartDMSession",
     [93] = "csmp.EndDMSession",
     [95] = "csmp.NeighborP19012",
     [96] = "csmp.SerialDevSettings",
     [97] = "csmp.SerialDevMetrics",
     [98] = "csmp.IPMAddressSettings",
    [107] = "csmp.LedsSettings",
    [108] = "csmp.LedsStatus",
    [111] = "csmp.MAPTStatus",
    [112] = "csmp.MAPTMetrics",
    [115] = "csmp.RawSockForwarderSettings",
    [116] = "csmp.RawSockForwarderStatus",
    [117] = "csmp.RawSockForwarderMetrics",
    [120] = "csmp.NAT44StaticMap",
    [121] = "csmp.NAT44DynamicMap",
    [122] = "csmp.NAT44Interface",
    [124] = "csmp.NetStat",
    [125] = "csmp.ArpCache",
    [130] = "csmp.IeeeP19012RPLMetrics",
    [139] = "csmp.ConsoleSettings",
    [140] = "csmp.ZeroizeNode",
    [141] = "csmp.NetworkRole",
    [142] = "csmp.IOxSettings",
    [143] = "csmp.IOxManagement",
    [144] = "csmp.IOxCredentials",
    [145] = "csmp.IOxHostExec",
    [146] = "csmp.IOxHostStatus",
    [147] = "csmp.IOxStatus",
    [148] = "csmp.DwardRouteQuery",
    [149] = "csmp.DwardRouteNumber",
    [150] = "csmp.AppHeaderInfo",
    [155] = "csmp.LowpanAdaptivePhyStatusQuery",
    [156] = "csmp.AdaptiveModulationSettings",
    [157] = "csmp.AdaptiveModulationStatus",
    [163] = "csmp.ChannelControl",
    [170] = "csmp.DtlsRelaySettings",
    [171] = "csmp.CertReEnrollSettings",
    [172] = "csmp.CertBundle",
    [173] = "csmp.CertAutoRenewSettings",
    [180] = "csmp.PatchCapabilityInfo",
    [200] = "csmp.BBUStatus",
    [201] = "csmp.BBUSettings",
    [202] = "csmp.BBUDiagInfo",
    [214] = "csmp.HeaterSettings",
    [215] = "csmp.HeaterStatus",
    [217] = "csmp.ExtAlarmSetting",
    [218] = "csmp.ExtAlarmStatus",
    [219] = "csmp.StoredInfo",
    [220] = "csmp.FastSyncStatus",
    [240] = "csmp.MplSettings",
    [241] = "csmp.MplStats",
    [242] = "csmp.MplReset",
    [243] = "csmp.MplDomainAddress",
    [301] = "csmp.SwitchPortSettings",
    [302] = "csmp.SwitchPortMetrics",
    [303] = "csmp.SwitchPortDetailMetrics",
    [307] = "csmp.AclDenyEventMsg",
    [308] = "csmp.MACSecSettings",
    [309] = "csmp.MACsecStatus",
    [310] = "csmp.LoggingCapability",
    [311] = "csmp.LoggingStatus",
    [312] = "csmp.LoggingReport",
    [313] = "csmp.RPLStats",
    [314] = "csmp.DHCP6Stats",
    [315] = "csmp.Neighbor802154GSettings",
    [316] = "csmp.NeighborP19012Settings",
    [320] = "csmp.FMROperateTry",
    [321] = "csmp.FMROperateCatch",
    [325] = "csmp.ScanChannelRssiSettings",
    [326] = "csmp.ChannelRssiStats",
    [327] = "csmp.ChannelRssi",
    [328] = "csmp.LinkTestSettings",
    [329] = "csmp.LinkTestStats",
    [330] = "csmp.ChannelPacketStatsClr",
    [331] = "csmp.ChannelPacketStats",
    [332] = "csmp.RadioNeighborStatsClr",
    [333] = "csmp.RadioNeighborStats",
    [334] = "csmp.ChannelFunctionSettings",
    [335] = "csmp.EDFEmodeSettings",
    [336] = "csmp.LowpanMtuSettings",
    [337] = "csmp.DwellIntervalSettings",
    [340] = "csmp.StackModeSettings",
    [341] = "csmp.WirelessConsoleSetting",
    [342] = "csmp.WirelessConsoleAuthorize",
    [343] = "csmp.StackModeTime",
    [344] = "csmp.StackModeResponse",
    [350] = "csmp.PON",
    [351] = "csmp.PRN",
    [352] = "csmp.BackupPowerSetting",
    [353] = "csmp.OutageStats",
    [360] = "csmp.P2pInstanceInfo",
    [361] = "csmp.P2pRouteInfo",
    [362] = "csmp.P2pStats",
    [363] = "csmp.P2pDeleteRoute",
    [364] = "csmp.P2pStartDiscover",
    [500] = "csmp.EventReport",
    [501] = "csmp.EventIndex",
    [502] = "csmp.EventSubscribe",
    [510] = "csmp.EventStats",
    [525] = "csmp.DbgCurrentNodeStatus",
    [540] = "csmp.ETXAlgorithmSettings",
    [550] = "csmp.SnifferSettings",
    [551] = "csmp.SnifferFramecount",
    [600] = "csmp.FFNGatewaySettings",
    [601] = "csmp.FFNChildStats",
    }

-- Load required ports, modes
local COAP_PORT_RFC       = 61628
local COAP_PORT_DRAFT     = 61624
local COAP_MODE_RFC       = "rfc"
local COAP_MODE_DRAFT     = "draft"
local COAP_MODE_AMBIGUOUS = "unknown"

-- Option metadata
  local coapOptionInfo = {
    [1]  = { name = "If-Match",       format = "opaque" },
    [3]  = { name = "Uri-Host",       format = "string" },
    [4]  = { name = "ETag",           format = "opaque" },
    [5]  = { name = "If-None-Match",  format = "empty"  },
    [6]  = { name = "Observe",        format = "uint"   },
    [7]  = { name = "Uri-Port",       format = "uint"   },
    [8]  = { name = "Location-Path",  format = "string" },
    [11] = { name = "Uri-Path",       format = "string" },
    [12] = { name = "Content-Format", format = "uint"   },
    [14] = { name = "Max-Age",        format = "uint"   },
    [15] = { name = "Uri-Query",      format = "string" },
    [16] = { name = "Accept",         format = "uint",  mode = COAP_MODE_DRAFT },
    [17] = { name = "Accept",         format = "uint",  mode = COAP_MODE_RFC   },
    [19] = { name = "Token",          format = "opaque",mode = COAP_MODE_DRAFT },
    [20] = { name = "Location-Query", format = "string" },
    [23] = { name = "Block2",         format = "uint"   },
    [27] = { name = "Block1",         format = "uint"   },
    [28] = { name = "Size2",          format = "uint"   },
    [35] = { name = "Proxy-Uri",      format = "string" },
    [39] = { name = "Proxy-Scheme",   format = "string" },
    [60] = { name = "Size1",          format = "uint"   }
  }

-- Create the CSMP Proto, add CoAP and CSMP TLV fields
local coapTypeNames = {
    [0]="CON",
    [1]="NON",
    [2]="ACK",
    [3]="RST"
  }

local coapCodeNames = {
    [0x00] = "Empty",
    [0x01] = "GET",
    [0x02] = "POST",
    [0x03] = "PUT",
    [0x04] = "DELETE",
    [0x41] = "2.01 Created",
    [0x43] = "2.03 Valid",
    [0x45] = "2.05 Content"
  }

local pfVer       = ProtoField.uint8('csmp.ver', 'Version', base.DEC, nil, 0xC0)
local pfType      = ProtoField.uint8('csmp.type', 'Type', base.DEC,coapTypeNames, 0x30)
local pfTkl       = ProtoField.uint8('csmp.tkl', 'Token Length', base.DEC, nil, 0x0F)
local pfOc        = ProtoField.uint8('csmp.oc', 'Option Count', base.DEC, nil, 0x0F)
local pfCode      = ProtoField.uint8('csmp.code', 'Code', base.DEC, coapCodeNames)
local pfMid       = ProtoField.uint16('csmp.mid', 'Message ID', base.DEC)
local pfTok       = ProtoField.bytes('csmp.token', 'Token')
local pfOptNumber = ProtoField.uint32("csmp.option.number", "Option Number", base.DEC)
local pfOptDelta  = ProtoField.uint32("csmp.option.delta", "Option Delta", base.DEC)
local pfOptDeltaNibble = ProtoField.uint8("csmp.option.delta_nibble", "Option Delta", base.DEC, nil, 0xF0)
local pfOptLength = ProtoField.uint32("csmp.option.length", "Option Length", base.DEC)
local pfOptLengthNibble = ProtoField.uint8("csmp.option.length_nibble", "Option Length", base.DEC, nil, 0x0F)
local pfOptName   = ProtoField.string("csmp.option.name", "Option Name")
local pfOptDescription = ProtoField.string("csmp.option.description",  "Option Desc")
local pfUriPath   = ProtoField.string("csmp.uri_path", "Uri-Path")
local pfOptValue  = ProtoField.bytes("csmp.option.value", "Option Value")
local pfPayloadMarker = ProtoField.uint8("csmp.payload_marker", "Payload Marker", base.HEX)
local pfPayload   = ProtoField.bytes("csmp.payload", "Payload")
local pfTlvId     = ProtoField.uint32('csmp.tlvid', 'ID', base.DEC)
local pfTlvLength = ProtoField.uint32('csmp.tlvlen', 'Length', base.DEC)
local pfTlvData   = ProtoField.bytes('csmp.tlvdata', 'Raw Data')

local oProtoCsmp  = Proto('csmp', 'CSMP: CoAP Simple Management Protocol')

-- Load required fields
oProtoCsmp.fields = {pfVer, pfType, pfTkl, pfOc, pfCode, pfMid, pfTok,
                     pfOptNumber, pfOptDelta, pfOptDeltaNibble, pfOptLength, pfOptLengthNibble,
                     pfOptName, pfOptDescription, pfUriPath, pfOptValue,
                     pfPayloadMarker, pfPayload,
                     pfTlvId, pfTlvLength, pfTlvData}

-- Decode TLV name from Protobuf
local function getTlvName(typeValue)
    local protobufName =
        protobufMessageMap[typeValue]

    if protobufName == nil then
        return "Unknown"
    end

    return protobufName:match("([^%.]+)$")
        or protobufName
end

-- Decode Option field
local function decodeOptionField(tvb, offset, nibble)
    if nibble <= 12 then
        return nibble, offset
    elseif nibble == 13 then
        if offset + 1 > tvb:len() then
            return nil, offset, "Truncated 8-bit extension"
        end

        return 13 + tvb(offset, 1):uint(), offset + 1
    elseif nibble == 14 then
        if offset + 2 > tvb:len() then
            return nil, offset, "Truncated 16-bit extension"
        end

        return 269 + tvb(offset, 2):uint(), offset + 2
    end

    return nil, offset, "Reserved nibble value 15"
end

local function getOptionMetadata(optionNumber, coapMode)
  local metadata = coapOptionInfo[optionNumber]

  if metadata ~= nil
        and (metadata.mode == nil or metadata.mode == coapMode) then
        return metadata
    end

    return {
      name = string.format("Unknown Option %d", optionNumber),
      format = "opaque"
    }
end

-- Decode RFC CoAP Header Options
local function dissectRfcOptions(tvb, offset, subtree, coapMode)
    local tvbLen = tvb:len()
    local previousOptionNumber = 0
    local optionIndex = 0
    local uriPathParts = {}

    while offset < tvbLen do
        local optionStart = offset
        local optionHeader = tvb(offset, 1):uint()

        -- Payload marker is valid only at an option boundary.
        if optionHeader == 0xFF then
            local markerItem = subtree:add(pfPayloadMarker, tvb(offset, 1))

            markerItem:set_text("End of options marker: 255")
            offset = offset + 1

            if #uriPathParts > 0 then
                local uriPath = "/" .. table.concat(uriPathParts, "/")

                subtree:add(pfUriPath, tvb(optionStart, 0), uriPath):set_text(
                        "[Uri-Path: " .. uriPath .. "]")
            end

            if offset >= tvbLen then
                subtree:add_expert_info(PI_MALFORMED, PI_ERROR,
                        "Payload marker is not followed by payload data")
                return nil, false
            end

            local payloadRange = tvb(offset, tvbLen - offset)
            local payloadItem = subtree:add(pfPayload, payloadRange)

            payloadItem:set_text(string.format("Payload, Length: %d", payloadRange:len()))

            return payloadRange, true
        end

        offset = offset + 1

        local deltaNibble = bit.rshift(bit.band(optionHeader, 0xF0), 4)
        local lengthNibble = bit.band(optionHeader, 0x0F)

        local optionDelta, decodeError
        optionDelta, offset, decodeError = decodeOptionField(tvb, offset, deltaNibble)

        if optionDelta == nil then
            subtree:add_expert_info(PI_MALFORMED, PI_ERROR, "Invalid option delta: " .. decodeError)
            return nil, false
        end

        local optionLength
        optionLength, offset, decodeError = decodeOptionField(tvb, offset, lengthNibble)

        if optionLength == nil then
            subtree:add_expert_info(PI_MALFORMED, PI_ERROR, "Invalid option length: " .. decodeError)
            return nil, false
        end

        if offset + optionLength > tvbLen then
            subtree:add_expert_info(PI_MALFORMED, PI_ERROR, "Option value extends beyond packet")
            return nil, false
        end

        optionIndex = optionIndex + 1

        local optionNumber = previousOptionNumber + optionDelta

        previousOptionNumber = optionNumber

        -- local metadata = coapOptionInfo[optionNumber] or {name = "Unknown", format = "opaque"}
        local metadata = getOptionMetadata(optionNumber, coapMode)
        local optionValueRange = tvb(offset, optionLength)
        local displayValue = ""

        if metadata.format == "string" then
            displayValue = optionValueRange:string()
        elseif metadata.format == "uint" then
            if optionLength == 0 then
              displayValue = "0"
            elseif optionLength <= 4 then
              displayValue = tostring(optionValueRange:uint())
            else
              displayValue = tostring(optionValueRange:bytes())
            end
        elseif metadata.format == "empty" then
            displayValue = ""
        else
            displayValue = tostring(optionValueRange:bytes())
        end

        local optionEnd = offset + optionLength
        local optionRange = tvb(optionStart, optionEnd - optionStart)
        local title = string.format("Opt Name: #%d: %s", optionIndex, metadata.name)

        if displayValue ~= "" then
            title = title .. ": " .. displayValue
        end

        --local optionTree = subtree:add(optionRange, title)
        -- Create a filterable option subtree.
        local optionTree = subtree:add(pfOptName, optionRange, metadata.name)

        -- Preserve the Wireshark-style display text.
        optionTree:set_text(title)

        local critical = bit.band(optionNumber, 1) ~= 0 and "Critical" or "Elective"
        local safety = bit.band(optionNumber, 2) ~= 0 and "Unsafe" or "Safe"

        optionTree:add(pfOptDescription, optionRange, string.format(
                   "Type %d, %s, %s", optionNumber, critical, safety))
        optionTree:add(pfOptDeltaNibble, tvb(optionStart, 1))
        optionTree:add(pfOptLengthNibble, tvb(optionStart, 1))
        optionTree:add(pfOptNumber, optionRange, optionNumber)
        optionTree:add(pfOptDelta, optionRange, optionDelta)
        optionTree:add(pfOptLength, optionRange, optionLength)

        if optionLength > 0 then
            optionTree:add(pfOptValue, optionValueRange):set_text(metadata.name .. ": " .. displayValue)
        end

        if optionNumber == 11 then
            uriPathParts[#uriPathParts + 1] = displayValue
        end

        offset = optionEnd
    end

    -- Add URI summary when no payload marker exists.
    if #uriPathParts > 0 then
        local uriPath = "/" .. table.concat(uriPathParts, "/")

        subtree:add(pfUriPath, tvb(tvbLen, 0), uriPath):set_text("[Uri-Path: " .. uriPath .. "]")
    end

    -- Valid message without a payload marker or payload.
    return nil, true
end

-- Decode Draft CoAP Header Options
local function dissectDraftOptions(tvb, offset, subtree, optionCount)
    local tvbLen = tvb:len()
    local previousOptionNumber = 0
    local parsedOptions = 0
    local optionIndex = 0
    local markerMode = optionCount == 15
    local jumpPending = false
    local uriPathParts = {}

    local function fail(message)
        subtree:add_expert_info(
            PI_MALFORMED,
            PI_ERROR,
            message
        )
        return nil, false
    end

    -- Draft length nibble 15 uses repeated extension bytes.
    local function readLength(currentOffset, nibble)
        if nibble < 15 then
            return nibble, currentOffset
        end

        local length = 15

        while true do
            if currentOffset >= tvbLen then
                return nil, currentOffset,
                    "Truncated draft option length"
            end

            local extension =
                tvb(currentOffset, 1):uint()

            currentOffset = currentOffset + 1
            length = length + extension

            if length > 1034 then
                return nil, currentOffset,
                    "Draft option length exceeds 1034 bytes"
            end

            if extension < 255 then
                return length, currentOffset
            end
        end
    end

    -- Delta nibble 15 introduces an option jump.
    local function readJump(currentOffset, jumpType)
        if jumpType == 1 then
            return 15, currentOffset

        elseif jumpType == 2 then
            if currentOffset >= tvbLen then
                return nil, currentOffset,
                    "Truncated one-byte option jump"
            end

            local value =
                tvb(currentOffset, 1):uint()

            return (value + 2) * 8, currentOffset + 1

        elseif jumpType == 3 then
            if currentOffset + 2 > tvbLen then
                return nil, currentOffset,
                    "Truncated two-byte option jump"
            end

            local value =
                tvb(currentOffset, 2):uint()

            return (value + 258) * 8, currentOffset + 2
        end

        return nil, currentOffset,
            "Reserved draft option-jump encoding"
    end

    local function finishPayload(payloadOffset)
        if #uriPathParts > 0 then
            local uriPath = "/" .. table.concat(uriPathParts, "/")

            subtree:add(pfUriPath, tvb(payloadOffset, 0), uriPath):set_text(
                    "[Uri-Path: " .. uriPath .. "]")
        end

        if payloadOffset >= tvbLen then
            return nil, true
        end

        local payloadRange = tvb(payloadOffset, tvbLen - payloadOffset)

        subtree:add(pfPayload, payloadRange):set_text(string.format(
                "Payload, Length: %d", payloadRange:len()))

        return payloadRange, true
    end

    -- OC 0: payload starts immediately after the fixed header.
    if optionCount == 0 then
        return finishPayload(offset)
    end

    while offset < tvbLen do
        local optionStart = offset
        local header = tvb(offset, 1):uint()

        offset = offset + 1

        -- Marker is legal only when OC is 15.
        if header == 0xF0 then
            if not markerMode then
                return fail(
                    "Draft option marker encountered when OC is not 15"
                )
            end

            if jumpPending then
                return fail(
                    "End-of-options marker cannot follow an option jump"
                )
            end

            subtree:add(pfPayloadMarker, tvb(optionStart, 1))
                   :set_text("End of options marker: 240 (0xF0)")

            return finishPayload(offset)
        end

        local deltaNibble = bit.rshift(bit.band(header, 0xF0), 4)
        local lengthNibble = bit.band(header, 0x0F)

        if deltaNibble == 15 then
            if jumpPending then
                return fail(
                    "An option jump cannot follow another option jump"
                )
            end

            local jumpDelta, newOffset, jumpError = readJump(offset, lengthNibble)

            if jumpDelta == nil then
                return fail(jumpError)
            end

            local jumpRange = tvb(optionStart, newOffset - optionStart)
            local jumpTree = subtree:add(jumpRange, string.format(
                             "Option Jump: +%d", jumpDelta))

            jumpTree:add(pfOptDeltaNibble, tvb(optionStart, 1))

            previousOptionNumber = previousOptionNumber + jumpDelta

            offset = newOffset
            jumpPending = true
        else
            local optionLength, valueOffset, lengthError = readLength(offset, lengthNibble)

            if optionLength == nil then
                return fail(lengthError)
            end

            if valueOffset + optionLength > tvbLen then
                return fail(
                    "Draft option value extends beyond packet"
                )
            end

            local optionNumber = previousOptionNumber + deltaNibble

            previousOptionNumber = optionNumber
            jumpPending = false
            parsedOptions = parsedOptions + 1
            optionIndex = optionIndex + 1

            local metadata = getOptionMetadata(optionNumber, COAP_MODE_DRAFT)

            local optionValueRange = tvb(valueOffset, optionLength)

            local displayValue = ""

            if metadata.format == "string" then
                displayValue = optionValueRange:string()

            elseif metadata.format == "uint" then
                if optionLength == 0 then
                    displayValue = "0"
                elseif optionLength <= 4 then
                    displayValue = tostring(optionValueRange:uint())
                else
                    displayValue = tostring(optionValueRange:bytes())
                end

            elseif metadata.format ~= "empty"
                and optionLength > 0 then
                displayValue = tostring(optionValueRange:bytes())
            end

            local optionEnd = valueOffset + optionLength
            local optionRange = tvb(optionStart, optionEnd - optionStart)
            local title = string.format("Opt Name: #%d: %s", optionIndex, metadata.name)

            if displayValue ~= "" then
                title = title .. ": " .. displayValue
            end

            local optionTree = subtree:add(pfOptName, optionRange, metadata.name)

            optionTree:set_text(title)

            local critical = bit.band(optionNumber, 1) ~= 0 and "Critical" or "Elective"
            local safety = bit.band(optionNumber, 2) ~= 0 and "Unsafe" or "Safe"

            optionTree:add(pfOptDescription, optionRange, string.format(
                    "Type %d, %s, %s", optionNumber, critical, safety))
            optionTree:add(pfOptDeltaNibble, tvb(optionStart, 1))
            optionTree:add(pfOptLengthNibble, tvb(optionStart, 1))
            optionTree:add(pfOptNumber, optionRange, optionNumber)
            optionTree:add(pfOptDelta, optionRange, deltaNibble)
            optionTree:add(pfOptLength, optionRange, optionLength)

            if optionLength > 0 then
                optionTree:add(pfOptValue, optionValueRange):set_text(
                    metadata.name .. ": " .. displayValue)
            end

            -- Draft token is carried as option 19
            if optionNumber == 19 then
              if optionLength < 1 or optionLength > 8 then
                  optionTree:add_expert_info(PI_MALFORMED, PI_ERROR, string.format(
                  "Invalid draft token length: %d", optionLength))
              else
                optionTree:add(pfTok, optionValueRange)
              end
            end

            if optionNumber == 11 then
                uriPathParts[#uriPathParts + 1] =
                    displayValue
            end

            offset = optionEnd

            -- OC 0–14 terminates after exactly OC options.
            if not markerMode
                and parsedOptions == optionCount then
                return finishPayload(offset)
            end
        end
    end

    if jumpPending then
        return fail(
            "Draft packet ended immediately after an option jump"
        )
    end

    if markerMode then
        return fail(
            "Draft packet ended before the 0xF0 marker"
        )
    end

    return fail(
        string.format("Expected %d draft options, decoded %d", optionCount, parsedOptions)
    )
end

-- Assess CoAP mode
local function getCoapMode(tvb, pinfo)
   -- Check by CoAP wellknown ports
   local is_rfcPort   = (pinfo.src_port == COAP_PORT_RFC or pinfo.dst_port == COAP_PORT_RFC)
   local is_draftPort = (pinfo.src_port == COAP_PORT_DRAFT or pinfo.dst_port == COAP_PORT_DRAFT)

   if is_rfcPort and is_draftPort then
     return COAP_MODE_AMBIGUOUS
   elseif is_rfcPort then
     return COAP_MODE_RFC
   elseif is_draftPort then
     return COAP_MODE_DRAFT
   end

   -- Check by CoAP header
   -- Fallback for Decode As or non-wellknown ports
   local bits_4_7 = bit.band(tvb(0, 1):uint(), 0x0F)
   -- If bits 4-7 are > 8, it cannot be a valid RFC Token Length (max 8)
   -- It must be a Draft Option Count (OC)
   if bits_4_7 > 8 then
     return COAP_MODE_DRAFT
   end

   -- Fallback for unusual/malformed packets
   return COAP_MODE_AMBIGUOUS
end

-- Decode VarInt
local function decodeVarInt(buf)
   local i = 0
   local value = 0

   while (i < buf:len()) do
     local byteValue = buf(i, 1):uint()
     -- A uint32 VarInt may use only four bits in byte 5.
     if i == 4 and byteValue > 0x0F then
       return nil, nil, "VarInt exceeds uint32 range"
     end

     value = value + (byteValue % 128) * (2 ^ (7 * i))
     i = i + 1

     -- High bit is clear: this is the final byte
     if byteValue < 128 then
       return i, value
     end
     if i >= 5 then
       return nil, nil, "VarInt exceeds 5 bytes"
     end
   end

   return nil, nil, "Truncated VarInt"
end

-- Dissect CoAP header and CSMP payload
function oProtoCsmp.dissector(oTvbData, oPinfo, oTreeItemRoot)

   local oTvbLen = oTvbData:len()
   oPinfo.cols.protocol = "CoAP/CSMP"

   -- Fixed header must be 4 bytes
   if oTvbLen < 4 then
     local truncatedTree = oTreeItemRoot:add(oProtoCsmp, oTvbData(0, oTvbLen),
                                         "Truncated Constrained Application Protocol")
     truncatedTree:add_expert_info(PI_MALFORMED, PI_ERROR, string.format(
                                   "CoAP header requires 4 bytes; only %d available", oTvbLen))
     return
   end

   local coapMode = getCoapMode(oTvbData, oPinfo)

   local subtree = oTreeItemRoot:add(oProtoCsmp, oTvbData(0, oTvbLen), "Constrained Application Protocol")                                       :append_text(string.format(" (%s)", coapMode))

   subtree:add(pfVer,  oTvbData(0, 1))
   local firstByte = oTvbData(0, 1):uint()
   local version = bit.rshift(bit.band(firstByte, 0xC0), 6)
   if version ~= 1 then
     subtree:add_expert_info(PI_MALFORMED, PI_ERROR, string.format("Unsupported CoAP version: %d", version))
     return
   end

   subtree:add(pfType, oTvbData(0, 1))

   local hdrBits_4_7 = bit.band(firstByte, 0x0F)

   if coapMode == COAP_MODE_DRAFT then

     subtree:add(pfOc, oTvbData(0, 1)):append_text(" (draft)")
     --oPinfo.cols.info = string.format("OC:%d ", hdrBits_4_7)

   elseif coapMode == COAP_MODE_RFC then
     subtree:add(pfTkl, oTvbData(0, 1)):append_text(" (rfc)")
     --oPinfo.cols.info = string.format("TL:%d ", hdrBits_4_7)
   end
   -- Add remaining fixed header fields
   subtree:add(pfCode, oTvbData(1, 1))
   subtree:add(pfMid,  oTvbData(2, 2))
   local codeVal = oTvbData(1, 1):uint()
   local codeName = coapCodeNames[codeVal]

   local typeValue = bit.rshift(bit.band(firstByte, 0x30), 4)
   local typeName = coapTypeNames[typeValue] or string.format("(%d)", typeValue)
   local codeDisplay = codeName or string.format("%d.%02d",
                                                 math.floor(codeVal / 32), codeVal % 32)
   subtree:append_text(string.format(", %s %s", typeName, codeDisplay))

   if codeName ~= nil then
    oPinfo.cols.info:append(string.format("  %s %s ", typeName, codeName))
   else
    oPinfo.cols.info:append(string.format("  %s %d.%02d ", typeName,
                     math.floor(codeVal / 32), codeVal % 32))
   end

   local payloadRange
   local optionsValid = true
   if coapMode == COAP_MODE_RFC then
      local tokenLength = hdrBits_4_7
      local optionOffset = 4

    if tokenLength > 8 then
        subtree:add_expert_info(PI_MALFORMED, PI_ERROR, string.format("Invalid RFC token length: %d",
        tokenLength))
        return
    end

    if optionOffset + tokenLength > oTvbLen then
        subtree:add_expert_info(PI_MALFORMED, PI_ERROR, "Token extends beyond the packet")
        return
    end

    if tokenLength > 0 then
        subtree:add(pfTok, oTvbData(optionOffset, tokenLength))
        optionOffset = optionOffset + tokenLength
    end

    payloadRange, optionsValid = dissectRfcOptions(oTvbData, optionOffset, subtree, coapMode)

    elseif coapMode == COAP_MODE_DRAFT then
    payloadRange, optionsValid = dissectDraftOptions(oTvbData, 4, subtree, hdrBits_4_7)
    end

    if not optionsValid then
        return
    end

   -- payloadRange now contains only the CSMP payload.
   -- Save Info column
   local colInfo = tostring(oPinfo.cols.info)
   local tlvIds = {}

   if payloadRange ~= nil then
      local oSubtree = oTreeItemRoot:add(oProtoCsmp, payloadRange, 'CoAP Simple Management Protocol')
      local payloadTvb = payloadRange:tvb("CSMP payload")
      local uiIndex = 0

   -- Loop through data and add the TLVs
   while (uiIndex < payloadTvb:len()) do
      local typeLen, typeValue, typeError = decodeVarInt(payloadTvb(uiIndex))  -- Get TLV ID
      if typeLen == nil then
        oSubtree:add_expert_info(PI_MALFORMED, PI_ERROR, "Invalid TLV type: " .. typeError)
        break
      end

      local lengthLen, lengthValue, lengthError = decodeVarInt(payloadTvb(uiIndex + typeLen)) -- Get TLV Length
      if lengthLen == nil then
        oSubtree:add_expert_info(PI_MALFORMED, PI_ERROR, "Invalid TLV length: " .. lengthError)
        break
      end

      local dataOffset = uiIndex + typeLen + lengthLen
      if dataOffset + lengthValue > payloadTvb:len() then
        oSubtree:add_expert_info(PI_MALFORMED, PI_ERROR,
                 string.format("TLV %d length %d extends beyond payload", typeValue, lengthValue))
        break
      end

      local dataBuf = payloadTvb(dataOffset, lengthValue)                        -- Get TLV Data
      tlvIds[#tlvIds + 1] = string.format("%d", typeValue)
      -- Add the TLV subtree
      local tlvName = getTlvName(typeValue)
      local oTlvTree = oSubtree:add(oProtoCsmp, payloadTvb(uiIndex, typeLen + lengthLen + lengthValue),
           string.format("TLV:%d %s", typeValue, tlvName))

      oTlvTree:add(pfTlvId, payloadTvb(uiIndex, typeLen), typeValue)
      oTlvTree:add(pfTlvLength, payloadTvb(uiIndex + typeLen, lengthLen), lengthValue)
      oTlvTree:add(pfTlvData, dataBuf)

      -- Attempt to decode with ProtoBuf
      local protobufMessage = protobufMessageMap[typeValue]
      if protobufMessage then
        if oProtoProtobuf then
          oPinfo.private["pb_msg_type"] = "message," .. protobufMessage
          oProtoProtobuf:call(dataBuf:tvb(), oPinfo, oTlvTree)
        else
          oTlvTree:add_expert_info(PI_UNDECODED, PI_WARN, "Protobuf dissector is unavailable")
        end
      end

      uiIndex = dataOffset + lengthValue
   end
   if #tlvIds > 0 then
    local ids = table.concat(tlvIds, " ")
    colInfo = colInfo .. " TLV:" .. ids .. " "
    oSubtree:append_text(", TLV:" .. ids)
  end
 end
   -- Update Protocol/Info columns
   oPinfo.cols.protocol:set("CoAP/CSMP")
   oPinfo.cols.protocol:fence()
   oPinfo.cols.info:clear()
   oPinfo.cols.info:set(colInfo)
   oPinfo.cols.info:fence()

end

-- Add dissector to CoAP Data Media Type Dissector Table
-- DissectorTable.get("media_type"):add("application/octet-stream", oProtoCsmp)
local udp_table = DissectorTable.get("udp.port")
udp_table:add(COAP_PORT_RFC, oProtoCsmp)
udp_table:add(COAP_PORT_DRAFT, oProtoCsmp)
