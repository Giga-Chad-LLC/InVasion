extends Node

const HOST: String = "127.0.0.1"
const PORT: int = 8000
const RECONNECT_TIMEOUT: float = 3.0


const Client = preload("res://client.gd")
var _client: Client = Client.new()


func _ready() -> void:
	_client.connect("connected", self, "_handle_client_connected")
	_client.connect("disconnected", self, "_handle_client_disconnected")
	_client.connect("error", self, "_handle_client_error")
	_client.connect("data", self, "_handle_client_data")
	add_child(_client)
	_client.connect_to_host(HOST, PORT)



func _connect_after_timeout(timeout: float) -> void:
	yield(get_tree().create_timer(timeout), "timeout") # Delay for timeout
	_client.connect_to_host(HOST, PORT)


func _handle_client_connected() -> void:
	print("Client connected to server.")
	
	var message: PoolByteArray = [104, 101, 108, 108, 111, 32, 102, 114,
								  111, 109, 32, 103, 111, 100, 111, 116, 32]
	var res = _client.send(message)
	print("Send message to server: ", message.get_string_from_ascii())
	print("Result", res)


func _handle_client_data(data: PoolByteArray) -> void:
	print("Client data: ", data.get_string_from_utf8())

func _handle_client_disconnected() -> void:
	print("Client disconnected from server.")
	_connect_after_timeout(RECONNECT_TIMEOUT) # Try to reconnect after 3 seconds
	
func _handle_client_error() -> void:
	print("Client error.")
	_connect_after_timeout(RECONNECT_TIMEOUT) # Try to reconnect after 3 seconds
