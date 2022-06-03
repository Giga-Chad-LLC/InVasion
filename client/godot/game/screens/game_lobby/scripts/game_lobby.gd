extends Control

signal scene_changed(scene_name)
signal play_click_sound()


onready var KillsValue = $CenterContainer/Wrapper/KillsValue
onready var DeathsValue = $CenterContainer/Wrapper/DeathsValue
onready var RatioValue = $CenterContainer/Wrapper/RatioValue
onready var VictoriesValue = $CenterContainer/Wrapper/VictoriesValue
onready var MatchesValue = $CenterContainer/Wrapper/MatchesValue
onready var ErrorLabel = $CenterContainer/Wrapper/ErrorLabel

const HTTPRequester = preload("res://network/http_request.gd")
var http_request: HTTPRequester = HTTPRequester.new()
var max_ratio_length_after_period: int = 2

func _on_ExitButton_pressed():
	emit_signal("play_click_sound")
	emit_signal("scene_changed", "start_screen")


func _on_StartGameButton_pressed():
	emit_signal("play_click_sound")
	emit_signal("scene_changed", "game")

func _ready():
	http_request.connect("receive", self, "_handle_request_complete")
	http_request.connect("error", self, "_handle_request_fail")
	add_child(http_request)
	hide_error()
	retrieve_stats()


func retrieve_stats():
	var body = to_json({"username" : Global.username, "token" : Global.access_token})
	var headers = PoolStringArray([])
	http_request.method_get('/statistic', headers, body)

func _handle_request_complete(response_code: int, headers: PoolStringArray, body: JSONParseResult):
	print(body.result)
	if (response_code == 200):
		set_data(body.result)
	elif (response_code == 0):
		show_error("Invalid request")
	elif (response_code == 500):
		show_error("Internal server error. Unable to load stats")
	else:
		show_error(body.result.message)

func _handle_request_fail(message: String):
	show_error(message)


func set_data(data):
	KillsValue.text = str(data.kills)
	DeathsValue.text = str(data.deaths)
	var ratio: float = 1.0
	if (data.deaths != 0):
		ratio = float(data.kills) / float(data.deaths)
	RatioValue.text = get_short_ratio(str(ratio))
	VictoriesValue.text = str(data.victories)
	MatchesValue.text = str(data.matches)

func hide_error():
	ErrorLabel.text = ""
	ErrorLabel.visible = false

func show_error(error_message: String):
	ErrorLabel.text = error_message
	ErrorLabel.visible = true



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
