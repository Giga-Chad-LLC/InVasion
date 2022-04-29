extends Control

signal scene_changed(scene_name)

onready var start_game_button = $CenterContainer/VBoxContainer/StartGameButton


func _on_StartGameButton_pressed():
	emit_signal("scene_changed", "game")


