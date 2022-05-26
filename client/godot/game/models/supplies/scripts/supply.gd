extends Node

onready var sprite = $Sprite
onready var useAnimationPlayer = $Sprite/UseAnimation

var supply_id = -1
var supply_type = -1

func play_use_animation():
	print("Supply ", supply_id, " was used!")
	useAnimationPlayer.play("Use")


func _ready():
	if (sprite.material):
		sprite.material.set_shader_param("opacity", 0)
