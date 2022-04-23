extends Node2D

# Variables
onready var bullet_model = load("res://models/bullet/bullet.tscn")
onready var shoot_point = $ShootPoint
onready var cooldown_timer = $CooldownTimer
onready var bullet_spawning_node = get_tree().get_root().get_node("World/YSort/Bullets")
var is_cooldown = false


# User-defined functions
func instance_bullet(rotation: float):
	var bullet_model_instance = Global.instance_node_at_location(bullet_model,
							  bullet_spawning_node, shoot_point.global_position)
	bullet_model_instance.rotation = rotation

func start_cooldown():
#	instance_bullet(rotation)
	is_cooldown = true
	cooldown_timer.start()
#	emit_signal("shoot_weapon", Vector2(cos(global_rotation), sin(global_rotation)))


func _on_CooldownTimer_timeout():
	is_cooldown = false


func animate_gun():
	look_at(get_global_mouse_position())


