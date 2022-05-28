extends KinematicBody2D

onready var animationPlayer = $AnimationPlayer
onready var animationTree = $AnimationTree
onready var animationState = animationTree.get("parameters/playback")
onready var hitAnimationPlayer = $Sprite/HitAnimation
onready var sprite = $Sprite
onready var player_gun = $Gun

var velocity = Vector2.ZERO
var player_id: int = -1
var team_id: int = -1
var player_specialization: int = -1
var username: String = ""

#var initial_hitpoints: int = 0
#var current_hitpoints: int = 0
#var magazine: int = 0
#var ammo: int = 0


func set_sprite_color(color: Color):
	sprite.modulate = color

func animate_player():
	#	Move the player
	if velocity != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", velocity)
		animationTree.set("parameters/Run/blend_position", velocity)

		animationState.travel("Run")
	else:
		animationState.travel("Idle")
	move_and_slide(velocity)


func update_player_position(player_state_model):
	velocity = Vector2(player_state_model.get_velocity().get_x(), player_state_model.get_velocity().get_y())
	global_position = Vector2(player_state_model.get_position().get_x(), player_state_model.get_position().get_y())

func update_player_specialization(new_player_specialization):
	player_specialization = new_player_specialization
#	print("Set specialization of player ", player_id, " to ", player_specialization)

func update_player_gun_rotation(direction):
	player_gun.set_gun_rotation(direction)

func play_hit_animation():
	hitAnimationPlayer.play("Hit")


func _process(_delta):
	animate_player()

func _ready():
	# Activate animation for the character
	animationTree.active = true
	sprite.material.set_shader_param("hit_opacity", 0)













	


