extends Reference

# scenes
var bullet_scene = preload("res://models/bullet/bullet.tscn")


class_name BulletsStateManager

func spawn_bullet(data, bullets_parent_node, location):
	# data = { bullet_id, player_id, velocity }
	var spawned_bullet = Global.instance_node_at_location(bullet_scene, bullets_parent_node, location)
	spawned_bullet.name = str(data['bullet_id'])
	spawned_bullet.bullet_id = data['bullet_id']
	spawned_bullet.player_id = data['player_id']
	spawned_bullet.velocity = data['velocity']
	spawned_bullet.rotation = data['velocity'].angle()


func despawn_bullet(bullet_id, bullets_parent_node):
	if (bullets_parent_node and bullets_parent_node.has_node(str(bullet_id))):
		bullets_parent_node.get_node(str(bullet_id)).queue_free()
	else:
		print("Error: Bullets parent node is ", bullets_parent_node)



var all_bullets_on_map: Array = [] # stored strings (names of bullets in godot nodes hierarchy)

func update_bullets_states(bullets_positions: Array, bullets_parent_node):
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
			var data = {
				'bullet_id': bullet.get_bullet_id(),
				'player_id': bullet.get_player_id(),
				'velocity': Vector2(bullet.get_velocity().get_x(), bullet.get_velocity().get_y())
			}
			spawn_bullet(data, bullets_parent_node, Vector2(bullet.get_position().get_x(), bullet.get_position().get_y()))
		
	# find bullet to delete from tree
	var bullets_to_delete = []
	for i in range(0, all_bullets_on_map.size()):
		if (!existing_bullets.get(all_bullets_on_map[i], false)):
			bullets_to_delete.push_back(i)
	
	for i in range(0, bullets_to_delete.size()):
		despawn_bullet(all_bullets_on_map.pop_at(i), bullets_parent_node)
