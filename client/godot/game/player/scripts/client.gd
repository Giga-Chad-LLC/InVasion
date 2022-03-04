extends Node


# TCP Networking
const HOST: String = "127.0.0.1" # 192.168.2.78
const PORT: int = 8000
const RECONNECT_TIMEOUT: float = 3.0

const Network = preload("res://network/network.gd")
var connection: Network = Network.new()


func init_network() -> void:
	add_child(connection)
	connection.connect("connected", self, "_handle_client_connected")
	connection.connect("disconnected", self, "_handle_client_disconnected")
	connection.connect("error", self, "_handle_client_error")
	connection.connect_to_host(HOST, PORT)


func is_connected_to_host() -> bool:
	return connection.is_connected_to_host()

func send_packed_data(data: PoolByteArray) -> bool:
	return connection.send(_pack_data(data))

func send_raw_data(data: PoolByteArray) -> bool:
	return connection.send(data)


# Appends metadata to the front of `data` array (little endian)
func _pack_data(data: PoolByteArray) -> PoolByteArray:
	var bytes_encoder: StreamPeer = StreamPeerBuffer.new();
	var bytes_array: PoolByteArray
#	Append `data` length to the first 4 bytes of the payload
	bytes_encoder.put_32(data.size())

	bytes_array.append_array(bytes_encoder.data_array)
	bytes_array.append_array(data)
	return bytes_array



# Default handlers
func _connect_after_timeout(timeout: float) -> void:
	yield(get_tree().create_timer(timeout), "timeout") # Delay for timeout
	connection.connect_to_host(HOST, PORT)


func _handle_client_connected() -> void:
	print("Client connected to server.")


func _handle_client_disconnected() -> void:
	print("Client disconnected from server.")
	_connect_after_timeout(RECONNECT_TIMEOUT) # Try to reconnect after 3 seconds

func _handle_client_error() -> void:
	print("Client connection error.")
	_connect_after_timeout(RECONNECT_TIMEOUT) # Try to reconnect after 3 seconds
