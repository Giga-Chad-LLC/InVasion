extends Node
class_name PlayersStateManager

# scenes
var player_scene = preload("res://player/player_template.tscn")
var UI


var players_on_map: Array = [] # stored ints (ids of players in godot nodes hierarchy)
var players_data: Dictionary = {
	# id: { username, specialization, current_hitpoint, initial_hitpoints, team_id, kills, deaths }
}

func set_player_data(player_id, team_id):
	players_data[player_id] = {
		"username": "",
		"specialization": -1,
		"initial_hitpoints": 0,
		"current_hitpoints": 0,
		"team_id": team_id,
		"kills": 0,
		"deaths": 0,
	}

func remove_player_data(player_id):
	players_data.erase(player_id)

func set_players_data(data):
	for i in range(0, data.size()):
		players_data[data[i].get_player_id()] = {
			"username": data[i].get_username(),
			"specialization": data[i].get_specialization(),
			"initial_hitpoints": data[i].get_initial_hitpoints(),
			"current_hitpoints": data[i].get_current_hitpoints(),
			"team_id": data[i].get_team_id(),
			"kills": data[i].get_kills(),
			"deaths": data[i].get_deaths(),
		}

func spawn_player(player_id, local_team_id, players_parent_node, location):
	var spawned_player = Global.instance_node_at_location(player_scene, players_parent_node, location)
	spawned_player.name = str(player_id)
	spawned_player.player_id = player_id
	spawned_player.team_id = players_data[player_id].team_id
	spawned_player.reset_health_stats(players_data[player_id].current_hitpoints, players_data[player_id].initial_hitpoints)

	if (spawned_player.team_id != local_team_id):
		spawned_player.set_sprite_color(Color(1, 0.27, 0.27))
	else:
		spawned_player.set_sprite_color(Color(0.27, 1, 0.27))
	


func despawn_player(player_id, players_parent_node):
	if (players_parent_node and players_parent_node.has_node(str(player_id))):
		players_parent_node.get_node(str(player_id)).queue_free()
	else:
		print("Error: Cannot despawn player with id " + str(player_id) + ", players parent node is ", players_parent_node)


func update_players_states(player_positions: Array, main_player, players_parent_node):
	if (!players_parent_node):
		print("Error: Players parent node is ", players_parent_node)
		return
	
	var existing_players: Dictionary = {}
	
	for i in range(0, player_positions.size()):
		var model = player_positions[i]
		existing_players[model.get_player_id()] = true
		
		if (model.get_player_id() == main_player.player_id):
			main_player.update_player_position(model)
			# update specialization
			main_player.update_player_specialization(players_data[main_player.player_id].specialization)
		else:
			var player_id = model.get_player_id()
			
			if (players_parent_node.has_node(str(player_id))): 
				players_parent_node.get_node(str(player_id)).update_player_position(model)
			else:
				players_on_map.push_back(player_id)
				# we will respawn player and set his hp to maximum
				if (players_data[player_id].current_hitpoints == 0):
					players_data[player_id].initial_hitpoints = players_data[player_id].initial_hitpoints
					players_data[player_id].current_hitpoints = players_data[player_id].initial_hitpoints
				players_data[player_id].team_id = model.get_team_id()

				spawn_player(player_id, main_player.team_id, players_parent_node, Vector2(model.get_position().get_x(), model.get_position().get_y()))
			# update specialization
			var player = players_parent_node.get_node(str(player_id))
			player.update_player_specialization(players_data[player_id].specialization)
	
	# find players to delete from tree
	var valid_players = []
	for i in range(0, players_on_map.size()):
		if (existing_players.get(players_on_map[i], false)):
			valid_players.push_back(players_on_map[i])
		else:
			despawn_player(players_on_map[i], players_parent_node)
	players_on_map = valid_players

func change_player_gun_rotation(player_id, new_gun_rotation, main_player, players_parent_node):
	if (player_id != main_player.player_id):
		if (players_parent_node.has_node(str(player_id))):
			players_parent_node.get_node(str(player_id)).update_player_gun_rotation(
				Vector2(new_gun_rotation.get_x(), new_gun_rotation.get_y())
			)

func update_damaged_players_states(damaged_players: Array, main_player, players_parent_node):
	for i in range(damaged_players.size()):
		var damage_event = damaged_players[i]
		var attacker_id = damage_event.get_damaged_by()
		var defender_id = damage_event.get_player_id()
		var new_defender_hitpoints = damage_event.get_new_hitpoints()
		
		players_data[defender_id].current_hitpoints = new_defender_hitpoints
		
		if (defender_id == main_player.player_id):
			main_player.play_hit_animation()
			UI.get_node("HUD/HealthStats").update_current_hitpoints(new_defender_hitpoints)
		else:
			var other_player = players_parent_node.get_node(str(defender_id))
			if (other_player):
				other_player.play_hit_animation()
				other_player.update_current_hitpoints(new_defender_hitpoints)

func update_killed_players_states(killed_players: Array, main_player, players_parent_node):
	for i in range(killed_players.size()):
		var killed_player = killed_players[i]
		var killed_id = killed_player.get_player_id()
		
		players_data[killed_id].current_hitpoints = 0
		
		if (killed_id == main_player.player_id):
			main_player.set_is_dead(true) # player will disappear
			main_player.set_is_active(false) # Deactivate player (won't move the gun and send requests)
			UI.get_node("RespawnMenu").toggle(true) # Show respawn screen
			UI.get_node("HUD/HealthStats").update_current_hitpoints(0)
		else:
			despawn_player(killed_player.get_player_id(), players_parent_node)
#			print("Player ", killed_player.get_killed_by(), " killed player ", killed_player.get_player_id())

func get_killed_players_info(killed_players: Array, main_player, players_parent_node):
	var killed_player_info = [] # { player_id, team_id, killed_by_id, killed_by_team_id }
	for i in range(killed_players.size()):
		var killed_player = killed_players[i]
		var killed_player_id = killed_player.get_player_id()
		
		if (killed_player_id == main_player.player_id):
			killed_player_info.push_back({
				"player_id": killed_player_id,
				"team_id": main_player.team_id,
				"killed_by_id": killed_player.get_killed_by(),
				"killed_by_team_id": (main_player.team_id + 1) % 2,
			})
		else:
			var killed_player_node = players_parent_node.get_node(str(killed_player_id))
			if (killed_player_node):
				killed_player_info.push_back({
					"player_id": killed_player.get_player_id(),
					"team_id": killed_player_node.team_id,
					"killed_by_id": killed_player.get_killed_by(),
					"killed_by_team_id": (killed_player_node.team_id + 1) % 2,
				})
	
	return killed_player_info




func change_player_specialization(player_specialization, main_player, players_parent_node):
	var player_id = player_specialization.get_player_id()
	players_data[player_id].specialization = player_specialization.get_specialization()
	players_data[player_id].initial_hitpoints = player_specialization.get_initial_hitpoints()
	players_data[player_id].current_hitpoints = player_specialization.get_initial_hitpoints()
	
	if (player_id == main_player.player_id):
		main_player.update_player_specialization(player_id)
	else:
		if (players_parent_node.has_node(str(player_id))):
			players_parent_node.get_node(str(player_id)).update_player_specialization(player_specialization.get_specialization())

func set_players_usernames(usernames):
	for i in range(0, usernames.size()):
		var player_id = usernames[i].player_id
		players_data[player_id].username = usernames[i].username


func update_player_hitpoints(player_id, new_hitpoints, players_parent_node):
	if (players_data.has(player_id)):
		players_data[player_id].current_hitpoints = new_hitpoints
		if (players_parent_node.has_node(str(player_id))):
			players_parent_node.get_node(str(player_id)).update_current_hitpoints(new_hitpoints)
		else:
			print("Error when tried to update health bar of player who updated his HPs, player is not is the scene tree")
	else:
		print("Error when using updating single player HPs: player is not in the `players_data` dictionary")






