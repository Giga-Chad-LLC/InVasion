extends Control

var ENDPOINT = "http://127.0.0.1:5555"

signal scene_changed(scene_name)

onready var label_error = $"./LabelOfError"
onready var http_request = $CheckButton/HTTPRequest

func _ready():
	#http_request.connect("request_completed", self, "_http_request_completed")
	label_error.set("custom_colors/font_color", Color(1,0,0))
	label_error.visible = false

func _on_GotoBack_pressed():
	emit_signal("scene_changed", "start_menu")


# VERY BAD !!!
var password
var username
func _on_CheckButton_pressed():
	label_error.visible = false
	password = $"./PasswordEdit".get_text()
	username = $"./UsernameEdit".get_text()
	var body = {"nickname" : username, "password" : password}
	http_request.request(ENDPOINT + "/login", PoolStringArray([]),
		false, HTTPClient.METHOD_GET, to_json(body))

func _on_HTTPRequest_request_completed(result, response_code, headers, body):
	if (response_code == 200):
		var json = JSON.parse(body.get_string_from_utf8())
		print(json.result)
		print("Login response: ", json.result)
		Global.access_token = json.result.token
		Global.username = username
		
		emit_signal("scene_changed", "game_lobby")
	elif (response_code == 0):
		label_error.text = "ERROR FROM SERVER"
	elif(response_code == 404):
		label_error.text = "Empty login or password"
	elif (response_code == 500):
		label_error.text = "Invalid Symbols!"
	else:
		label_error.text = "Wrong login or password!"
	label_error.visible = true
