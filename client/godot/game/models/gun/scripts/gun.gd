extends Node2D

# Variables
onready var bullet_model = load("res://models/bullet/bullet.tscn")
onready var shoot_point = $ShootPoint
onready var cooldown_timer = $CooldownTimer
onready var bullet_spawning_node = get_node_or_null("YSort/Bullets")
onready var reloading_sound = $ReloadingSound
onready var empty_magazine_sound = $EmptyMagazineSound
const human_gun_sound = preload("res://models/gun/sounds/human_gun_sound.wav")
const alien_gun_sound = preload("res://models/gun/sounds/alien_gun_sound.wav")
onready var ShootingSounds = $ShootingSounds

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

func play_empty_magazine_sound():
	empty_magazine_sound.play()

func play_shooting_sound(team_id):
	if (team_id == Global.TeamId.Humans):
		var human_gun_sound_player = AudioStreamPlayer2D.new()
		human_gun_sound_player.stream = human_gun_sound
		ShootingSounds.add_child(human_gun_sound_player)
		human_gun_sound_player.connect("finished", self, "handle_shooting_sound_finished", [ human_gun_sound_player ])
		human_gun_sound_player.play()
	else:
		var alien_gun_sound_player = AudioStreamPlayer2D.new()
		alien_gun_sound_player.stream = alien_gun_sound
		ShootingSounds.add_child(alien_gun_sound_player)
		alien_gun_sound_player.connect("finished", self, "handle_shooting_sound_finished", [ alien_gun_sound_player ])
		alien_gun_sound_player.play()

func handle_shooting_sound_finished(audio_stream):
	# `audio_stream` points to the audio stream that needs to be deleted after the audio file finished playing
	audio_stream.queue_free()
