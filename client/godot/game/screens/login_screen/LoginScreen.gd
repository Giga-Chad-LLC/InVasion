extends Control


signal scene_changed(scene_name)
onready var start_game_button = $CenterContainer/VBoxContainer/Button
onready var label_error = $"./LabelOfError"
onready var http_request = $"./CheckButton/HTTPRequest"

func _ready():
	label_error.set("custom_colors/font_color", Color(1,0,0))
	label_error.visible = false
	

func _on_Button_pressed():
	var password = $"./CheckButton/PasswordEdit".get_text()
	var username = $"./CheckButton/UsernameEdit".get_text()
	var body = {"nickname" : username, "password" : password}
	http_request.request("http://0.0.0.0:5555/login", PoolStringArray([]),
		false, HTTPClient.METHOD_GET, to_json(body))

func _on_HTTPRequest_request_completed(result, response_code, headers, body):
	if (response_code == 200):
		emit_signal("scene_changed", "game_menu")
	elif (response_code == 0):
		label_error.text = "ERROR FROM SERVER"
	elif(response_code == 404):
		label_error.text = "Bad request: empty fields"
	else:
		label_error.text = "Wrong login or password"
	label_error.visible = true


func _on_GotoBack_pressed():
	emit_signal("scene_changed", "start_menu")
