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
			var next_scene = load("res://screens/start_screen/start_screen.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene)
		'login':
			var next_scene = load("res://screens/login_screen/LoginWindow.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene)
		'lobby':
			var next_scene = load("res://screens/lobby_screen/LobbyScreen.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene)
		_:
			print("No matching scene found for: '", next_scene_name, "'")


func set_current_scene(next_scene):
	current_scene.queue_free()
	current_scene = next_scene
	current_scene.connect("scene_changed", self, "_handle_scene_changed")
