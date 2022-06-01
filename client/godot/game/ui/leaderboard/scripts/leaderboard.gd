extends Control

onready var AllyTable = $Container/AllyTeam
onready var FoeTable = $Container/FoeTeam
var is_active: = true


func _ready():
	set_process_input(true)

func _input(event):
	if event.is_action_released("leaderboard"):
		if (is_active):
			visible = false
	elif event.is_action_pressed("leaderboard"):
		if (is_active):
			visible = true

# Ally
func add_user(id: int, username: String, team_id: int, local_team_id: int):
	if (team_id == local_team_id):
		AllyTable.add_user(id, username)
	else:
		FoeTable.add_user(id, username)


# if table does not contain the user with provided `id`, then no-op
func remove_user(id: int):
	AllyTable.remove_user(id)
	FoeTable.remove_user(id)

func add_kill(id: int):
	AllyTable.add_kill(id)
	FoeTable.add_kill(id)

func add_death(id: int):
	AllyTable.add_death(id)
	FoeTable.add_death(id)
