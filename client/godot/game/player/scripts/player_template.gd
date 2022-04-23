extends KinematicBody2D

# Animations
onready var animationPlayer = $AnimationPlayer
onready var animationTree = $AnimationTree
onready var sprite = $Sprite
onready var animationState = animationTree.get("parameters/playback")
var velocity = Vector2.ZERO

# Godobuf
const GameStateResponseModel = preload("res://proto/response-models/game_state_response_model.gd")


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


func update_player_position(player_position_model):
	velocity = Vector2(player_position_model.get_velocity().get_x(), player_position_model.get_velocity().get_y())
	global_position = Vector2(player_position_model.get_position().get_x(), player_position_model.get_position().get_y())

func _ready():
	animationTree.active = true

func _process(_delta):
	animate_player()
