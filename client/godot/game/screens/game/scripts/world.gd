extends Node2D

export(int) var left_bound = -1000
export(int) var right_bound = 1000
export(int) var up_bound = -1000
export(int) var down_bound = 1000

var player_scene = preload("res://player/player_template.tscn")
# Godobuf
const PlayersPositionsResponseModel = preload("res://proto/response-models/players_positions_response_model.gd")

func spawn_new_player(player_id, location):
	var parent = get_tree().get_root().get_node("World/YSort/OtherPlayers")
	var spawned_player = Global.instance_node_at_location(player_scene, parent, location)
	spawned_player.name = str(player_id)

func despawn_player(player_id):
	get_tree().get_root().get_node("World/YSort/OtherPlayers/" + str(player_id)).queue_free()


func update_game_state(game_state: Array):
	var parent_node = get_tree().get_root().get_node("World/YSort/OtherPlayers")
	
	for i in range(0, game_state.size()):
		var player: PlayersPositionsResponseModel.PlayerPositionResponseModel = game_state[i]
		if (parent_node.has_node(str(player.get_playerId()))): 
#			animate and move the player
			parent_node.get_node(str(player.get_playerId())).update_player_position(player)
		else:
			spawn_new_player(player.get_playerId(),
				Vector2(player.get_position().get_x(), player.get_position().get_y()))
