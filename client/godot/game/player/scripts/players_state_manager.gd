extends Reference

# scenes
var player_scene = preload("res://player/player_template.tscn")

# Godobuf
const DamagedPlayersResponseModel = preload("res://proto/response-models/damaged_players_response_model.gd")


class_name PlayersStateManager

func spawn_player(data, players_parent_node, location):
	var spawned_player = Global.instance_node_at_location(player_scene, players_parent_node, location)
	spawned_player.name = str(data['player_id'])
	if (data['player_team_id'] != data['local_team_id']):
		spawned_player.set_sprite_color(Color(1, 0.27, 0.27))
	else:
		spawned_player.set_sprite_color(Color(0.27, 1, 0.27))


func despawn_player(player_id, players_parent_node):
	if (players_parent_node and players_parent_node.has_node(str(player_id))):
		players_parent_node.get_node(str(player_id)).queue_free()
	else:
		print("Error: Cannot despawn, players parent node is ", players_parent_node)



func update_players_states(player_positions: Array, team_id: int, players_parent_node):
	if (!players_parent_node):
		print("Error: Players parent node is ", players_parent_node)
		return
	
	for i in range(0, player_positions.size()):
		var player = player_positions[i]
		if (players_parent_node.has_node(str(player.get_player_id()))): 
			# animate and move the player
			players_parent_node.get_node(str(player.get_player_id())).update_player_position(player)
		else:
			var data = {
				'player_id': player.get_player_id(),
				'player_team_id': player.get_team_id(),
				'local_team_id': team_id
			}
			spawn_player(data, players_parent_node, Vector2(player.get_position().get_x(), player.get_position().get_y()))


func update_damaged_players_states(damaged_players: Array, main_player, players_parent_node):
	for i in range(damaged_players.size()):
		var damage_event: DamagedPlayersResponseModel.DamagedPlayerResponseModel = damaged_players[i]
		var attacker = damage_event.get_damaged_by()
		var defender = damage_event.get_player_id()
		var new_defender_hitpoints = damage_event.get_new_hitpoints()
		
		












