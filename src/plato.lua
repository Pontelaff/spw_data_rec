---------------------------------------------------------------
-- @file plato.lua
-- @author Jonas Gesch (jonas.gesch@dlr.de)
-- @brief Dissector for PLATO Proprietary Protocol (PPP)
--        as defined in PLATO-DLR-PL-ICD-0007 Rev 2.0 chapter 5
-- @date 2022-02-23
---------------------------------------------------------------

local CCD_SIDE_E = 0
local CCD_SIDE_F = 1
local HEADER_LENGTH = 12
local PLATO_ID = 0xF0

plato_protocol = Proto("PLATO",  "PLATO Proprietary Protocol")

target_address = ProtoField.uint8("plato.target_address", "Target Address", base.HEX)
protocol_id	= ProtoField.uint8("plato.protocol_id", "Protocol ID", base.HEX)
message_length = ProtoField.uint16("plato.message_length", "Message Length", base.DEC)
deb_mode = ProtoField.uint16("plato.deb_mode", "DEB Mode", base.DEC, NULL, 0x0700)
last_packet = ProtoField.uint16("plato.last_packet", "Last Packet", base.DEC, NULL, 0x0080)
ccd_side = ProtoField.uint16("plato.ccd_side", "CCD side", base.DEC, NULL, 0x0040)
aeb_id = ProtoField.uint16("plato.aeb_id", "AEB ID", base.DEC, NULL, 0x0030)
packet_type = ProtoField.uint16("plato.packet_type", "Packet Type", base.DEC, NULL, 0x0003)
frame_counter = ProtoField.uint16("plato.frame_counter", "Frame Counter", base.DEC)
seq_counter = ProtoField.uint16("plato.seq_counter", "Sequence Counter", base.DEC)
--reserved = ProtoField.uint8("plato.reserved", "Reserved", base.HEX)
header_crc = ProtoField.uint8("rmap.header_crc", "Header CRC", base.HEX)
data_field = ProtoField.bytes("rmap.data_field", "Data Field", base.NONE)
data_crc = ProtoField.uint8("rmap.data_crc", "Data CRC", base.HEX)


plato_protocol.fields = { target_address, protocol_id, message_length, packet_type, deb_mode, last_packet, ccd_side, aeb_id, frame_counter, seq_counter, header_crc, data_field, data_crc }

function plato_protocol.dissector(buffer, pinfo, tree)
  length = buffer:len()
  -- Ignore if packet is empty
  if length == 0 then return end

  -- Ignore if not a PLATO packet
  if buffer(1,1):uint() ~= PLATO_ID then return end

  local msg_length = buffer(2,2):int()

  -- Type field (2 byte, bits 15-11 and 3-2 reserved for future use)
  local type_field = buffer(4,2):int()
  local type_field_string = string.format("Type: 0x%04x", type_field)

  -- Right shift and mask relevant bits to add information as text
  local deb_mode_number = bit.band(bit.rshift(type_field, 8), 0x0007)
  local deb_mode_string = get_deb_mode_name(deb_mode_number)

  local aeb_id_number = bit.band(bit.rshift(type_field, 4), 0x0003) + 1

  local ccd_side_flag = bit.band(bit.rshift(type_field, 6), 0x0001)
  local ccd_side_string = get_ccd_side(ccd_side_flag)

  local packet_type_number = bit.band(type_field, 0x0003)
  local packet_type_string = get_packet_type_name(packet_type_number)

  -- Info for Wireshark columns
  pinfo.cols.protocol = plato_protocol.name
  info_string = string.format("Length = %3d     Type = %s     CCD = %d%s     SeqCounter = %d", msg_length, packet_type_string, aeb_id_number, ccd_side_string, buffer(8,2):int())
  pinfo.cols.info = info_string

  -- PLATO subtree
  local subtree = tree:add(plato_protocol, buffer(), "PLATO Protocol Data")


  -- Header subtree
  local headerSubtree = subtree:add(plato_protocol, buffer(), "Header")
  headerSubtree:add(target_address, buffer(0,1))
  headerSubtree:add(protocol_id, buffer(1,1))
  headerSubtree:add(message_length, buffer(2,2))
	-- Type subtree
	local typeSubtree = headerSubtree:add(plato_protocol, buffer(), type_field_string)
	typeSubtree:add(deb_mode, buffer(4,2)):append_text(" (" .. deb_mode_string .. ")")
	typeSubtree:add(last_packet, buffer(4,2))
	typeSubtree:add(ccd_side, buffer(4,2)):append_text(" (" .. ccd_side_string .. ")")
	typeSubtree:add(aeb_id, buffer(4,2))
	typeSubtree:add(packet_type, buffer(4,2)):append_text(" (" .. packet_type_string .. ")")
  headerSubtree:add(frame_counter, buffer(6,2))
  headerSubtree:add(seq_counter, buffer(8,2))
  --headerSubtree:add(reserved, buffer(10,1))
  headerSubtree:add(header_crc, buffer(11,1))


  -- Payload subtree
  if length > HEADER_LENGTH then
    local payloadSubtree = subtree:add(plato_protocol, buffer(), "Payload")
	payloadSubtree:add(data_field, buffer(HEADER_LENGTH, msg_length - 1))
	payloadSubtree:add(data_crc, buffer(HEADER_LENGTH + msg_length - 1, 1))
  end

end

-- DEB mode text information
function get_deb_mode_name(deb_mode)
  local deb_mode_name = "Unknown"

      if deb_mode == 0 then deb_mode_name = "FULL-IMAGE"
  elseif deb_mode == 1 then deb_mode_name = "FULL-IMAGE PATTERN"
  elseif deb_mode == 2 then deb_mode_name = "WINDOWING"
  elseif deb_mode == 3 then deb_mode_name = "WINDOWING PATTERN" end

  return deb_mode_name
end

-- CCD side text information
function get_ccd_side(ccd_side_flag)
  local ccd_side = "Unknown"

      if ccd_side_flag == CCD_SIDE_E then ccd_side = 'E'
  elseif ccd_side_flag == CCD_SIDE_F then ccd_side = 'F' end

  return ccd_side
end

-- Packet type text information
function get_packet_type_name(packet_type)
  local packet_type_name = "Unknown"

      if packet_type == 0 then packet_type_name = "Data           "
  elseif packet_type == 1 then packet_type_name = "Overscan data  "
  elseif packet_type == 2 then packet_type_name = "DEB houskeeping"
  elseif packet_type == 3 then packet_type_name = "AEB houskeeping" end

  return packet_type_name
end


local tcp_port = DissectorTable.get("tcp.port")
tcp_port:add(59274, plato_protocol)