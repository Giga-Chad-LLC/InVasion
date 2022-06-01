extends GridContainer


export(Color) var highlight_color = Color(1, 1, 1, 0)
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
	Ratio.text = str(data['ratio'])

func change_color(new_color: Color):
	# use Color(0, 0, 0, 0.185) to hightlight the main player
	highlight_color = new_color
	colorize()

func colorize():
	Hightlighter.color = highlight_color
