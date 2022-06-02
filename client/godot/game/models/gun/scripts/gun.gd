extends Node2D

# Variables
onready var bullet_model = load("res://models/bullet/bullet.tscn")
onready var shoot_point = $ShootPoint
onready var cooldown_timer = $CooldownTimer
onready var bullet_spawning_node = get_node_or_null("YSort/Bullets")
onready var reloading_sound = $ReloadingSound

var is_cooldown = false
var should_follow_mouse: bool = false
var new_gun_rotation: float = 0


func instance_bullet(rotation: float):
	var bullet_model_instance = Global.instance_node_at_location(
		bullet_model,
		bullet_spawning_node,
		shoot_point.global_position
	)
	bullet_model_instance.rotation = rotation

func start_cooldown():
	is_cooldown = true
	cooldown_timer.start()


func _on_CooldownTimer_timeout():
	is_cooldown = false

func _process(_delta):
	if (should_follow_mouse):
		look_at(get_global_mouse_position())
	else:
		# here are the other players, we want to animate their guns rotations
		global_rotation = lerp_angle(global_rotation, new_gun_rotation, 0.1)


func set_gun_rotation(direction: Vector2):
	new_gun_rotation = direction.angle()
	
func get_gun_rotation():
	return Vector2(cos(global_rotation), sin(global_rotation))

func play_reloading_sound():
	reloading_sound.play()
