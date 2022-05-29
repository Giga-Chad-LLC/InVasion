extends GridContainer


const Row = preload("res://ui/leaderboard/leaderboard_row.tscn")

var team_data: Dictionary = {
	# id: { username, kills, deaths }
}
var team_ids: Array = []

func custom_sort(a, b):
	return team_data[a].kills > team_data[b].kills

func clear_table():
	for child in self.get_children():
		self.remove_child(child)
		child.queue_free()

func render_table():
	team_ids.sort_custom(self, "custom_sort")
	clear_table()
	
	self.add_child(HSplitContainer.new())
	
	for i in range(0, team_ids.size()):
		var id = team_ids[i]
		var new_row = Row.instance()
		self.add_child(new_row)
		new_row.set_data(team_data[id])
	
	self.add_child(HSplitContainer.new())

func add_user(id: int, username: String):
	if (team_ids.find(id) == -1):
		team_data[id] = {
			"username": username,
			"kills": 0,
			"deaths": 0,
			"ratio": 0.0
		}
		
		team_ids.push_back(id)
		render_table()

func calc_ratio(id):
	if (!team_data.has(id)):
		return
	
	if (team_data[id].deaths == 0):
		team_data[id].ratio = 1.0
	else:
		team_data[id].ratio = team_data[id].kills / team_data[id].deaths

func add_kill(id: int):
	if (team_data.has(id)):
		team_data[id].kills += 1
		calc_ratio(id)
		print("Add kill")
		render_table()

func add_death(id: int):
	if (team_data.has(id)):
		team_data[id].deaths += 1
		print("Add death")
		calc_ratio(id)
		render_table()

func remove_user(id: int):
	if (team_data.has(id) and team_ids.find(id) != -1):
		team_data.erase(id)
		team_ids.remove(id)
		print("Remove")
		render_table()
