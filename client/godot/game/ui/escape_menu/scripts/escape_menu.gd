extends Control


var is_escaped = false setget set_is_escaped



func _unhandled_input(event):
	if event.is_action_pressed('escape'):
		self.is_escaped = !is_escaped

func set_is_escaped(value):
	is_escaped = value
#	get_tree().paused = is_escaped # ???
	visible = is_escaped


func _on_Resume_pressed():
	self.is_escaped = false
