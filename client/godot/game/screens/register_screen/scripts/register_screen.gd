extends Control

signal scene_changed(scene_name)
signal play_click_sound()

const HTTPRequester = preload("res://network/http_request.gd")
var http_request: HTTPRequester = HTTPRequester.new()

onready var ErrorLabel = $CenterContainer/Form/ErrorLabel
onready var UsernameInput = $CenterContainer/Form/UsernameInput
onready var PasswordInput = $CenterContainer/Form/PasswordInput

var username: String = ""
var password: String = ""

func _ready():
	http_request.connect("receive", self, "_handle_request_complete")
	http_request.connect("error", self, "_handle_request_fail")
	add_child(http_request)
	hide_error()


func _on_BackButton_pressed():
	emit_signal("play_click_sound")
	emit_signal("scene_changed", "start_screen")

func _on_SubmitButton_pressed():
	emit_signal("play_click_sound")
	username = UsernameInput.text
	password = PasswordInput.text
	if (username.empty() || password.empty()):
		show_error("Please, enter all fields")
		return
		
	hide_error()
	var body = to_json({"username" : username, "password" : password})
	var headers = PoolStringArray([])
	http_request.method_post("/register", headers, body)
	

func _on_UsernameInput_text_changed(new_text):
	hide_error()

func _on_PasswordInput_text_changed(new_text):
	hide_error()


func _handle_request_complete(response_code: int, headers: PoolStringArray, body: JSONParseResult):
	if (response_code == 200):
		# print(body.result)
		Global.access_token = body.result.token
		Global.username = username
		emit_signal("scene_changed", "game_lobby")
	elif (response_code == 0):
		show_error("Invalid request.")
	elif (response_code == 500):
		show_error("Internal server error. Try again")
	else:
		show_error("Username is already taken")

func _handle_request_fail(message: String):
	show_error(message)


func hide_error():
	ErrorLabel.text = ""
	ErrorLabel.visible = false

func show_error(error_message: String):
	ErrorLabel.text = error_message
	ErrorLabel.visible = true




