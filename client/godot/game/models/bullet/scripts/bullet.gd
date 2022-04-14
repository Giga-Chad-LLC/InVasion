extends Node2D


# Variables
export(int) var speed = 900
export(int) var damage = 1
var velocity = Vector2(1, 0)

# User-defined functions
func _on_Hitbox_area_entered(_area):
	queue_free()

func is_in_game_world_bounds(position) -> bool:
	return (
		position.x <= GameWorld.right_bound and
		position.x >= GameWorld.left_bound and
		position.y <= GameWorld.down_bound and
		position.y >= GameWorld.up_bound
	)

func _on_Hitbox_body_entered(_body):
	queue_free()


# Built-in functions
func _ready():
	visible = false
	yield(get_tree(), "idle_frame")
	
	velocity = velocity.rotated(rotation)
	visible = true
	
func _physics_process(delta):
	global_position += velocity * speed * delta
	
	if (not is_in_game_world_bounds(global_position)):
		queue_free()
	

