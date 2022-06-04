extends Node

onready var flamelash_sound = $FlamelashSound



func _ready():
	flamelash_sound.play()



func _on_FlamelashSound_finished():
	flamelash_sound.play()
