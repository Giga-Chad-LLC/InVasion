extends Node2D

export(int) var left_bound = -1000
export(int) var right_bound = 1000
export(int) var up_bound = -1000
export(int) var down_bound = 1000

var player_scene = preload("res://player/player_template.tscn")
var bullet_scene = preload("res://models/bullet/bullet.tscn")
# Godobuf
const PlayersPositionsResponseModel = preload("res://proto/response-models/players_positions_response_model.gd")
const BulletsPositionsResponseModel = preload("res://proto/response-models/bullets_positions_response_model.gd")
const GameStateResponseModel = preload("res://proto/response-models/game_state_response_model.gd")

func spawn_new_player(player_id, location):
	var parent = get_tree().get_root().get_node("World/YSort/OtherPlayers")
	var spawned_player = Global.instance_node_at_location(player_scene, parent, location)
	spawned_player.name = str(player_id)

func despawn_player(player_id):
	get_tree().get_root().get_node("World/YSort/OtherPlayers/" + str(player_id)).queue_free()

func despawn_bullet(bullet_id):
	get_tree().get_root().get_node("World/YSort/Bullets/" + str(bullet_id)).queue_free()

func spawn_new_bullet(bullet_id, player_id, location, velocity):
	var parent = get_tree().get_root().get_node("World/YSort/Bullets")
	var spawned_bullet = Global.instance_node_at_location(bullet_scene, parent, location)
	spawned_bullet.name = str(bullet_id)
	spawned_bullet.bullet_id = bullet_id
	spawned_bullet.player_id = player_id
	spawned_bullet.velocity = velocity
	spawned_bullet.rotation = velocity.angle()

func update_players_states(player_positions: Array):
	var parent_node = get_tree().get_root().get_node("World/YSort/OtherPlayers")
	
	for i in range(0, player_positions.size()):
		var player = player_positions[i]
		if (parent_node.has_node(str(player.get_player_id()))): 
			# animate and move the player
			parent_node.get_node(str(player.get_player_id())).update_player_position(player)
		else:
			spawn_new_player(player.get_player_id(),
				Vector2(player.get_position().get_x(), player.get_position().get_y()))

func update_bullets_states(bullets_positions: Array):
	var parent_node = get_tree().get_root().get_node("World/YSort/Bullets")
	for i in range(0, bullets_positions.size()):
		var bullet = bullets_positions[i]
		var bullet_node_name = str(bullet.get_bullet_id())
		if (parent_node.has_node(bullet_node_name)):
			# animate and move the bullet
			parent_node.get_node(bullet_node_name).update_bullet_position(bullet)
		else:
			spawn_new_bullet(bullet.get_bullet_id(), bullet.get_player_id(),
				Vector2(bullet.get_position().get_x(), bullet.get_position().get_y()),
				Vector2(bullet.get_velocity().get_x(), bullet.get_velocity().get_y()))






