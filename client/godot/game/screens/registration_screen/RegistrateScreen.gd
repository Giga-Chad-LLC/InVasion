extends Control


signal scene_changed(scene_name)

onready var label_error = $"./LabelOfError"
onready var http_request = $CheckButton/HTTPRequest

func _ready():
	#http_request.connect("request_completed", self, "_on_HTTPRequest_request_completed")
	label_error.set("custom_colors/font_color", Color(1,0,0))
	label_error.visible = false	

func _on_GotoBack_pressed():
	emit_signal("scene_changed", "start_menu")


func _on_CheckButton_pressed():
	label_error.visible = false
	var password = $"./CheckButton/PasswordEdit".get_text()
	var username = $"./CheckButton/UsernameEdit".get_text()
	var body = {"nickname" : username, "password" : password}
	http_request.request("http://0.0.0.0:5555/registration", PoolStringArray([]),
		false, HTTPClient.METHOD_POST, to_json(body))

func _on_HTTPRequest_request_completed(result, response_code, headers, body):
	if (response_code == 200):
		emit_signal("scene_changed", "game_menu")
	elif (response_code == 0):
		label_error.text = "ERROR FROM SERVER"
	elif(response_code == 404):
		label_error.text = "Empty login or password"
	elif (response_code == 500):
		label_error.text = "Invalid Symbols!"
	else:
		label_error.text = "This Login already taken!"
	label_error.visible = true
