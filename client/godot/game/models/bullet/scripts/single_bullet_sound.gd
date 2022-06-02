extends Node2D

const  human_bullet_sound = preload("res://models/bullet/sounds/human_gun_sound.wav")
const  alien_bullet_sound = preload("res://models/bullet/sounds/alien_gun_sound.wav")

onready var sound = $Sound

func play(team_id):
	if (team_id == 0):
		sound.stream = human_bullet_sound
	else:
		sound.stream = alien_bullet_sound
	sound.play()

func playing():
	return sound.playing
