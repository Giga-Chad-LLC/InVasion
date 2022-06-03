extends Control

signal scene_changed(scene_name)
signal play_click_sound()


func _on_LoginButton_pressed():
	emit_signal("play_click_sound")
	emit_signal("scene_changed", "login_screen")


func _on_RegisterButton_pressed():
	emit_signal("play_click_sound")
	emit_signal("scene_changed", "register_screen")


