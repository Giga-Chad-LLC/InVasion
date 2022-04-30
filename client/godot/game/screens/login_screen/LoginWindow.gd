extends Control


signal scene_changed(scene_name)
onready var start_game_button = $CenterContainer/VBoxContainer/Button
onready var info_label = $"./LabelOfError"

func _ready():
	info_label.set("custom_colors/font_color", Color(1,0,0))
	info_label.visible = false
	

func _on_Button_pressed():
	var password = $"./CheckButton/PasswordEdit".get_text()
	var username = $"./CheckButton/UsernameEdit".get_text()
	# тут инициализация запроса к БД
	if (password == "0000" and username == "aboba"):
		# тут логика при успешном подключении
		emit_signal("scene_changed", "lobby")
	else:
		info_label.visible = true


func _on_GotoBack_pressed():
	emit_signal("scene_changed", "start_menu")
