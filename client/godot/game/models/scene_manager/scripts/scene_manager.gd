extends Node2D

onready var current_scene = $UI/Control

func _ready():
	current_scene.connect("scene_changed", self, "_handle_scene_changed")

func _handle_scene_changed(next_scene_name):
	match next_scene_name:
		'game':
			var next_scene = load("res://screens/game/world.tscn").instance()
			add_child(next_scene)
			set_current_scene(next_scene)
		'start_menu':
			var next_scene = load("res://screens/new_start_screen/new_start_screen.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene)
		'login':
			var next_scene = load("res://screens/login_screen/LoginScreen.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene)
		'registrate':
			var next_scene = load("res://screens/registration_screen/RegistrateScreen.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene)
		'game_menu':
			var next_scene = load("res://screens/game_menu_screen/GameMenuScreen.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene)
		_:
			print("No matching scene found for: '", next_scene_name, "'")


func set_current_scene(next_scene):
	current_scene.queue_free()
	current_scene = next_scene
	current_scene.connect("scene_changed", self, "_handle_scene_changed")
