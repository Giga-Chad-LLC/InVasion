extends Control

signal toggle_escape_menu(is_active)
signal scene_changed(scene_name)

var is_active = false setget set_is_active
onready var click_sound = $ClickSound

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
	click_sound.play()
	self.is_active = false
	emit_signal("toggle_escape_menu", self.is_active)


func _on_Quit_pressed():
	click_sound.play()
	emit_signal("scene_changed", "game_lobby")
