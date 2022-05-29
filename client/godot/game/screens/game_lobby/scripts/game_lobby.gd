extends Control

signal scene_changed(scene_name)


func _on_ExitButton_pressed():
	emit_signal("scene_changed", "start_screen")


func _on_StartGameButton_pressed():
	emit_signal("scene_changed", "game")
