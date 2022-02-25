extends Node



# TCP Client
signal connected      # Connected to server
signal receive        # Received data from server
signal disconnected   # Disconnected from server
signal error          # Error with connection to server


var _status: int = 0
var _stream: StreamPeerTCP = StreamPeerTCP.new()


func _ready() -> void:
	_status = _stream.get_status()


func _process(_delta):
	var new_status: int = _stream.get_status()
	if new_status != _status:
		_status = new_status
		match _status:
			_stream.STATUS_NONE:
				print("Disconnected from host.")
				emit_signal("disconnected")
			_stream.STATUS_CONNECTING:
				print("Connecting to host.")
			_stream.STATUS_CONNECTED:
				print("Connected to host.")
				emit_signal("connected")
			_stream.STATUS_ERROR:
				print("Error with socket stream.")
				emit_signal("error")

	if _status == _stream.STATUS_CONNECTED:
		var available_bytes: int = _stream.get_available_bytes()
		if available_bytes > 0:
#			print("available bytes: ", available_bytes)
			var data: Array = _stream.get_partial_data(available_bytes)
			# Check for read error.
			if data[0] != OK:
				print("Error getting data from stream: ", data[0])
				emit_signal("error")
			else:
				emit_signal("receive", data[1])

func connect_to_host(host: String, port: int) -> void:
	print("Connecting to %s:%d" % [host, port])
	# Reset status so we can tell if it changes to error again.
	_status = _stream.STATUS_NONE
	if _stream.connect_to_host(host, port) != OK:
		print("Error connecting to host.")
		emit_signal("error")


func is_connected_to_host() -> bool:
	return _status == _stream.STATUS_CONNECTED

func send(data: PoolByteArray) -> bool:
	if _status != _stream.STATUS_CONNECTED:
		print("Error: Stream is not currently connected.")
		return false
	var result: int = _stream.put_data(data)
	if result != OK:
		print("Error writing to stream: ", result)
		return false
	return true



# Test
#func try_out_tcp_client_with_cpp_asion_server():
#	var tcp_connection = StreamPeerTCP.new()
#	# Try connecting to the C++ ping-pong server
#	var tcp_server_host = "localhost"
#	var tcp_server_port = 8000
#	tcp_connection.connect_to_host(tcp_server_host, tcp_server_port)
#
#	if (tcp_connection.is_connected_to_host()):
#		print("Connected to host: ", tcp_server_host, ":", tcp_server_port)
#		var string = "Hello for my boiii\n"
#		tcp_connection.put_data(string.to_utf8())
#	else:
#		print("Failed to connect to: ", tcp_server_host, ":", tcp_server_port)
#
#	return tcp_connection
