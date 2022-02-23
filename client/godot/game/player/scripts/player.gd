extends KinematicBody2D

# Variables
export var ACCELERATION = 500
export var MAX_SPEED = 100
export var FRICTION = 500

onready var animationPlayer = $AnimationPlayer
onready var animationTree = $AnimationTree
onready var animationState = animationTree.get("parameters/playback")

enum { MOVE }
var state = MOVE
var velocity = Vector2.ZERO
var player_id = 1

enum {
	Idle,
	StartMovingUp,
	StartMovingRight,
	StartMovingDown,
	StartMovingLeft,
	
	StopMovingUp,
	StopMovingRight,
	StopMovingDown,
	StopMovingLeft
}

class MoveRequestModel:
	func send_request():
		if (Input.is_action_just_pressed("ui_up")):
			return StartMovingUp
		if (Input.is_action_just_released("ui_up")):
			return StopMovingUp
		
		if (Input.is_action_just_pressed("ui_right")):
			return StartMovingRight
		if (Input.is_action_just_released("ui_right")):
			return StopMovingRight
		
		if (Input.is_action_just_pressed("ui_down")):
			return StartMovingDown
		if (Input.is_action_just_released("ui_down")):
			return StopMovingDown
		
		if (Input.is_action_just_pressed("ui_left")):
			return StartMovingLeft
		if (Input.is_action_just_released("ui_left")):
			return StopMovingLeft
		return Idle

class MoveReactModel:
	var input_vector = Vector2.ZERO
	
	func get_request(move_event):
		if (move_event == StartMovingUp):
			input_vector.y = -1
		if (move_event == StopMovingUp):
			input_vector.y = 0
		
		if (move_event == StartMovingRight):
			input_vector.x = 1
		if (move_event == StopMovingRight):
			input_vector.x = 0
		
		if (move_event == StartMovingDown):
			input_vector.y = 1
		if (move_event == StopMovingDown):
			input_vector.y = 0
		
		if (move_event == StartMovingLeft):
			input_vector.x = -1
		if (move_event == StopMovingLeft):
			input_vector.x = 0
		
		return input_vector


onready var move_request_model = MoveRequestModel.new()
onready var move_react_model = MoveReactModel.new()



# Built-in functions
func _ready():
	animationTree.active = true
	

func _physics_process(delta):
#	match state:
#		MOVE:
#			var input_vector = get_move_input_vector()
#			player_move(delta, input_vector)
	var action = move_request_model.send_request()
#	yield(get_tree().create_timer(0.2), "timeout")
	var input_vector = move_react_model.get_request(action)
	player_move(delta, input_vector)
	

func get_move_input_vector():
	var input_vector = Vector2.ZERO
	input_vector.x = Input.get_action_strength("ui_right") - Input.get_action_strength("ui_left")
	input_vector.y = Input.get_action_strength("ui_down") - Input.get_action_strength("ui_up")
	input_vector = input_vector.normalized()
	return input_vector

# User-defined functions
func player_move(delta, input_vector):
	if input_vector != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", input_vector)
		animationTree.set("parameters/Run/blend_position", input_vector)

		animationState.travel("Run")
		
		velocity = velocity.move_toward(input_vector * MAX_SPEED, ACCELERATION * delta)
	else:
		animationState.travel("Idle")
		velocity = velocity.move_toward(Vector2.ZERO, FRICTION * delta)

	velocity = move_and_slide(velocity)

