extends Control


var remaining_time_sec: int = 0
onready var timer = $Timer
onready var minutes_label = $Clock/Minutes
onready var seconds_label = $Clock/Seconds


func show_text():
	var mins: int = remaining_time_sec / 60
	var secs: int = remaining_time_sec % 60
	
	minutes_label.text = get_text_for_time(mins)
	seconds_label.text = get_text_for_time(secs)

func get_text_for_time(count):
	var label: String = ""
	if (count < 10):
		label += "0"
	
	label += str(count)
	return label

func start(time_sec):
	remaining_time_sec = int(time_sec)
	show_text()
	timer.start()


func _on_Timer_timeout():
	remaining_time_sec -= 1
	show_text()
	if (remaining_time_sec > 0):
		timer.start()
