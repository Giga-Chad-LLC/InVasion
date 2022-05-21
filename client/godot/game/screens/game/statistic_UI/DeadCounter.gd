extends Label

var counterDeads = 0

func _ready():
	text = "deads: " + String(counterDeads)

func increment():
	text = "deads: " + String(counterDeads)
