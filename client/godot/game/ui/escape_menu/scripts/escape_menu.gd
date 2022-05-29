extends Control

signal toggle_escape_menu(is_active)
signal scene_changed(scene_name)

var is_active = false setget set_is_active


# `Escape` button pressed
func _unhandled_input(event):
	if event.is_action_pressed('escape'):
		toggle(!is_active)


func toggle(value):
	self.is_active = value
	emit_signal("toggle_escape_menu", self.is_active)


# setters
func set_is_active(value):
	is_active = value
	visible = value


# signals
func _on_Resume_pressed():
	self.is_active = false
	emit_signal("toggle_escape_menu", self.is_active)


func _on_Quit_pressed():
	emit_signal("scene_changed", "game_menu")
