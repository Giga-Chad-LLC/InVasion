extends KinematicBody2D

# Animations
onready var animationPlayer = $AnimationPlayer
onready var animationTree = $AnimationTree
onready var animationState = animationTree.get("parameters/playback")
onready var player_gun = $Gun

# Parameters
var previous_action = MoveRequestModel.MoveRequestModel.MoveEvent.Idle
var velocity = Vector2.ZERO
var player_id: int = -1


# Godobuf
const MoveRequestModel = preload("res://proto/request-models/move_request_model.gd")
const PlayerPositionResponseModel = preload("res://proto/response-models/player_position_response_model.gd")
const PlayersPositionsResponseModel = preload("res://proto/response-models/players_positions_response_model.gd")
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

func _physics_process(delta):
#	Send data to server
	if (player_id != -1): # means that we have made sucessfull handshake with the server
		var action: MoveRequestModel.MoveRequestModel = get_packed_move_action()
		if (action.get_current_event() != MoveRequestModel.MoveRequestModel.MoveEvent.Idle
			and previous_action != action.get_current_event() and
			client.is_connected_to_host()):
			previous_action = action.get_current_event()
			var network_packet = NetworkPacket.new()
			network_packet.set_data(action.to_bytes(), Global.RequestModels.MoveRequestModel)
			producer.push_data(network_packet)

#	Receive data from server
	var received_packet: NetworkPacket = consumer.pop_data()
	if (received_packet != null):
		if (received_packet.message_type == Global.ResponseModels.PlayerIdResponseModel):
			set_player_id(received_packet)
		elif (received_packet.message_type == Global.ResponseModels.PlayersPositionsResponseModel):
			var players_positions = PlayersPositionsResponseModel.PlayersPositionsResponseModel.new()
#			print("PP: ", received_packet.get_bytes().size())
			var result_code = players_positions.from_bytes(received_packet.get_bytes())
			if (result_code != PlayersPositionsResponseModel.PB_ERR.NO_ERRORS): 
				print("Error while receiving: ", "cannot unpack players positions")
			else:
#				Find ourselves and update position
				players_positions = players_positions.get_players()
				for i in range(0, players_positions.size()):
					var model: PlayersPositionsResponseModel.PlayerPositionResponseModel = players_positions[i]
					if (model.get_playerId() == player_id):
						update_player_position(model)
						players_positions.erase(model) # erase ourselves
						break
				# update other players
				GameWorld.update_game_state(players_positions)
		else:
			print("Unknown message type!")
	
	animate_player()
	player_gun.animate_gun()

func update_player_position(player_position_model: PlayersPositionsResponseModel.PlayerPositionResponseModel):
	velocity = Vector2(player_position_model.get_velocity().get_x(), player_position_model.get_velocity().get_y())
	global_position = Vector2(player_position_model.get_position().get_x(), player_position_model.get_position().get_y())

func animate_player():
	#	Move the player
	if velocity != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", velocity)
		animationTree.set("parameters/Run/blend_position", velocity)

		animationState.travel("Run")
	else:
		animationState.travel("Idle")
	move_and_slide(velocity)



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
	var player_id_model = PlayerIdResponseModel.PlayerIdResponseModel.new()
	var result_code = player_id_model.from_bytes(packet.get_bytes())
	if (result_code != PlayerIdResponseModel.PB_ERR.NO_ERRORS):
		print("Error while receiving: ", "cannot unpack player id")
	else:
		player_id = player_id_model.get_playerId()
		print("Set my id in a game session: ", player_id)

# Networking
func init_network() -> void:
	client.connection.connect("connected", self, "_handle_client_connected")
	add_child(client)
	client.init_network()

func _handle_client_connected() -> void:
	producer.init(funcref(self, "_produce"))

func _handle_client_receive_data(data: PoolByteArray, worker: Worker) -> void:
#	print("Recieved data: ", data)
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





