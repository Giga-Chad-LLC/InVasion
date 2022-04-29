extends Control


signal scene_changed(scene_name)
onready var start_game_button = $CenterContainer/VBoxContainer/Button

func _on_Button_pressed():
	var password = $"./PasswordEdit".get_text()
	var username = $"./UsernameEdit".get_text()
	if (password == "0000" and username == "aboba"):
		print("suka")
		emit_signal("scene_changed", "lobby")
