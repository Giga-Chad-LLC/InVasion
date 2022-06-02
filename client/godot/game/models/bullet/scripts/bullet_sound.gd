extends Node2D

const sound = preload("res://models/bullet/single_bullet_sound.tscn")


func _process(delta):
	for old_sound in get_children():
		if (!old_sound.is_playing()):
			remove_child(old_sound)

func play_sound(team_id):
	var new_sound = sound.instance()	
	add_child(new_sound)
	new_sound.play(team_id)
	
