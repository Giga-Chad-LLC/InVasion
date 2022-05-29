extends Control

signal scene_changed(scene_name)


func _on_LoginButton_pressed():
	emit_signal("scene_changed", "login_screen")


func _on_RegisterButton_pressed():
	emit_signal("scene_changed", "register_screen")


