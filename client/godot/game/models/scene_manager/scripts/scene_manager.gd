extends Node2D

onready var current_scene = $UI/StartScreen
var current_scene_name = "start_screen"
onready var lobby_music = $UI/LobbyMusic

func _ready():
	current_scene.connect("scene_changed", self, "_handle_scene_changed")
	lobby_music.play()
	
func _handle_scene_changed(next_scene_name):
	match next_scene_name:
		'start_screen':
			var next_scene = load("res://screens/start_screen/start_screen.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene, next_scene_name)
		'login_screen':
			var next_scene = load("res://screens/login_screen/login_screen.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene, next_scene_name)
		'register_screen':
			var next_scene = load("res://screens/register_screen/register_screen.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene, next_scene_name)
		'game_lobby':
			var next_scene = load("res://screens/game_lobby/game_lobby.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene, next_scene_name)
		'game':
			var next_scene = load("res://screens/game/world.tscn").instance()
			add_child(next_scene)
			set_current_scene(next_scene, next_scene_name)
		_:
			print("No matching scene found for: '", next_scene_name, "'")


func set_current_scene(next_scene, next_scene_name):
	if (next_scene_name == "game"):
		lobby_music.stop()
	else:
		if (!lobby_music.is_playing()):
			lobby_music.play()
	
	current_scene.queue_free()
	current_scene = next_scene
	current_scene.connect("scene_changed", self, "_handle_scene_changed")
	
		
