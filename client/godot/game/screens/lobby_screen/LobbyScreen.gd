extends Control

signal scene_changed(scene_name)


func _on_connectServerButton_pressed():
	emit_signal("scene_changed", "game")


func _on_ExitButton_pressed():
	emit_signal("scene_changed", "start_menu")
