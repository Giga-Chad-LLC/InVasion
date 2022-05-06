extends Control

signal scene_changed(scene_name)

onready var start_game_button = $CenterContainer/VBoxContainer/StartGameButton



func _on_LoginButton_pressed():
	emit_signal("scene_changed", "login")



func _on_RegistrateButton_pressed():
	emit_signal("scene_changed", "registrate")
