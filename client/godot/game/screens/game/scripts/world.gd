extends Node2D


signal scene_changed(scene_name)

# Nodes
onready var battle_music = $BattleMusic
onready var bullets_parent_node = $YSort/Bullets
onready var players_parent_node = $YSort/OtherPlayers
onready var supplies_parent_node = $YSort/Supplies
onready var Player = $YSort/Player
onready var UI = $UI
onready var RespawnMenu = $UI/RespawnMenu
onready var RespawnSpecializationSelector = $UI/RespawnMenu/SpecializationSelector
onready var SessionTimer = $UI/HUD/Timer
onready var TeamsScore = $UI/HUD/TeamsScore
onready var AmmoStats = $UI/HUD/AmmoStats
onready var HealthStats = $UI/HUD/HealthStats
onready var Leaderboard = $UI/Leaderboard


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
const HandshakeResponseModel = preload("res://proto/response-models/handshake_response_model.gd")
const UpdatePlayerHitpointsResponseModel = preload("res://proto/response-models/update_player_hitpoints_response_model.gd")
const UpdatePlayerAmmoResponseModel = preload("res://proto/response-models/update_player_ammo_response_model.gd")
const UseSupplyResponseModel = preload("res://proto/response-models/use_supply_response_model.gd")
const WeaponDirectionResponseModel = preload("res://proto/response-models/weapon_direction_response_model.gd")
const WeaponStateResponseModel = preload("res://proto/response-models/weapon_state_response_model.proto.gd")
const UsernameResponseModel = preload("res://proto/response-models/username_response_model.gd")
const ClientConnectedResponseModel = preload("res://proto/response-models/client_connected_response_model.gd")
const ClientDisconnectedResponseModel = preload("res://proto/response-models/client_disconnected_response_model.gd")

# Network
const Connection = preload("res://player/scripts/client_connection.gd")
var client_connection: Connection = Connection.new()

const Worker = preload("res://network/worker.gd")
var producer: Worker = Worker.new() # thread that stores events from client
var consumer: Worker = Worker.new() # thread that will read data from the server into a buffer
									# and put correct network packets to the thread-safe-queue
var is_game_running = true

# for debugging purposes
func _unhandled_input(event):
	if (event.is_action_pressed("print_info")):
		print(Player.player_specialization)
		print(players_state_manager.players_data)



# scene/ui changing
func _on_Quit_pressed():
	emit_signal("scene_changed", "game_lobby")

# disable player movements when escape menu is opened
func _on_EscapeMenu_toggle_escape_menu(is_escaped):
	if (is_game_running): # if the game has not ended yet
		Player.set_is_active(!is_escaped)

# player wants to respawn - send required request model for that
func _on_RespawnButton_pressed():
	if (client_connection and client_connection.is_connected_to_host() and producer):
		 # will be null if specialization did not change
		var spec_packet = Player.get_player_specialization_request(RespawnSpecializationSelector.selected_specialization)
		if (spec_packet):
			producer.push_data(spec_packet)
		producer.push_data(Player.get_respawn_player_request())


func _ready():
	# Play battle music
	battle_music.play()
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
		var move_request = Player.get_player_move_request()
		if (move_request):
			producer.push_data(move_request)
		var shoot_request = Player.get_player_shoot_request()
		if (shoot_request):
			producer.push_data(shoot_request)
		var apply_ability_request = Player.get_apply_ability_request()
		if (apply_ability_request):
			producer.push_data(apply_ability_request)
		var use_supply_request = Player.get_use_supply_request()
		if (use_supply_request):
			producer.push_data(use_supply_request)
		var gun_rotation_request = Player.get_player_gun_rotation_request()
		if (gun_rotation_request):
			producer.push_data(gun_rotation_request)
		var reload_gun_request = Player.get_reload_gun_request()
		if (reload_gun_request):
			producer.push_data(reload_gun_request)
	
#	Receive data from server
	var received_packet = consumer.pop_data()
	if (received_packet == null):
		return
	
	match received_packet.message_type:
		Global.ResponseModels.HandshakeResponseModel: 
			print("Our access token: ", Global.access_token)
			Player.username = Global.username
			var handshake_model = HandshakeResponseModel.HandshakeResponseModel.new()
			var result_code = handshake_model.from_bytes(received_packet.get_bytes())
			if (result_code != HandshakeResponseModel.PB_ERR.NO_ERRORS):
				print("Error while receiving: ", "cannot unpack handshake")
			else:
				# here we only know team_id and player_id (we need specialization as well)
				Player.set_player_info(
					handshake_model.get_player_id(),
					handshake_model.get_team_id()
				)
				Player.change_skin()
				
				# set main player id in a leaderboard
				Leaderboard.set_main_player_id(handshake_model.get_player_id())
				
				# set usernames
				players_state_manager.set_player_data(Player.player_id, Player.team_id)
				players_state_manager.set_players_data(handshake_model.get_players_data())
				# update supplies positions
				supplies_state_manager.update_supplies_states(handshake_model.get_supplies(), supplies_parent_node)
				# set player specialization
				SessionTimer.start(int(handshake_model.get_remaining_session_time_ms() / 1000))
				TeamsScore.set_teams_score(
					handshake_model.get_first_team_kills_count(),
					handshake_model.get_second_team_kills_count(),
					Player.team_id
				)
				
				# set data in the leaderboard
				var players_on_map = handshake_model.get_players_data()
				for i in range(0, players_on_map.size()):
					var player_id = players_on_map[i].get_player_id()
					var username = players_on_map[i].get_username()
					var team_id = players_on_map[i].get_team_id()
					var kills = players_on_map[i].get_kills()
					var deaths = players_on_map[i].get_deaths()
					Leaderboard.add_user(player_id, username, team_id, Player.team_id)
					Leaderboard.add_kills(player_id, kills)
					Leaderboard.add_deaths(player_id, deaths)
				
				RespawnMenu.toggle(true, "Select specialization")
				# send our credencials
				producer.push_data(Player.get_client_credentials_model())
		Global.ResponseModels.ClientConnectedResponseModel:
			var client_connected_model = ClientConnectedResponseModel.ClientConnectedResponseModel.new()
			var result_code = client_connected_model.from_bytes(received_packet.get_bytes())
			if (result_code != ClientConnectedResponseModel.PB_ERR.NO_ERRORS):
				print("Error while receiving: ", "cannot unpack player connected model")
			else:
				var player_id = client_connected_model.get_player_id()
				print("Client ", player_id, " joined the match!")
				players_state_manager.set_player_data(player_id, client_connected_model.get_team_id())
		Global.ResponseModels.ClientDisconnectedResponseModel:
			var client_disconnected_model = ClientDisconnectedResponseModel.ClientDisconnectedResponseModel.new()
			var result_code = client_disconnected_model.from_bytes(received_packet.get_bytes())
			if (result_code != ClientDisconnectedResponseModel.PB_ERR.NO_ERRORS):
				print("Error while receiving: ", "cannot unpack player connected model")
			else:
				var player_id = client_disconnected_model.get_player_id()
				print("Client ", player_id, " left the match!")
				Leaderboard.remove_user(player_id)
				players_state_manager.remove_player_data(player_id)
		Global.ResponseModels.UsernameResponseModel:
			var username_model = UsernameResponseModel.UsernameResponseModel.new()
			var result_code = username_model.from_bytes(received_packet.get_bytes())
			if (result_code != UsernameResponseModel.PB_ERR.NO_ERRORS):
				print("Error while receiving: ", "cannot unpack username model")
			else:
				var player_id = username_model.get_player_id()
				var team_id = players_state_manager.players_data[player_id].team_id
				var username = username_model.get_username()
				players_state_manager.set_players_usernames([{
					"player_id": player_id,
					"username": username
				}])
				Leaderboard.add_user(player_id, username, team_id, Player.team_id)
		Global.ResponseModels.PlayerSpecializationResponseModel:
			var new_player_specialization = PlayerSpecializationResponseModel.PlayerSpecializationResponseModel.new()
			var result_code = new_player_specialization.from_bytes(received_packet.get_bytes())
			if (result_code != PlayerSpecializationResponseModel.PB_ERR.NO_ERRORS): 
				print("Error while receiving: ", "cannot unpack player specialization model")
			
			players_state_manager.change_player_specialization(
				new_player_specialization,
				Player,
				players_parent_node
			)
			
			if (new_player_specialization.get_player_id() == Player.player_id and !Player.is_dead):
				Player.set_is_active(true)
				Player.visible = true
				Player.reset_ammo_stats(new_player_specialization.get_ammo(), new_player_specialization.get_magazine())
				AmmoStats.reset_ammo_stats(new_player_specialization.get_ammo(), new_player_specialization.get_magazine())
				HealthStats.reset_health_stats(new_player_specialization.get_initial_hitpoints(), new_player_specialization.get_initial_hitpoints())
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
				bullets_state_manager.update_bullets_states(new_game_state.get_bullets(), bullets_parent_node, players_state_manager, Player, players_parent_node)
				# update scores
				if (new_game_state.get_killed_players() and !new_game_state.get_killed_players().empty()):
					var killed_players_info = players_state_manager.get_killed_players_info(
						new_game_state.get_killed_players(),
						Player,
						players_parent_node
					)
					
					TeamsScore.update_teams_score(
						killed_players_info,
						Player.team_id
					)
					
					for i in range(0, killed_players_info.size()):
						var player_id = killed_players_info[i].player_id
						var team_id = killed_players_info[i].team_id
						var killed_by_id = killed_players_info[i].killed_by_id
						var killed_by_team_id = killed_players_info[i].killed_by_team_id
						Leaderboard.add_kills(killed_by_id, 1)
						Leaderboard.add_deaths(player_id, 1)
					
		Global.ResponseModels.SupplyResponseModel:
			var new_supplies_state = SupplyResponseModel.SupplyResponseModel.new()
			var result_code = new_supplies_state.from_bytes(received_packet.get_bytes())
			if (result_code != GameStateResponseModel.PB_ERR.NO_ERRORS): 
				print("Error while receiving: ", "cannot unpack game update model")
			else:
				supplies_state_manager.update_supplies_states([new_supplies_state.get_supply()], supplies_parent_node)
		Global.ResponseModels.WeaponStateResponseModel:
			# Update our ammo count, gun reloading state
			var new_weapon_state = WeaponStateResponseModel.WeaponStateResponseModel.new()
			var result_code = new_weapon_state.from_bytes(received_packet.get_bytes())
			if (result_code != WeaponStateResponseModel.PB_ERR.NO_ERRORS):
				print("Error while receiving: ", "cannot unpack weapon state model")
			else:
				Player.set_is_reloading(new_weapon_state.get_is_reloading())
				Player.update_ammo_stats(new_weapon_state.get_left_ammo(), new_weapon_state.get_left_magazine())
				AmmoStats.update_ammo_stats(new_weapon_state.get_left_ammo(), new_weapon_state.get_left_magazine())
				if (new_weapon_state.get_is_reloading_required()):
					Player.player_gun.play_empty_magazine_sound()
		Global.ResponseModels.WeaponDirectionResponseModel:
			var player_weapon_direction = WeaponDirectionResponseModel.WeaponDirectionResponseModel.new()
			var result_code = player_weapon_direction.from_bytes(received_packet.get_bytes())
			if (result_code != WeaponDirectionResponseModel.PB_ERR.NO_ERRORS):
				print("Error while receiving: ", "cannot unpack weapon direction model")
			else:
				players_state_manager.change_player_gun_rotation(
					player_weapon_direction.get_player_id(),
					player_weapon_direction.get_direction(),
					Player,
					players_parent_node
				)
		Global.ResponseModels.UseSupplyResponseModel:
			var used_supply = UseSupplyResponseModel.UseSupplyResponseModel.new()
			var result_code = used_supply.from_bytes(received_packet.get_bytes())
			if (result_code != UseSupplyResponseModel.PB_ERR.NO_ERRORS): 
				print("Error while receiving: ", "cannot unpack use supply model")
			else:
				# update used supply
				supplies_state_manager.update_used_supply(
					used_supply.get_supply_id(),
					supplies_parent_node
				)
		
		Global.ResponseModels.UpdatePlayerAmmoResponseModel:
			var new_ammo = UpdatePlayerAmmoResponseModel.UpdatePlayerAmmoResponseModel.new()
			var result_code = new_ammo.from_bytes(received_packet.get_bytes())
			if (result_code != UpdatePlayerAmmoResponseModel.PB_ERR.NO_ERRORS): 
				print("Error while receiving: ", "cannot unpack update player ammo model")
			else:
				Player.update_ammo_stats(new_ammo.get_new_ammo())
				AmmoStats.update_ammo_stats(new_ammo.get_new_ammo())
		Global.ResponseModels.UpdatePlayerHitpointsResponseModel:
			var new_hitpoints = UpdatePlayerHitpointsResponseModel.UpdatePlayerHitpointsResponseModel.new()
			var result_code = new_hitpoints.from_bytes(received_packet.get_bytes())
			if (result_code != UpdatePlayerHitpointsResponseModel.PB_ERR.NO_ERRORS): 
				print("Error while receiving: ", "cannot unpack update player hitpoints model")
			else:
				if (new_hitpoints.get_player_id() == Player.player_id):
					print("We used aid kit, new HP: ", new_hitpoints.to_string())
					HealthStats.update_current_hitpoints(new_hitpoints.get_new_hitpoints())
				else:
					players_state_manager.update_player_hitpoints(
						new_hitpoints.get_player_id(),
						new_hitpoints.get_new_hitpoints(),
						players_parent_node
					)
		Global.ResponseModels.GameOverResponseModel:
			print("Game over!")
			# show leaderboard when match ended
			Leaderboard.is_active = false
			Leaderboard.visible = true
			# Stop the client and show the results table
			client_connection.close_connection()
			is_game_running = false
			Player.set_is_active(false)
		Global.ResponseModels.RespawnPlayerResponseModel:
			Player.set_is_dead(false)
			Player.set_is_active(true)
			RespawnMenu.toggle(false)
			HealthStats.maximize_current_hitpoints() # uses memorized default HPs
			Player.maximize_magazine()
			AmmoStats.maximize_magazine() # uses memorized Ammo
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
#			print("Network packet is invalid: ", network_packet)
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





