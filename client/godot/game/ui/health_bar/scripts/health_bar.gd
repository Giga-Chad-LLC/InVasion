extends Node

onready var HealthBar = $Health

var initial_hitpoints: int = 0 setget set_initial_hitpoints
var current_hitpoints: int = 0 setget set_current_hitpoints

func update_current_hitpoints(new_current_hitpoints):
	set_current_hitpoints(new_current_hitpoints)

func reset_health_stats(new_current_hitpoints, new_initial_hitpoints):
	set_initial_hitpoints(new_initial_hitpoints)
	set_current_hitpoints(new_current_hitpoints)

func maximize_current_hitpoints():
	set_current_hitpoints(initial_hitpoints)

func set_initial_hitpoints(value):
	initial_hitpoints = value
	HealthBar.max_value = value

func set_current_hitpoints(value):
	current_hitpoints = value
	HealthBar.value = value
