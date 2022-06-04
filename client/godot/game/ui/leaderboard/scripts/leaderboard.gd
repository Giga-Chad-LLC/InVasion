extends Control

onready var AllyTable = $Container/AllyTeam
onready var FoeTable = $Container/FoeTeam
onready var Title = $Title
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

func set_text(text):
	Title.text = text

func set_main_player_id(id: int):
	AllyTable.main_player_id = id

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

func add_kills(id: int, count: int):
	AllyTable.add_kills(id, count)
	FoeTable.add_kills(id, count)

func add_deaths(id: int, count: int):
	AllyTable.add_deaths(id, count)
	FoeTable.add_deaths(id, count)
