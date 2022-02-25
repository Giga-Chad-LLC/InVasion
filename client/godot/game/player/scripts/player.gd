extends KinematicBody2D

# Variables
export var ACCELERATION = 500
export var MAX_SPEED = 100
export var FRICTION = 500

onready var animationPlayer = $AnimationPlayer
onready var animationTree = $AnimationTree
onready var animationState = animationTree.get("parameters/playback")



enum { MOVE }
var state = MOVE
var velocity = Vector2.ZERO
var player_id = 1

enum {
	Idle = 0,
	StartMovingUp = 1,
	StartMovingRight = 2,
	StartMovingDown = 3,
	StartMovingLeft = 4,
	
	StopMovingUp = 5,
	StopMovingRight = 6,
	StopMovingDown = 7,
	StopMovingLeft = 8
}

var input_vector = Vector2.ZERO
var previous_action = Idle

# Godobuf
const PlayerProto = preload("res://player/scripts/player_proto.gd")

# TCP Networking
const HOST: String = "127.0.0.1"
const PORT: int = 8000
const RECONNECT_TIMEOUT: float = 3.0

const Client = preload("res://client.gd")
var _client: Client = Client.new()


# Appends data.size() to the front of data (little endian)
func pack(data: PoolByteArray) -> PoolByteArray:
	var bytes_encoder: StreamPeer = StreamPeerBuffer.new();
	var bytes_array: PoolByteArray
	bytes_encoder.put_32(data.size())

#	print("Raw data: ", data)
#	print("Size only: ", bytes_encoder.data_array)
	bytes_array.append_array(bytes_encoder.data_array)
	bytes_array.append_array(data)
#	print("Total pack: ", bytes_array)
	
	return bytes_array

func get_packed_move_action() -> PlayerProto.PlayerAction:
	var packed_player_action = PlayerProto.PlayerAction.new()
	
	if (Input.is_action_just_pressed("ui_up")):
#		return StartMovingUp
		packed_player_action.set_key_pressed(packed_player_action.Action.StartMovingUp)
	elif (Input.is_action_just_released("ui_up")):
#		return StopMovingUp
		packed_player_action.set_key_pressed(packed_player_action.Action.StopMovingUp)
	elif (Input.is_action_just_pressed("ui_right")):
#		return StartMovingRight
		packed_player_action.set_key_pressed(packed_player_action.Action.StartMovingRight)
	elif (Input.is_action_just_released("ui_right")):
#		return StopMovingRight
		packed_player_action.set_key_pressed(packed_player_action.Action.StopMovingRight)
	
	elif (Input.is_action_just_pressed("ui_down")):
#		return StartMovingDown
		packed_player_action.set_key_pressed(packed_player_action.Action.StartMovingDown)
	elif (Input.is_action_just_released("ui_down")):
#		return StopMovingDown
		packed_player_action.set_key_pressed(packed_player_action.Action.StopMovingUp)
	
	elif (Input.is_action_just_pressed("ui_left")):
#		return StartMovingLeft
		packed_player_action.set_key_pressed(packed_player_action.Action.StartMovingLeft)
	elif (Input.is_action_just_released("ui_left")):
#		return StopMovingLeft
		packed_player_action.set_key_pressed(packed_player_action.Action.StopMovingLeft)
	else:
		packed_player_action.set_key_pressed(packed_player_action.Action.Idle)
	
	return packed_player_action

func get_response(move_event):
	if (move_event == StartMovingUp):
		input_vector.y = -1
	if (move_event == StopMovingUp):
		input_vector.y = 0
	
	if (move_event == StartMovingRight):
		input_vector.x = 1
	if (move_event == StopMovingRight):
		input_vector.x = 0
	
	if (move_event == StartMovingDown):
		input_vector.y = 1
	if (move_event == StopMovingDown):
		input_vector.y = 0
	
	if (move_event == StartMovingLeft):
		input_vector.x = -1
	if (move_event == StopMovingLeft):
		input_vector.x = 0
	
	return input_vector




# Built-in functions
func _ready():
	animationTree.active = true
	init_network()

func _physics_process(delta):
#	match state:
#		MOVE:
#			var input_vector = get_move_input_vector()
#			player_move(delta, input_vector)
	var action: PlayerProto.PlayerAction = get_packed_move_action()
	if (action.get_key_pressed() != Idle and previous_action != action.get_key_pressed() and
		_client.is_connected_to_host()):
		previous_action = action.get_key_pressed()
		var packed_data: PoolByteArray = pack(action.to_bytes())
		var result = _client.send(packed_data)
		if (result):
			print("Sent: ", packed_data)
#		print("Packed: ", action.to_bytes())
#		print("Send: action ", action.get_key_pressed())

#	yield(get_tree().create_timer(0.2), "timeout")
	var input_vector = get_response(action.get_key_pressed())
	player_move(delta, input_vector)


# TCP Networking
func init_network() -> void:
	_client.connect("connected", self, "_handle_client_connected")
	_client.connect("disconnected", self, "_handle_client_disconnected")
	_client.connect("error", self, "_handle_client_error")
	_client.connect("receive", self, "_handle_client_receive_data")
	add_child(_client)
	_client.connect_to_host(HOST, PORT)
	


func _connect_after_timeout(timeout: float) -> void:
	yield(get_tree().create_timer(timeout), "timeout") # Delay for timeout
	_client.connect_to_host(HOST, PORT)


func _handle_client_connected() -> void:
	print("Client connected to server.")
#
#	var message: PoolByteArray = [104, 101, 108, 108, 111, 32, 102, 114,
#								  111, 109, 32, 103, 111, 100, 111, 116, 32]
#	var res = _client.send(message)
#	print("Send message to server: ", message.get_string_from_ascii())



func _handle_client_receive_data(data: PoolByteArray) -> void:
	var unpacked_player_action = PlayerProto.PlayerAction.new()
	var result_code = unpacked_player_action.from_bytes(data)
	if (result_code == PlayerProto.PB_ERR.NO_ERRORS):
		print("Received data: ", unpacked_player_action.get_key_pressed())
	else:
		print("Error when receive: ", "cannot unpack data")

func _handle_client_disconnected() -> void:
	print("Client disconnected from server.")
	_connect_after_timeout(RECONNECT_TIMEOUT) # Try to reconnect after 3 seconds

func _handle_client_error() -> void:
	print("Client connection error.")
	_connect_after_timeout(RECONNECT_TIMEOUT) # Try to reconnect after 3 seconds


# User-defined functions
func get_move_input_vector():
	var input_vector = Vector2.ZERO
	input_vector.x = Input.get_action_strength("ui_right") - Input.get_action_strength("ui_left")
	input_vector.y = Input.get_action_strength("ui_down") - Input.get_action_strength("ui_up")
	input_vector = input_vector.normalized()
	return input_vector

func player_move(delta, input_vector):
	if input_vector != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", input_vector)
		animationTree.set("parameters/Run/blend_position", input_vector)

		animationState.travel("Run")
		
		velocity = velocity.move_toward(input_vector * MAX_SPEED, ACCELERATION * delta)
	else:
		animationState.travel("Idle")
		velocity = velocity.move_toward(Vector2.ZERO, FRICTION * delta)

	velocity = move_and_slide(velocity)



# Godobuf
#func godobuf(action):
#	# Create packed class (message)
#	var a = TestProto.A.new()
#	# Example, set field f1
#	a.set_f1(3.14159)
#	# Pack message A
#	# Use to_bytes() method, it's return PoolByteArray
#	var packed_bytes = a.to_bytes()
#	print("Serialized: ", packed_bytes)
#
#	# Create unpacked class (message)
#	var b = TestProto.A.new()
#	# Unpack byte sequence to class (message) A.
#	# Use from_bytes(PoolByteArray my_byte_sequence) method
#	var result_code = b.from_bytes(packed_bytes)
#	# result_code must be checked (see Unpack result codes section)
#	if result_code == TestProto.PB_ERR.NO_ERRORS:
#		print("OK")
#	else:
#		print("Failed")
#		return
#	# Use class 'a' fields. Example, get field f1
#	var f1 = b.get_f1()
#	print("Deserialized: ", f1)

