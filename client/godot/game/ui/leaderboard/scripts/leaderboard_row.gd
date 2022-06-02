extends GridContainer


export(Color) var highlight_color = Color(1, 1, 1, 0)
const max_ratio_length_after_period: int = 2
onready var Hightlighter = $Background/Highlighter

onready var Username = $Username
onready var Kills = $Kills
onready var Deaths = $Deaths
onready var Ratio = $Ratio


func _ready():
	colorize()

func set_data(data):
	Username.text = data['username']
	Kills.text = str(data['kills'])
	Deaths.text = str(data['deaths'])
	Ratio.text = get_short_ratio(str(data['ratio']))

func get_short_ratio(current_ratio: String):
	var ratio = ""
	var found_period: bool = false
	var length_after_period = 0
	for i in range(0, current_ratio.length()):
		if (!found_period):
			ratio += current_ratio[i]
		else:
			if (length_after_period + 1 <= max_ratio_length_after_period):
				ratio += current_ratio[i]
				length_after_period += 1
			else:
				break
		
		if (current_ratio[i] == '.'):
			found_period = true
	
	return ratio

func change_color(new_color: Color):
	# use Color(1, 1, 1, 0.185) to hightlight the main player
	highlight_color = new_color
	colorize()

func colorize():
	Hightlighter.color = highlight_color
