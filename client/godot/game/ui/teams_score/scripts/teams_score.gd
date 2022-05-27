extends Node

var team1_kills = 0
var team2_kills = 0

onready var team1_score_label = $Score1/Score
onready var team2_score_label = $Score2/Score

func update_teams_score(killed_players, our_team_id):
	for i in range(0, killed_players.size()):
		if (killed_players[i].team_id == our_team_id):
			team2_kills += 1
		else:
			team1_kills += 1
	show_score()

func set_teams_score(team1_score, team2_score):
	team1_score = team1_score
	team2_score = team2_score
	show_score()

func show_score():
	team1_score_label.text = str(team1_kills)
	team2_score_label.text = str(team2_kills)
