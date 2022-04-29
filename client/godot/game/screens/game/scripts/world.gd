extends Node2D


signal scene_changed(scene_name)

# Nodes
onready var bullets_parent_node = $YSort/Bullets
onready var players_parent_node = $YSort/OtherPlayers
onready var Player = $YSort/Player

var PlayersStateManager = preload("res://player/scripts/players_state_manager.gd")
onready var players_state_manager = PlayersStateManager.new()
var BulletsStateManager = preload("res://models/bullet/scripts/bullets_state_manager.gd")
onready var bullets_state_manager = BulletsStateManager.new()

# Godobuf
const MoveRequestModel = preload("res://proto/request-models/move_request_model.gd")
const ShootRequestModel = preload("res://proto/request-models/shoot_request_model.gd")

const PlayerPositionResponseModel = preload("res://proto/response-models/player_position_response_model.gd")
const PlayerInfoResponseModel = preload("res://proto/response-models/player_info_response_model.gd")
const GameStateResponseModel = preload("res://proto/response-models/game_state_response_model.gd")

# Network
const Connection = preload("res://player/scripts/client_connection.gd")
var client_connection: Connection = Connection.new()

const Worker = preload("res://network/worker.gd")
var producer: Worker = Worker.new() # thread that stores events from client
var consumer: Worker = Worker.new() # thread that will read data from the server into a buffer
									# and put correct network packets to the thread-safe-queue


# scene changing
func _on_Quit_pressed():
	emit_signal("scene_changed", "start_menu")



func _ready():
	# Establish connection to server
	client_connection.connection.connect("connected", self, "_handle_connection_opened")
	add_child(client_connection)
	client_connection.open_connection()

	# Run consumer (thread that reads data from server)
	consumer.init(funcref(self, "_consumer_receive_data"))
	add_child(producer)
	add_child(consumer)

func _process(_delta):
#	Send player movements to server
	if (client_connection and client_connection.is_connected_to_host()): # means that we have made sucessfull handshake with the server
		# send actions to server
		producer.push_data(Player.get_player_move_request())
		producer.push_data(Player.get_player_shoot_request())
	
#	Receive data from server
	var received_packet = consumer.pop_data()
	if (received_packet == null):
		return
	
	match received_packet.message_type:
		Global.ResponseModels.PlayerInfoResponseModel:
			Player.set_player_info(received_packet)
		Global.ResponseModels.GameStateResponseModel:
			var new_game_state = GameStateResponseModel.GameStateResponseModel.new()
			var result_code = new_game_state.from_bytes(received_packet.get_bytes())
			if (result_code != GameStateResponseModel.PB_ERR.NO_ERRORS): 
				print("Error while receiving: ", "cannot unpack game update model")
			else:
				# update myself
				var players_positions = new_game_state.get_players()
				for i in range(0, players_positions.size()):
					var model = players_positions[i]
					if (model.get_player_id() == Player.player_id):
						Player.update_player_position(model)
						players_positions.erase(model) # erase ourselves
						break
				# update other players
				players_state_manager.update_players_states(players_positions, Player.team_id, players_parent_node)
				# update bullets
				bullets_state_manager.update_bullets_states(new_game_state.get_bullets(), bullets_parent_node)
		Global.ResponseModels.ShootingStateResponseModel:
			# Update our ammo count, gun reloading state
#			print("We shot a bullet!")
			pass
		_:
			print("Unknown message type: ", received_packet.message_type)



# producer
func _handle_connection_opened() -> void:
	producer.init(funcref(self, "_produce"))

func _produce(worker: Worker) -> void:
	while true:
		if (worker.queue.is_empty()):
			worker.semaphore.wait()

		worker.mutex.lock()
		var should_exit: bool = worker.exit_thread
		worker.mutex.unlock()

		if should_exit:
			break

		var network_packet = worker.queue.pop()
		if (!network_packet):
			print("Network packet is invalid: ", network_packet)
			continue
		if (!client_connection.send_packed_data(network_packet)):
			print("Error while sending packet: ", network_packet.get_bytes())


# consumer
func _consumer_receive_data(worker: Worker):
	client_connection.connection.connect("receive", self, "_handle_data_received", [worker])

func _handle_data_received(data: PoolByteArray, worker: Worker) -> void:
	client_connection.reader.add_data(data)
	var chunk: Array = client_connection.reader.get_next_packet_sequence()
	while (!chunk.empty()):
		var network_packet = client_connection._unpack_data(chunk)
		if (network_packet):
			worker.push_data(network_packet)
		chunk = client_connection.reader.get_next_packet_sequence()
	client_connection.reader.flush()




