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
const ShootRequestModel = preload("res://proto/request-models/shoot_request_model.gd")

const PlayerPositionResponseModel = preload("res://proto/response-models/player_position_response_model.gd")
const PlayerIdResponseModel = preload("res://proto/response-models/player_id_response_model.gd")
const GameStateResponseModel = preload("res://proto/response-models/game_state_response_model.gd")

# Network
const Connection = preload("res://player/scripts/client.gd")
const NetworkPacket = preload("res://network/data_types.gd")
var client: Connection = Connection.new()

const Worker = preload("res://network/worker.gd")
const ThreadSafeQueue = preload("res://network/thread_safe_queue.gd")
var producer: Worker = Worker.new() # thread that stores events from client
var consumer: Worker = Worker.new() # thread that will read data from the server into a buffer
									# and put correct network packets to the thread-safe-queue
var server_network_packet_queue: ThreadSafeQueue = ThreadSafeQueue.new() # thread that stores events from server
var consumer_unpacking_thread: Thread = Thread.new()

# Built-in functions
func _ready():
	# Activate animation for the character
	animationTree.active = true
	# Establish connection to server
	init_network()
	# Run consumer (thread that reads data from server)
	consumer.init(funcref(self, "_consumer_receive_data"))
	add_child(producer)
	add_child(consumer)

func _physics_process(_delta):
#	Send player movements to server
	if (player_id != -1): # means that we have made sucessfull handshake with the server
		# Moving logic
		send_player_move_request()
		# Shooting logic
		send_player_shoot_request()
	
#	Receive data from server
	var received_packet: NetworkPacket = consumer.pop_data() 
	if (received_packet != null):
		if (received_packet.message_type == Global.ResponseModels.PlayerIdResponseModel):
			set_player_id(received_packet)
		elif (received_packet.message_type == Global.ResponseModels.GameStateResponseModel):
			var new_game_state = GameStateResponseModel.GameStateResponseModel.new()
			var result_code = new_game_state.from_bytes(received_packet.get_bytes())
			if (result_code != GameStateResponseModel.PB_ERR.NO_ERRORS): 
				print("Error while receiving: ", "cannot unpack game update model")
			else:
				# update myself
				var players_positions = new_game_state.get_players()
				for i in range(0, players_positions.size()):
					var model = players_positions[i]
					if (model.get_player_id() == player_id):
						update_player_position(model)
						players_positions.erase(model) # erase ourselves
						break
				# update other players
				GameWorld.update_players_states(players_positions)
				# update bullets
				var bullets_positions = new_game_state.get_bullets()
				GameWorld.update_bullets_states(bullets_positions)
		elif (received_packet.message_type == Global.ResponseModels.ShootingStateResponseModel):
			# Update our ammo count, gun reloading state
			print("We shot a bullet!")
		else:
			print("Unknown message type: ", received_packet.message_type)

#	Client logic methods
	animate_player()
	player_gun.animate_gun()

func send_player_move_request():
	var action: MoveRequestModel.MoveRequestModel = get_packed_move_action()
	if (action.get_current_event() != MoveRequestModel.MoveRequestModel.MoveEvent.Idle
		and previous_action != action.get_current_event() and
		client.is_connected_to_host()):
		previous_action = action.get_current_event()
		var network_packet = NetworkPacket.new()
		network_packet.set_data(action.to_bytes(), Global.RequestModels.MoveRequestModel)
		producer.push_data(network_packet)

func send_player_shoot_request():
	if (Input.is_action_pressed("shoot") and not player_gun.is_cooldown):
		player_gun.shoot_bullet(player_gun.global_rotation)
		var action = ShootRequestModel.ShootRequestModel.new()
		action.set_player_id(player_id)
		action.new_weapon_direction()
		action.get_weapon_direction().set_x(cos(player_gun.global_rotation))
		action.get_weapon_direction().set_y(sin(player_gun.global_rotation))
		var network_packet = NetworkPacket.new()
		network_packet.set_data(action.to_bytes(), Global.RequestModels.ShootRequestModel)
		producer.push_data(network_packet)

func update_player_position(player_position_model):
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
		player_id = player_id_model.get_player_id()
		print("Set my id in a game session: ", player_id)

# Networking
func init_network() -> void:
	client.connection.connect("connected", self, "_handle_client_connected")
	add_child(client)
	client.init_network()

func _handle_client_connected() -> void:
	producer.init(funcref(self, "_produce"))

func _handle_client_receive_data(data: PoolByteArray, worker: Worker) -> void:
#	var network_packet: NetworkPacket = client._unpack_data(data)
#	if (network_packet):
#		worker.push_data(network_packet)
	# store incoming data into a main bytes buffer
	client.reader.add_data(data)
	var chunk: Array = client.reader.get_next_packet_sequence()
	while (!chunk.empty()):
		worker.push_data(client._unpack_data(chunk))
		chunk = client.reader.get_next_packet_sequence()
	client.reader.flush()

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
