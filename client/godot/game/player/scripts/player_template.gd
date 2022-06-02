#extends KinematicBody2D
extends "res://player/scripts/base_player.gd"

onready var HealthBar = $HealthBar

func update_health_stats(current_hitpoints, initial_hitpoints):
	HealthBar.update_health_stats(current_hitpoints, initial_hitpoints)


func update_current_hitpoints(new_current_hitpoints):
	HealthBar.update_current_hitpoints(new_current_hitpoints)

func reset_health_stats(new_current_hitpoints, new_initial_hitpoints):
	HealthBar.reset_health_stats(new_current_hitpoints, new_initial_hitpoints)
