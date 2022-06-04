extends KinematicBody2D

onready var animationTree = $Skin/Character/AnimationTree
onready var animationState = animationTree.get("parameters/playback")
onready var hitAnimationPlayer = $Skin/Character/Sprite/HitAnimation
onready var sprite = $Skin/Character/Sprite
onready var player_gun = $Gun
onready var player_skin = $Skin

const alien_skin = preload("res://player/alien_skin.tscn")
const human_skin = preload("res://player/human_skin.tscn")

const alien_stormtrooper_skin = preload("res://player/images/alien_stormtrooper.png")
const alien_sentinel_skin = preload("res://player/images/alien_sentinel.png")
const alien_support_skin = preload("res://player/images/alien_support.png")
const alien_medic_skin = preload("res://player/images/alien_medic.png")

const human_stormtrooper_skin = preload("res://player/images/human_stormtrooper.png")
const human_sentinel_skin = preload("res://player/images/human_sentinel.png")
const human_support_skin = preload("res://player/images/human_support.png")
const human_medic_skin = preload("res://player/images/human_medic.png")



var velocity = Vector2.ZERO
var player_id: int = -1
var team_id: int = -1
var player_specialization: int = -1
var username: String = ""

func set_specialization_skin():
	if (team_id == Global.TeamId.Humans):
		match player_specialization:
			Global.SpecializationTypes.Stormtrooper:
				sprite.set_texture(human_stormtrooper_skin)
			Global.SpecializationTypes.Sentinel:
				sprite.set_texture(human_sentinel_skin)
			Global.SpecializationTypes.Support:
				sprite.set_texture(human_support_skin)
			Global.SpecializationTypes.Medic:
				sprite.set_texture(human_medic_skin)
	else:
		match player_specialization:
			Global.SpecializationTypes.Stormtrooper:
				sprite.set_texture(alien_stormtrooper_skin)
			Global.SpecializationTypes.Sentinel:
				sprite.set_texture(alien_sentinel_skin)
			Global.SpecializationTypes.Support:
				sprite.set_texture(alien_support_skin)
			Global.SpecializationTypes.Medic:
				sprite.set_texture(alien_medic_skin)


func change_skin():
	var skin: Node2D = null
	if (team_id == Global.TeamId.Humans):
		skin = human_skin.instance()
	else:
		skin = alien_skin.instance()
	
	if (!player_skin.get_children().empty()):
		player_skin.get_children()[0].queue_free()
	
	player_skin.add_child(skin)
	
	animationTree = skin.get_node('AnimationTree')
	animationTree.active = true
	animationState = animationTree.get("parameters/playback")
	hitAnimationPlayer = skin.get_node('Sprite/HitAnimation')
	sprite = skin.get_node('Sprite')
	sprite.material.set_shader_param("hit_opacity", 0)
	
	set_specialization_skin()
	player_gun.change_skin(team_id, player_specialization)


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
	set_specialization_skin()
	player_gun.change_skin(team_id, player_specialization)

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













	


