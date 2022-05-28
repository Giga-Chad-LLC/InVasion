extends Label

var counterKills = 0

func _ready():
	text = "kills:    " + String(counterKills)

func increment():
	text = "kills:    " + String(counterKills)
