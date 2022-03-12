extends KinematicBody2D

# Physics
export var ACCELERATION = 500
export var MAX_SPEED = 100
export var FRICTION = 500
# Animations
onready var animationPlayer = $AnimationPlayer
onready var animationTree = $AnimationTree
onready var animationState = animationTree.get("parameters/playback")

# Moves
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

var previous_action = Idle
var current_move_event = Idle # makes the character move
enum { MOVE }
var state = MOVE
var velocity = Vector2.ZERO
var current_position = Vector2.ZERO
var player_id: int = -1


# Godobuf
const PlayerProto = preload("res://player/scripts/player_proto.gd")
const MoveRequestModel = preload("res://proto/request-models/move_request_model.gd")

const PlayerPositionResponseModel = preload("res://proto/response-models/player_position_response_model.gd")
const PlayerIdResponseModel = preload("res://proto/response-models/player_id_response_model.gd")

# Network
const Connection = preload("res://player/scripts/client.gd")
const NetworkPacket = preload("res://network/data_types.gd")
var client: Connection = Connection.new()

const Worker = preload("res://network/worker.gd")
var producer: Worker = Worker.new() 
var consumer: Worker = Worker.new()


# Built-in functions
func _ready():
	animationTree.active = true
	init_network()
	# Run consumer (thread that reads data from server)
	consumer.init(funcref(self, "_consumer_receive_data"))
	add_child(producer)
	add_child(consumer)


func _process(delta):
#	Send data to server
	if (player_id != -1): # means that we have made sucessfull handshake with the server
		var action: MoveRequestModel.MoveRequestModel = get_packed_move_action()
		if (action.get_current_event() != Idle and previous_action != action.get_current_event() and
			client.is_connected_to_host()):
			previous_action = action.get_current_event()
			var network_packet = NetworkPacket.new()
			network_packet.set_data(action.to_bytes(), Global.RequestModels.MoveRequestModel)
			producer.push_data(network_packet)
	
#	Receive data from server
	var received_packet: NetworkPacket = consumer.pop_data()
	if (received_packet != null):
		if (received_packet.message_type == Global.ResponseModels.PlayerActionResponseModel):
			var unpacked_player_action = PlayerProto.PlayerAction.new()
			var result_code = unpacked_player_action.from_bytes(received_packet.get_bytes())
			if (result_code == PlayerProto.PB_ERR.NO_ERRORS):
				current_move_event = unpacked_player_action.get_key_pressed()
				print("Action from server: ", current_move_event)
			else:
				print("Error while receiving: ", "cannot unpack player action")
		elif (received_packet.message_type == Global.ResponseModels.PlayerIdResponseModel):
			set_player_id(received_packet)
			print(player_id)
		elif (received_packet.message_type == Global.ResponseModels.PlayerPositionResponseModel):
			update_player_position(received_packet)
			
		else:
			print("Unknown message type!")
	
	player_move_by_coordinates(delta)

func update_player_position(packet: NetworkPacket):
	var player_position_model = PlayerPositionResponseModel.PlayerPositionResponseModel.new()
	player_position_model.from_bytes(packet.get_bytes())
	print("Velocity: ", player_position_model.get_velocity().get_x(), " ", player_position_model.get_velocity().get_y())
	print("Position: ", player_position_model.get_position().get_x(), " ", player_position_model.get_position().get_y())
	print("Id: ", player_position_model.get_playerId())
	velocity.x = player_position_model.get_velocity().get_x()
	velocity.y = player_position_model.get_velocity().get_y()
	position.x = player_position_model.get_position().get_x()
	position.y = player_position_model.get_position().get_y()
	global_position.linear_interpolate(position, 0.001)
	

func player_move_by_coordinates(delta):
	#	Move the player	
	if velocity != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", velocity)
		animationTree.set("parameters/Run/blend_position", velocity)

		animationState.travel("Run")
	else:
		animationState.travel("Idle")

#	velocity = move_and_slide(velocity)
	global_position += velocity
	




# save pressed key to the model object
func get_packed_move_action() -> MoveRequestModel.MoveRequestModel:
	var packed_player_action = MoveRequestModel.MoveRequestModel.new()
	packed_player_action.set_player_id(player_id)
	packed_player_action.set_session_id(0) # dummy session ID, will be removed in the future probably 

	if (Input.is_action_just_pressed("ui_up")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StartMovingUp)
	elif (Input.is_action_just_released("ui_up")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StopMovingUp)
	elif (Input.is_action_just_pressed("ui_right")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StartMovingRight)
	elif (Input.is_action_just_released("ui_right")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StopMovingRight)

	elif (Input.is_action_just_pressed("ui_down")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StartMovingDown)
	elif (Input.is_action_just_released("ui_down")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StopMovingUp)

	elif (Input.is_action_just_pressed("ui_left")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StartMovingLeft)
	elif (Input.is_action_just_released("ui_left")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StopMovingLeft)
	else:
		packed_player_action.set_current_event(packed_player_action.MoveEvent.Idle)
	
	return packed_player_action

# Set player id retrieved from server
func set_player_id(packet: NetworkPacket) -> void:
	print("Set my id in a game session")
	var player_id_model = PlayerIdResponseModel.PlayerIdResponseModel.new()
	player_id = player_id_model.get_playerId()


# Networking
func init_network() -> void:
	client.connection.connect("connected", self, "_handle_client_connected")
	add_child(client)
	client.init_network()

func _handle_client_connected() -> void:
	producer.init(funcref(self, "_produce"))

func _handle_client_receive_data(data: PoolByteArray, worker: Worker) -> void:
	print("Recieved data: ", data)
	var network_packet: NetworkPacket = client._unpack_data(data)
	worker.push_data(network_packet)


# Producer and consumer queues
func _produce(worker: Worker) -> void:
	while true:
		if (worker.queue.is_empty()):
			worker.semaphore.wait()

		worker.mutex.lock()
		var should_exit: bool = worker.exit_thread
		worker.mutex.unlock()

		if should_exit:
			break

		var network_packet: NetworkPacket = worker.queue.pop()
		if (!client.send_packed_data(network_packet)):
			print("Error while sending packet: ", network_packet.get_bytes())

func _consumer_receive_data(worker: Worker):
	client.connection.connect("receive", self, "_handle_client_receive_data", [worker])


# Move player by key action
func get_move_vector_by_event(move_event):
	var input_vector = Vector2.ZERO
	
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
func player_move_by_key_action(delta):
	#	Move the player
	var input_vector = get_move_vector_by_event(current_move_event)
	
	if velocity != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", input_vector)
		animationTree.set("parameters/Run/blend_position", input_vector)

		animationState.travel("Run")

		velocity = velocity.move_toward(input_vector * MAX_SPEED, ACCELERATION * delta)
	else:
		animationState.travel("Idle")
		velocity = velocity.move_toward(Vector2.ZERO, FRICTION * delta)

	velocity = move_and_slide(velocity)








