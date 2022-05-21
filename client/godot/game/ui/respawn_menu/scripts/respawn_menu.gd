extends Control


signal toggle_respawn_menu(is_active)

var is_active = false setget set_is_active

func toggle(value):
	self.is_active = value
	emit_signal("toggle_respawn_menu", self.is_active)

# setters
func set_is_active(value):
	is_active = value
	visible = value

