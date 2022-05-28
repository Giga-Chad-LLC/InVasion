extends Control

signal toggle_tab_statistic(is_active)

onready var my_team = $AlliedTeam
onready var enemy_team = $EnemyTeam
var row = preload("res://ui/tab-statistic/row/row.tscn")

onready var number_kills_by_nickname = {}
onready var number_deaths_by_nickname = {}

var count_enemies = 0
var count_teammates = 0
var teammates_nicknames = []
var enemy_nicknames = []

var is_active = false setget set_is_active

func _ready():
	visible = false


func increment_count_kills(nickname):
	number_kills_by_nickname[nickname] += 1

func increment_count_deaths(nickname):
	number_deaths_by_nickname[nickname] += 1

func add_row(nickname, name_team):
	var new_row = row.instance()
	if (name_team == "my_team"):
		teammates_nicknames.append(nickname)
		count_teammates += 1
		my_team.add_child(new_row)
		
	else:
		enemy_nicknames.append(nickname)
		count_enemies += 1
		enemy_team.add_child(new_row)
		
	
	number_deaths_by_nickname[nickname] = 0
	number_kills_by_nickname[nickname] = 0
	new_row.set_default_values(nickname)


func get_ratio(nickname):
	if (number_deaths_by_nickname[nickname] == 0):
		return 0
	else:
		return number_kills_by_nickname[nickname] / number_deaths_by_nickname[nickname]

func update_row(new_row, nickname):
	new_row.set_nickname(nickname)
	new_row.set_count_deaths(number_deaths_by_nickname[nickname])
	new_row.set_count_kills(number_kills_by_nickname[nickname])
	new_row.set_ratio(get_ratio(nickname))

func update_table(name_of_team):
	var current_node
	var list_of_nicknames
	if (name_of_team == "my_team"):
		current_node = my_team
		list_of_nicknames = teammates_nicknames
	else:
		current_node = enemy_team
		list_of_nicknames = enemy_nicknames
	for child in current_node.get_children():
		current_node.remove_child(child)
	list_of_nicknames.sort_custom(self, "custom_comparator")	
	for nickname in list_of_nicknames:
		var new_row = row.instance()
		current_node.add_child(new_row)
		update_row(new_row, nickname)


func custom_comparator(a, b):
	var ratio_a = get_ratio(a)
	var ratio_b = get_ratio(b)
	return ratio_a >= ratio_b

# `tab` button pressed
func _unhandled_input(event):
	update_table("my_team")
	update_table("enemy_team")
	if event.is_action_pressed('tab'):
		toggle(!is_active)
	if event.is_action_pressed('my_team'):
		add_row("my", "my_team")
	if event.is_action_pressed("enemy"):
		add_row("b", "my_team")
		increment_count_deaths("b")
		increment_count_kills("b")


func toggle(value):
	self.is_active = value
	emit_signal("toggle_tab_statistic", self.is_active)


# setters
func set_is_active(value):
	is_active = value
	visible = value

