extends Control

signal scene_changed(scene_name)





func _on_LoginButton_pressed():
	emit_signal("scene_changed", "login")



func _on_RegistrateButton_pressed():
	emit_signal("scene_changed", "registrate")
