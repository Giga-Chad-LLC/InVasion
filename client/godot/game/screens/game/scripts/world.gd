extends Node2D


signal scene_changed(scene_name)

# Nodes
onready var bullets_parent_node = $YSort/Bullets
onready var players_parent_node = $YSort/OtherPlayers
onready var supplies_parent_node = $YSort/Supplies
onready var Player = $YSort/Player
onready var UI = $UI
onready var RespawnMenu = $UI/RespawnMenu
onready var RespawnSpecializationSelector = $UI/RespawnMenu/SpecializationSelector

var PlayersStateManager = preload("res://player/scripts/players_state_manager.gd")
onready var players_state_manager = PlayersStateManager.new()
var BulletsStateManager = preload("res://models/bullet/scripts/bullets_state_manager.gd")
onready var bullets_state_manager = BulletsStateManager.new()
var SuppliesStateManager = preload("res://models/supplies/scripts/supplies_state_manager.gd")
onready var supplies_state_manager = SuppliesStateManager.new()


# Godobuf
const MoveRequestModel = preload("res://proto/request-models/move_request_model.gd")
const ShootRequestModel = preload("res://proto/request-models/shoot_request_model.gd")
const ChangePlayerSpecializationRequestModel = preload("res://proto/request-models/change_player_specialization_request_model.gd")

const PlayerPositionResponseModel = preload("res://proto/response-models/player_position_response_model.gd")
const GameStateResponseModel = preload("res://proto/response-models/game_state_response_model.gd")
const GameOverResponseModel = preload("res://proto/response-models/game_over_response_model.gd")
const PlayerSpecializationResponseModel = preload("res://proto/response-models/player_specialization_response_model.gd")
const SupplyResponseModel = preload("res://proto/response-models/supply_response_model.gd")
const HandshakeResponseModel = preload("res://proto/response-models/handshake_response_model.proto.gd")
const UpdatePlayerHitpointsResponseModel = preload("res://proto/response-models/update_player_hitpoints_response_model.gd")
const UpdatePlayerAmmoResponseModel = preload("res://proto/response-models/update_player_ammo_response_model.gd")
const UseSupplyResponseModel = preload("res://proto/response-models/use_supply_response_model.gd")

# Network
const Connection = preload("res://player/scripts/client_connection.gd")
var client_connection: Connection = Connection.new()

const Worker = preload("res://network/worker.gd")
var producer: Worker = Worker.new() # thread that stores events from client
var consumer: Worker = Worker.new() # thread that will read data from the server into a buffer
									# and put correct network packets to the thread-safe-queue
var is_game_running = true

# scene/ui changing
func _on_Quit_pressed():
	emit_signal("scene_changed", "start_menu")

# disable player movements when escape menu is opened
func _on_EscapeMenu_toggle_escape_menu(is_escaped):
	if (is_game_running): # if the game has not ended yet
		Player.set_is_active(!is_escaped)

# player want to respawn - send required request model for that
func _on_RespawnButton_pressed():
	if (client_connection and client_connection.is_connected_to_host() and producer):
		 # will be null if specialization did not change
		var spec_packet = Player.get_player_specialization_request(RespawnSpecializationSelector.selected_specialization)
		if (spec_packet):
			producer.push_data(spec_packet)
		producer.push_data(Player.get_respawn_player_request())


func _ready():
	# Establish connection to server
	client_connection.connection.connect("connected", self, "_handle_connection_opened")
	add_child(client_connection)
	client_connection.open_connection()

	# Run consumer (thread that reads data from server)
	consumer.init(funcref(self, "_consumer_receive_data"))
	add_child(producer)
	add_child(consumer)
	
	# attach UI to the players state manager
	players_state_manager.UI = UI


func _process(_delta):
#	Send player movements to server
	if (client_connection and client_connection.is_connected_to_host() and Player.is_active): # means that we have made sucessfull handshake with the server
		# send actions to server
		producer.push_data(Player.get_player_move_request())
		producer.push_data(Player.get_player_shoot_request())
		producer.push_data(Player.get_apply_ability_request())
		producer.push_data(Player.get_use_supply_request())
	
#	Receive data from server
	var received_packet = consumer.pop_data()
	if (received_packet == null):
		return
	
	match received_packet.message_type:
		Global.ResponseModels.HandshakeResponseModel: 
			var handshake_model = HandshakeResponseModel.HandshakeResponseModel.new()
			var result_code = handshake_model.from_bytes(received_packet.get_bytes())
			if (result_code != HandshakeResponseModel.PB_ERR.NO_ERRORS):
				print("Error while receiving: ", "cannot unpack handshake")
			else:
				# we only know team_id and player_id (we need specialization as well)
				Player.set_player_info(
					handshake_model.get_player_id(),
					handshake_model.get_team_id()
				)
				supplies_state_manager.update_supplies_states(handshake_model.get_supplies(), supplies_parent_node)
				# set player specialization (as default for now)
				print("We set player info, send default specialization: ", Global.SpecializationTypes.Stormtrooper)
	#			producer.push_data(Player.get_player_specialization_request(Global.SpecializationTypes.Stormtrooper))
				RespawnMenu.toggle(true, "Select specialization")
		Global.ResponseModels.PlayerSpecializationResponseModel:
			var new_player_specialization = PlayerSpecializationResponseModel.PlayerSpecializationResponseModel.new()
			var result_code = new_player_specialization.from_bytes(received_packet.get_bytes())
			if (result_code != PlayerSpecializationResponseModel.PB_ERR.NO_ERRORS): 
				print("Error while receiving: ", "cannot unpack player specialization model")
			
			players_state_manager.change_player_specialization(
				new_player_specialization.get_player_id(),
				new_player_specialization.get_specialization(),
				Player,
				players_parent_node
			)
			if (new_player_specialization.get_player_id() == Player.player_id and !Player.is_dead):
				Player.set_is_active(true)
				Player.visible = true
		Global.ResponseModels.GameStateResponseModel:
			var new_game_state = GameStateResponseModel.GameStateResponseModel.new()
			var result_code = new_game_state.from_bytes(received_packet.get_bytes())
			if (result_code != GameStateResponseModel.PB_ERR.NO_ERRORS): 
				print("Error while receiving: ", "cannot unpack game update model")
			else:
				# update other players
				players_state_manager.update_players_states(new_game_state.get_players(), Player, players_parent_node)
				# update damaged players
				players_state_manager.update_damaged_players_states(new_game_state.get_damaged_players(), Player, players_parent_node)
				# update illed players
				players_state_manager.update_killed_players_states(new_game_state.get_killed_players(), Player, players_parent_node)
				# update bullets
				bullets_state_manager.update_bullets_states(new_game_state.get_bullets(), bullets_parent_node)
		Global.ResponseModels.SupplyResponseModel:
			var new_supplies_state = SupplyResponseModel.SupplyResponseModel.new()
			var result_code = new_supplies_state.from_bytes(received_packet.get_bytes())
			if (result_code != GameStateResponseModel.PB_ERR.NO_ERRORS): 
				print("Error while receiving: ", "cannot unpack game update model")
			else:
				supplies_state_manager.update_supplies_states([new_supplies_state.get_supply()], supplies_parent_node)
		Global.ResponseModels.ShootingStateResponseModel:
			# Update our ammo count, gun reloading state
			# print("We shot a bullet!")
			pass
		Global.ResponseModels.UseSupplyResponseModel:
			print("Someone used supply!")
			var used_supply = UseSupplyResponseModel.UseSupplyResponseModel.new()
			var result_code = used_supply.from_bytes(received_packet.get_bytes())
			if (result_code != UseSupplyResponseModel.PB_ERR.NO_ERRORS): 
				print("Error while receiving: ", "cannot unpack use supply model")
			else:
				var supply_name
				if (used_supply.get_applied_supply_type() == Global.SupplyType.AidKit):
					supply_name = "AidKit"
				elif(used_supply.get_applied_supply_type() == Global.SupplyType.AmmoCrate):
					supply_name = "AmmoCrate"
				
				print("Player ", used_supply.get_player_id(),
					  " used supply ", used_supply.get_supply_id(), " with type '", supply_name, "'")
				print("Left supply capacity: ", used_supply.get_left_supply_capacity(), ", is active: ", used_supply.get_is_active())
		Global.ResponseModels.UpdatePlayerAmmoResponseModel:
			var new_ammo = UpdatePlayerAmmoResponseModel.UpdatePlayerAmmoResponseModel.new()
			var result_code = new_ammo.from_bytes(received_packet.get_bytes())
			if (result_code != UpdatePlayerAmmoResponseModel.PB_ERR.NO_ERRORS): 
				print("Error while receiving: ", "cannot unpack update player ammo model")
			else:
				print("Our new ammo capacity: ", new_ammo.get_new_ammo())
		Global.ResponseModels.UpdatePlayerHitpointsResponseModel:
			var new_hitpoints = UpdatePlayerHitpointsResponseModel.UpdatePlayerHitpointsResponseModel.new()
			var result_code = new_hitpoints.from_bytes(received_packet.get_bytes())
			if (result_code != UpdatePlayerHitpointsResponseModel.PB_ERR.NO_ERRORS): 
				print("Error while receiving: ", "cannot unpack update player hitpoints model")
			else:
				print("Update our hitpoints", "New hitpoints: ", new_hitpoints.get_new_hitpoints())
		Global.ResponseModels.GameOverResponseModel:
			print("Game over!")
			# Stop the client and show the results table
			client_connection.close_connection()
			is_game_running = false
			Player.set_is_active(false)
		Global.ResponseModels.RespawnPlayerResponseModel:
			Player.set_is_dead(false)
			Player.set_is_active(true)
			RespawnMenu.toggle(false)
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





