extends Node2D

onready var current_scene = $UI/StartScreen
onready var menu_sound = $UI/LobbySound
onready var game_sound = $UI/BattleSound
var cur_sound

func _process(delta):
	# if sound ended, play it
	if (!cur_sound.playing):
		cur_sound.play()

func _ready():
	current_scene.connect("scene_changed", self, "_handle_scene_changed")
	cur_sound = menu_sound
	cur_sound.play()
	
	
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
			_change_sound("menu")
			var next_scene = load("res://screens/game_lobby/game_lobby.tscn").instance()
			$UI.add_child(next_scene)
			set_current_scene(next_scene)
		'game':
			_change_sound("game")
			var next_scene = load("res://screens/game/world.tscn").instance()
			add_child(next_scene)
			set_current_scene(next_scene)
		_:
			print("No matching scene found for: '", next_scene_name, "'")


func _change_sound(scene_name):
	if (scene_name == "menu" and cur_sound == menu_sound):
		return
	cur_sound.stop()
	if (scene_name == "game"):
		cur_sound = game_sound
	else:
		cur_sound = menu_sound
	cur_sound.play()

func set_current_scene(next_scene):	
	current_scene.queue_free()
	current_scene = next_scene
	current_scene.connect("scene_changed", self, "_handle_scene_changed")
	
		
