extends Node2D

const sound = preload("res://models/bullet/single_bullet_sound.tscn")


func _clean_died(team_id):
	for child in get_children():
		if (!child.playing()):
			child.play(team_id)
			return
			

func play_sound(team_id):
	_clean_died(team_id)
	var new_sound = sound.instance()	
	add_child(new_sound)
	new_sound.play(team_id)

	
