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


# User-defined functions
func player_move(delta):
	var input_vector = get_move_input_vector()
	
	if input_vector != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", input_vector)
		animationTree.set("parameters/Run/blend_position", input_vector)

		animationState.travel("Run")
		
		velocity = velocity.move_toward(input_vector * MAX_SPEED, ACCELERATION * delta)
	else:
		animationState.travel("Idle")
		velocity = velocity.move_toward(Vector2.ZERO, FRICTION * delta)
		
	velocity = move_and_slide(velocity)

func get_move_input_vector():
	var input_vector = Vector2.ZERO
	input_vector.x = Input.get_action_strength("ui_right") - Input.get_action_strength("ui_left")
	input_vector.y = Input.get_action_strength("ui_down") - Input.get_action_strength("ui_up")
	input_vector = input_vector.normalized()
	return input_vector


#var tcp_connection = StreamPeerTCP.new()

# Built-in functions
func _ready():
	animationTree.active = true
	
#	# Try connecting to the C++ ping-pong server
#	var tcp_server_host = "localhost"
#	var tcp_server_port = 12345
#	tcp_connection.connect_to_host(tcp_server_host, tcp_server_port)
#
#	if (tcp_connection.is_connected_to_host()):
#		print("Connected to host: ", tcp_server_host, ":", tcp_server_port)
#		var string = "Hello for my nigger boiii\n"
#		tcp_connection.put_data(string.to_utf8())
#
#	else:
#		print("Failed to connect to: ", tcp_server_host, ":", tcp_server_port)
#
func _physics_process(delta):
	match state:
		MOVE:
			player_move(delta)














