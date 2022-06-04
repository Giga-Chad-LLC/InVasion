extends KinematicBody2D


# Variables
var velocity = Vector2.ZERO
var bullet_id = -1
var player_id = -1 # id of owner of the bullet
var team_id = -1 # team id of the owner


func update_bullet_position(bullet_position_model):
	velocity = Vector2(bullet_position_model.get_velocity().get_x(), bullet_position_model.get_velocity().get_y())
	global_position = Vector2(bullet_position_model.get_position().get_x(), bullet_position_model.get_position().get_y())


func _process(_delta):
	move_and_slide(velocity) # interpolation
	rotation = velocity.angle()


