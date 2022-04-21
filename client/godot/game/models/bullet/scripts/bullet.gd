extends Node2D


# Variables
var velocity = Vector2.ZERO
var bullet_id = -1
var player_id = -1 # id of owner of the bullet

# Built-in functions
#func _ready():
#	visible = false
#	yield(get_tree(), "idle_frame")
#	visible = true

func update_bullet_position(bullet_position_model):
	velocity = Vector2(bullet_position_model.get_velocity().get_x(), bullet_position_model.get_velocity().get_y())
	global_position = Vector2(bullet_position_model.get_position().get_x(), bullet_position_model.get_position().get_y())


func _process(_delta):
	global_position += velocity / 100


