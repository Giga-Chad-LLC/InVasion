extends Control


signal toggle_respawn_menu(is_active)

var is_active = false setget set_is_active
onready var screen_title: Label = $LabelContainer/Title
onready var specialization_selector = $SpecializationSelector

func toggle(value, text = "You died!"):
	self.is_active = value
	screen_title.text = text
	emit_signal("toggle_respawn_menu", self.is_active)

# setters
func set_is_active(value):
	is_active = value
	visible = value
