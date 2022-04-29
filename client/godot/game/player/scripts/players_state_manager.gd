extends Reference

# scenes
var player_scene = preload("res://player/player_template.tscn")

class_name PlayersStateManager

func spawn_player(data, players_parent_node, location):
	var spawned_player = Global.instance_node_at_location(player_scene, players_parent_node, location)
	spawned_player.name = str(data['player_id'])
	if (data['player_team_id'] != data['local_team_id']):
		spawned_player.set_sprite_color(Color(1, 0.27, 0.27))


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
