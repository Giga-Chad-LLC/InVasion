extends Node2D


var gun_bullet = load("res://models/bullet/bullet.tscn")
var is_reloading = false
onready var shoot_point = $ShootPoint
onready var reload_timer = $ReloadTimer

func _physics_process(_delta):
	look_at(get_global_mouse_position())
	
	if (Input.is_action_pressed("shoot") and not is_reloading):
		shoot_bullet()


func instance_bullet():
	var gun_bullet_instance = Global.instance_node_at_location(gun_bullet, get_tree().get_root(), shoot_point.global_position)
#	gun_bullet_instance.name = "Bullet" + id
	gun_bullet_instance.gun_rotation = rotation

func shoot_bullet():
	instance_bullet()
	is_reloading = true
	reload_timer.start()

func _on_ReloadTimer_timeout():
	is_reloading = false
