-- TCP

message_length = ProtoField.int32("latency_sample.message_length", "messageLength", base.DEC)
type_tcp = ProtoField.string("latency_sample_tcp.type", "type")
pos_x_tcp = ProtoField.float("latency_sample_tcp.x", "x")
pos_y_tcp = ProtoField.float("latency_sample_tcp.y", "y")
border_r_tcp = ProtoField.uint8("latency_sample_tcp.border_r", "r")
border_g_tcp = ProtoField.uint8("latency_sample_tcp.border_g", "g")
border_b_tcp = ProtoField.uint8("latency_sample_tcp.border_b", "b")
fill_r_tcp = ProtoField.int16("latency_sample_tcp.fill_r", "r")
fill_g_tcp = ProtoField.int16("latency_sample_tcp.fill_g", "g")
fill_b_tcp = ProtoField.int16("latency_sample_tcp.fill_b", "b")

sample_protocol_tcp = Proto("LatencySampleTCP", "LatencySample TCP Protocol")
sample_protocol_tcp.fields = { message_length, type_tcp, pos_x_tcp, pos_y_tcp }

function sample_protocol_tcp.dissector(buffer, pinfo, tree)
  length = buffer:len()
  if length == 0 then return end

  local subtree = tree:add(sample_protocol_tcp, buffer(), "LatencySample Protocol Data")
  pinfo.cols.protocol = sample_protocol_tcp.name
  
  subtree:add(message_length, buffer(0,4))
  local msgType = buffer(4,1):uint()
  if msgType == 0 then
    subtree:add(type_tcp, "Position")
    subtree:add_le(pos_x_tcp, buffer(5,4))
    subtree:add_le(pos_y_tcp, buffer(9,4))
    pinfo.cols.info = "Position(" .. buffer(5,4):le_float() .. "," .. buffer(9,4):le_float() .. ")"
  elseif msgType == 1 then
    subtree:add(type_tcp, "BorderColor")
    subtree:add(border_r_tcp, buffer(5, 1))
    subtree:add(border_g_tcp, buffer(6, 1))
    subtree:add(border_b_tcp, buffer(7, 1))
    pinfo.cols.info = "Border(" .. buffer(5,1):uint() .. "," .. buffer(6,1):uint() .. "," .. buffer(7,1):uint() .. ")"
  elseif msgType == 2 then
    subtree:add(type_tcp, "FillColorDelta")
    subtree:add_le(fill_r_tcp, buffer(5, 2))
    subtree:add_le(fill_g_tcp, buffer(7, 2))
    subtree:add_le(fill_b_tcp, buffer(9, 3))
    pinfo.cols.info = "FillDelta(" .. buffer(5,2):le_int() .. "," .. buffer(7,2):le_int() .. "," .. buffer(9,2):le_iint() .. ")"
  end
end

local tcp_port = DissectorTable.get("tcp.port")
tcp_port:add(5789, sample_protocol_tcp)

-- UDP

type_udp = ProtoField.string("latency_sample_udp.type", "type")
pos_x_udp = ProtoField.float("latency_sample_udp.x", "x")
pos_y_udp = ProtoField.float("latency_sample_udp.y", "y")
border_r_udp = ProtoField.uint8("latency_sample_udp.border_r", "r")
border_g_udp = ProtoField.uint8("latency_sample_udp.border_g", "g")
border_b_udp = ProtoField.uint8("latency_sample_udp.border_b", "b")
fill_r_udp = ProtoField.int16("latency_sample_udp.fill_r", "r")
fill_g_udp = ProtoField.int16("latency_sample_udp.fill_g", "g")
fill_b_udp = ProtoField.int16("latency_sample_udp.fill_b", "b")


sample_protocol_udp = Proto("LatencySampleUDP", "LatencySample UDP Protocol")
sample_protocol_udp.fields = { type_udp, pos_x_udp, pos_y_udp, border_r_udp, border_g_udp, border_b_udp, fill_r_udp, fill_g_udp, fill_b_udp }

function sample_protocol_udp.dissector(buffer, pinfo, tree)
  length = buffer:len()
  if length == 0 then return end

  local subtree = tree:add(sample_protocol_udp, buffer(), "LatencySample Protocol Data")
  pinfo.cols.protocol = sample_protocol_udp.name
  
  local msgType = buffer(0,1):uint()
  if msgType == 0 then
    subtree:add(type_udp, "Position")
    subtree:add_le(pos_x_udp, buffer(1,4))
    subtree:add_le(pos_y_udp, buffer(5,4))
    pinfo.cols.info = "Position(" .. buffer(1,4):le_float() .. "," .. buffer(5,4):le_float() .. ")"
  elseif msgType == 1 then
    subtree:add(type_udp, "BorderColor")
    subtree:add(border_r_udp, buffer(1, 1))
    subtree:add(border_g_udp, buffer(2, 1))
    subtree:add(border_b_udp, buffer(3, 1))
    pinfo.cols.info = "Border(" .. buffer(1,1):uint() .. "," .. buffer(2,1):uint() .. "," .. buffer(3,1):uint() .. ")"
  elseif msgType == 2 then
    subtree:add(type_udp, "FillColorDelta")
    subtree:add_le(fill_r_udp, buffer(1, 2))
    subtree:add_le(fill_g_udp, buffer(3, 2))
    subtree:add_le(fill_b_udp, buffer(5, 3))
    pinfo.cols.info = "FillDelta(" .. buffer(1,2):le_int() .. "," .. buffer(3,2):le_int() .. "," .. buffer(5,2):le_iint() .. ")"
  end
end

local udp_port = DissectorTable.get("udp.port")
udp_port:add(5789, sample_protocol_udp)