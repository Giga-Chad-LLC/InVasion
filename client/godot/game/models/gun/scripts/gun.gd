extends Node2D

# Variables
onready var gun_sprite = $Sprite
onready var shoot_point = $ShootPoint
onready var cooldown_timer = $CooldownTimer
onready var bullet_spawning_node = get_node_or_null("YSort/Bullets")
onready var reloading_sound = $ReloadingSound
onready var empty_magazine_sound = $EmptyMagazineSound
onready var ShootingSounds = $ShootingSounds

const human_gun_sound = preload("res://models/gun/sounds/human_gun_sound.wav")
const alien_gun_sound = preload("res://models/gun/sounds/alien_gun_sound.wav")
const human_gun_0 = preload("res://models/gun/images/human_gun_0.png")
const alien_gun_0 = preload("res://models/gun/images/alien_gun_0.png")


var is_cooldown = false
var should_follow_mouse: bool = false
var new_gun_rotation: float = 0



func start_cooldown():
	is_cooldown = true
	cooldown_timer.start()


func change_skin(team_id):
	if (team_id == Global.TeamId.Humans):
		gun_sprite.set_texture(human_gun_0)
	else:
		gun_sprite.set_texture(alien_gun_0)

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
