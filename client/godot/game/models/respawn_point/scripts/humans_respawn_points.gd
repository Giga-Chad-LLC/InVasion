extends Node2D


func _ready():
	if (Global.STORE_RESPAWN_POINTS_IN_FILE):
		Global.save_humans_respawn_points_in_file(self)
