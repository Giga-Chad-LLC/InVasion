extends KinematicBody2D

export var ACCELERATION = 500
export var MAX_SPEED = 100
export var FRICTION = 500
var velocity = Vector2.ZERO

enum {
	MOVE, 
	ROLL,
	ATTACK
}

var state = MOVE
onready var animationPlayer = $AnimationPlayer
onready var animationTree = $AnimationTree
onready var animationState = animationTree.get("parameters/playback")

func _ready():
	animationTree.active = true


func _physics_process(delta):
	match state:
		MOVE:
			player_move(delta) # move the player
		ATTACK:
			player_attack(delta)	


func player_attack(_delta):
	animationState.travel("Attack")
	
func attack_animation_finished():
	velocity = Vector2.ZERO
	state = MOVE

func player_move(delta):
	# do the maths for the character moving
	var input_vector = Vector2.ZERO
	input_vector.x = Input.get_action_strength("ui_right") - Input.get_action_strength("ui_left")
	input_vector.y = Input.get_action_strength("ui_down") - Input.get_action_strength("ui_up")
	input_vector = input_vector.normalized()
	
	if input_vector != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", input_vector)
		animationTree.set("parameters/Run/blend_position", input_vector)
		# set attack direction
		animationTree.set("parameters/Attack/blend_position", input_vector)
		
		animationState.travel("Run")
		
		velocity = velocity.move_toward(input_vector * MAX_SPEED, ACCELERATION * delta)
	else:
		animationState.travel("Idle")
		velocity = velocity.move_toward(Vector2.ZERO, FRICTION * delta)
	
	velocity = move_and_slide(velocity)
	
	# check for melee attack
	if (Input.is_action_just_pressed("melee_attack")):
		state = ATTACK
	














