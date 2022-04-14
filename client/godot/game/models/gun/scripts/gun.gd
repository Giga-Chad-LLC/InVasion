extends Node2D

# Variables
onready var bullet_model = load("res://models/bullet/bullet.tscn")
onready var shoot_point = $ShootPoint
onready var reload_timer = $ReloadTimer
onready var bullet_spawning_node = get_tree().get_root().get_node("World/YSort/Bullets")
var is_reloading = false


# User-defined functions
func instance_bullet():
	var bullet_model_instance = Global.instance_node_at_location(bullet_model,
							  bullet_spawning_node, shoot_point.global_position)
	bullet_model_instance.initial_rotation = global_rotation

func shoot_bullet():
	instance_bullet()
	is_reloading = true
	reload_timer.start()

func _on_ReloadTimer_timeout():
	is_reloading = false

func rotate_gun_to_mouse_cursor():
	look_at(get_global_mouse_position())

func animate_gun():
	rotate_gun_to_mouse_cursor()
	
	if (Input.is_action_pressed("shoot") and not is_reloading):
		shoot_bullet()
