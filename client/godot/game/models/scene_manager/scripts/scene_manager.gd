extends Node2D

onready var current_scene = $UI/StartScreen

func _ready():
	current_scene.connect("scene_changed", self, "_handle_scene_changed")

func _handle_scene_changed(next_scene_name):
	match next_scene_name:
		'start_screen':
			var next_scene = load("res://screens/start_screen/start_screen.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene)
		'login_screen':
			var next_scene = load("res://screens/login_screen/login_screen.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene)
		'register_screen':
			var next_scene = load("res://screens/register_screen/register_screen.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene)
		'game_lobby':
			var next_scene = load("res://screens/game_lobby/game_lobby.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene)
		'game':
			var next_scene = load("res://screens/game/world.tscn").instance()
			add_child(next_scene)
			set_current_scene(next_scene)
		_:
			print("No matching scene found for: '", next_scene_name, "'")


func set_current_scene(next_scene):
	current_scene.queue_free()
	current_scene = next_scene
	current_scene.connect("scene_changed", self, "_handle_scene_changed")
