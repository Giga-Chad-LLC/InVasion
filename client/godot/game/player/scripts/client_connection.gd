extends Node


# TCP Networking
const HOST: String = "192.168.1.71" #  127.0.0.1 | 192.168.1.71
const PORT: int = 8000
const RECONNECT_TIMEOUT: float = 3.0

const Network = preload("res://network/network.gd")
const NetworkPacket = preload("res://network/data_types.gd")
const Worker = preload("res://network/worker.gd")
const TCPStreamParser = preload("res://network/tcp_stream_parser.gd")

var connection: Network = Network.new()
var reader: TCPStreamParser = TCPStreamParser.new()
var is_connection_closed: bool = false

func _ready():
	add_child(connection)
	connection.connect("connected", self, "_handle_client_connected")
	connection.connect("disconnected", self, "_handle_client_disconnected")
	connection.connect("error", self, "_handle_client_error")

func open_connection(host: String = HOST, port: int = PORT) -> void:
	connection.connect_to_host(host, port)

func close_connection():
	is_connection_closed = true
	connection.disconnect_from_host()


func is_connected_to_host() -> bool:
	return connection.is_connected_to_host()

func send_packed_data(data: NetworkPacket) -> bool:
	return connection.send(_pack_data(data))

func send_raw_data(data: PoolByteArray) -> bool:
	return connection.send(data)


# Appends metadata to the front of `data` array (little endian)
# Message is sent in format: [length: 4 bytes][type: 4 bytes][message: `length` bytes]
func _pack_data(data: NetworkPacket) -> PoolByteArray:
	var bytes_encoder: StreamPeer = StreamPeerBuffer.new()
	var bytes_array: PoolByteArray
	
#	Append `data` length to the first 4 bytes of the payload
	bytes_encoder.put_32(data.bytes.size())
#	Append `message_type` to the second 4 bytes of the payload
	bytes_encoder.put_32(data.message_type)

	bytes_array.append_array(bytes_encoder.data_array)
	bytes_array.append_array(data.get_bytes())
	return bytes_array

# Data format: little endian
# Message comes in format: [length: 4 bytes][type: 4 bytes][message: `length` bytes]
func _unpack_data(data: PoolByteArray) -> NetworkPacket:
	var byte_encoder: StreamPeer = StreamPeerBuffer.new()
	var network_packet: NetworkPacket = NetworkPacket.new()
	byte_encoder.data_array = data
#	invalid message
	if (data.size() < 8):
		print("Data size is less than: ", 8)
		return null;
#	message length
	var message_length = byte_encoder.get_32();
	byte_encoder.seek(4)
#	message type
	var message_type = byte_encoder.get_32()
	byte_encoder.seek(8)
#	invalid message
	if (data.size() - 8 < message_length):
		print("Data size in less than: ", message_length + 8)
		return null
	
	var data_object = byte_encoder.get_data(message_length)
#	Non-OK error code
	if (data_object[0] != 0):
		print("Error code when unpacking: ", data_object[0])
		return null
	var bytes: PoolByteArray = data_object[1]
	network_packet.set_data(bytes, message_type)
#	print("Length: ", message_length)
#	print("Type: ", message_type)
#	print("Bytes: ", bytes)
#	print("Data: ", data.subarray(0, 8 + message_length - 1))
	return network_packet

# Default handlers
func _connect_after_timeout(timeout: float) -> void:
	yield(get_tree().create_timer(timeout), "timeout") # Delay for timeout
	connection.connect_to_host(HOST, PORT)


func _handle_client_connected() -> void:
	print("Client connected to server.")


func _handle_client_disconnected() -> void:
	print("Client disconnected from server.")
	if (!is_connection_closed):
		_connect_after_timeout(RECONNECT_TIMEOUT) # Try to reconnect after 3 seconds

func _handle_client_error() -> void:
	print("Client connection error.")
	if (!is_connection_closed):
		_connect_after_timeout(RECONNECT_TIMEOUT) # Try to reconnect after 3 seconds
