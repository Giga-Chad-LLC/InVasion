extends Node2D


# scenes
var player_scene = preload("res://player/player_template.tscn")
var bullet_scene = preload("res://models/bullet/bullet.tscn")
# Godobuf
const GameStateResponseModel = preload("res://proto/response-models/game_state_response_model.gd")
# Nodes
onready var bullets_parent_node = get_tree().get_root().get_node("SceneManager/World/YSort/Bullets")
onready var players_parent_node = get_tree().get_root().get_node("SceneManager/World/YSort/OtherPlayers")

func spawn_new_player(player_id, player_team_id, local_team_id, location):
	var spawned_player = Global.instance_node_at_location(player_scene, players_parent_node, location)
	spawned_player.name = str(player_id)
	if (player_team_id != local_team_id):
		print("Change spawned player color!")
		spawned_player.set_sprite_color(Color(1, 0.27, 0.27))

func despawn_player(player_id):
	if (players_parent_node and players_parent_node.has_node(str(player_id))):
		players_parent_node.get_node(str(player_id)).queue_free()
	else:
		print("Error: Players parent node is ", players_parent_node)

func update_players_states(player_positions: Array, team_id: int):
	if (!players_parent_node):
		print("Error: Players parent node is ", players_parent_node)
		return
	
	for i in range(0, player_positions.size()):
		var player = player_positions[i]
		if (players_parent_node.has_node(str(player.get_player_id()))): 
			# animate and move the player
			players_parent_node.get_node(str(player.get_player_id())).update_player_position(player)
		else:
			spawn_new_player(player.get_player_id(), player.get_team_id(), team_id,
				Vector2(player.get_position().get_x(), player.get_position().get_y()))

func despawn_bullet(bullet_id):
	if (bullets_parent_node and bullets_parent_node.has_node(str(bullet_id))):
		bullets_parent_node.get_node(str(bullet_id)).queue_free()
	else:
		print("Error: Bullets parent node is ", bullets_parent_node)

func spawn_new_bullet(bullet_id, player_id, location, velocity):
	var spawned_bullet = Global.instance_node_at_location(bullet_scene, bullets_parent_node, location)
	spawned_bullet.name = str(bullet_id)
	spawned_bullet.bullet_id = bullet_id
	spawned_bullet.player_id = player_id
	spawned_bullet.velocity = velocity
	spawned_bullet.rotation = velocity.angle()


var all_bullets_on_map: Array = [] # stored strings (names of bullets in godot nodes hierarchy)

func update_bullets_states(bullets_positions: Array):
	if (!bullets_parent_node):
		print("Error: Bullets parent node is ", bullets_parent_node)
		return
	
	var existing_bullets: Dictionary = {}
	
	for i in range(0, bullets_positions.size()):
		var bullet = bullets_positions[i]
		var bullet_node_name = str(bullet.get_bullet_id())
		existing_bullets[bullet_node_name] = true
		
		if (bullets_parent_node.has_node(bullet_node_name)):
			# animate and move the bullet
			bullets_parent_node.get_node(bullet_node_name).update_bullet_position(bullet)
		else:
			all_bullets_on_map.push_back(bullet_node_name)
			spawn_new_bullet(bullet.get_bullet_id(), bullet.get_player_id(),
				Vector2(bullet.get_position().get_x(), bullet.get_position().get_y()),
				Vector2(bullet.get_velocity().get_x(), bullet.get_velocity().get_y()))
	# find bullet to delete from tree
	var bullets_to_delete = []
	for i in range(0, all_bullets_on_map.size()):
		if (!existing_bullets.get(all_bullets_on_map[i], false)):
			bullets_to_delete.push_back(i)
	
	for i in range(0, bullets_to_delete.size()):
		despawn_bullet(all_bullets_on_map.pop_at(i))


