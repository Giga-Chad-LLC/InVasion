extends KinematicBody2D



	
const FRICTION = 500
const MAX_SPEED = 70
const ACCELERATION = 500

onready var animationPlayer = $AnimationPlayer
onready var animationTree = $AnimationTree
onready var animationState = animationTree.get("parameters/playback")

var velocity = Vector2.ZERO


func _physics_process(delta):
	var velocity_vector = Vector2.ZERO

	velocity_vector.x = Input.get_action_strength("ui_right") - Input.get_action_strength("ui_left")
	velocity_vector.y = Input.get_action_strength("ui_down") - Input.get_action_strength("ui_up")
	velocity_vector = velocity_vector.normalized()
	
	if velocity_vector != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", velocity_vector)
		animationTree.set("parameters/Run/blend_position", velocity_vector)
		animationState.travel("Run")
		velocity = velocity.move_toward(velocity_vector * MAX_SPEED, ACCELERATION * delta)
		
	else:
		
		velocity = velocity.move_toward(Vector2.ZERO, FRICTION * delta)
		animationState.travel("Idle")
	velocity = move_and_slide(velocity)
