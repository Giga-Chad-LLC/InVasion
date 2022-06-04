extends Node


onready var flame_sound = $FlameSound

func _ready():
	flame_sound.play()


func _on_FlameSound_finished():
	flame_sound.play()
