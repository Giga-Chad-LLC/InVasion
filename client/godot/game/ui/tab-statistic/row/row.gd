extends VBoxContainer

onready var count_kills_label = $Stats/countKills
onready var count_deaths_label = $Stats/countDeaths
onready var ratio_label = $Stats/ratio
onready var nickname_label = $Stats/nickname


func set_default_values(nickname):
	nickname_label.text = nickname
	count_kills_label.text = String(0)
	count_deaths_label.text = String(0)
	ratio_label.text = String(0)

func set_nickname(nickname):
	nickname_label.text = nickname

func set_count_kills(value):
	count_kills_label.text = String(value)

func set_count_deaths(value):
	count_deaths_label.text = String(value)
	
func set_ratio(value):
	ratio_label.text = String(value)
