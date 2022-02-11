extends Sprite


	
func create_grass_destroy_effect():
	# if melee attack happend -> destroy the grass inside a game
	#if (Input.is_action_just_pressed("melee_attack")):
	var grass_effect_scene = load("res://objects/grass/grass_effect.tscn")
	var grass_effect_instance = grass_effect_scene.instance()
	var world_scene = get_tree().current_scene # get the world scene inside a game
	
	world_scene.add_child(grass_effect_instance)
	grass_effect_instance.global_position = global_position
	
	queue_free()


func _on_Hurtbox_area_entered(_area):
	create_grass_destroy_effect()
	queue_free()
