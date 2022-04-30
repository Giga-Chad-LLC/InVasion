#extends KinematicBody2D
extends "res://player/scripts/base_player.gd"

# Animations
onready var player_gun = $Gun

# Godobuf
const MoveRequestModel = preload("res://proto/request-models/move_request_model.gd")
const ShootRequestModel = preload("res://proto/request-models/shoot_request_model.gd")

const PlayerPositionResponseModel = preload("res://proto/response-models/player_position_response_model.gd")
const PlayerInfoResponseModel = preload("res://proto/response-models/player_info_response_model.gd")
const GameStateResponseModel = preload("res://proto/response-models/game_state_response_model.gd")

# Parameters
var previous_action = MoveRequestModel.MoveRequestModel.MoveEvent.Idle
var player_id: int = -1
var team_id: int = -1

# Network
const NetworkPacket = preload("res://network/data_types.gd")


# Built-in functions
func _ready():
	# Activate animation for the character
	animationTree.active = true
	$Gun.should_follow_mouse = true # gun will start following mouse


func get_player_move_request():
	var action: MoveRequestModel.MoveRequestModel = get_packed_move_action()
	if (action.get_current_event() != MoveRequestModel.MoveRequestModel.MoveEvent.Idle
		and previous_action != action.get_current_event()):
		previous_action = action.get_current_event()
		var network_packet = NetworkPacket.new()
		network_packet.set_data(action.to_bytes(), Global.RequestModels.MoveRequestModel)
		
		if (network_packet):
			return network_packet # producer.push_data(network_packet)
		return null

func get_player_shoot_request():
	if (Input.is_action_pressed("shoot") and not player_gun.is_cooldown):
		player_gun.start_cooldown()
		var action = ShootRequestModel.ShootRequestModel.new()
		action.set_player_id(player_id)
		action.new_weapon_direction()
		action.get_weapon_direction().set_x(cos(player_gun.global_rotation))
		action.get_weapon_direction().set_y(sin(player_gun.global_rotation))
		var network_packet = NetworkPacket.new()
		network_packet.set_data(action.to_bytes(), Global.RequestModels.ShootRequestModel)
		if (network_packet):
			return network_packet # producer.push_data(network_packet)
		return null

# save pressed key to the model object
func get_packed_move_action() -> MoveRequestModel.MoveRequestModel:
	var packed_player_action = MoveRequestModel.MoveRequestModel.new()
	packed_player_action.set_player_id(player_id)
	packed_player_action.set_session_id(0) # dummy session ID, will be removed in the future probably 

	if (Input.is_action_just_pressed("ui_up")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StartMovingUp)
	elif (Input.is_action_just_released("ui_up")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StopMovingUp)
	elif (Input.is_action_just_pressed("ui_right")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StartMovingRight)
	elif (Input.is_action_just_released("ui_right")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StopMovingRight)
	elif (Input.is_action_just_pressed("ui_down")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StartMovingDown)
	elif (Input.is_action_just_released("ui_down")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StopMovingUp)

	elif (Input.is_action_just_pressed("ui_left")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StartMovingLeft)
	elif (Input.is_action_just_released("ui_left")):
		packed_player_action.set_current_event(packed_player_action.MoveEvent.StopMovingLeft)
	else:
		packed_player_action.set_current_event(packed_player_action.MoveEvent.Idle)
	
	return packed_player_action

# Set player id retrieved from server
func set_player_info(packet: NetworkPacket) -> void:
	var player_info_model = PlayerInfoResponseModel.PlayerInfoResponseModel.new()
	var result_code = player_info_model.from_bytes(packet.get_bytes())
	if (result_code != PlayerInfoResponseModel.PB_ERR.NO_ERRORS):
		print("Error while receiving: ", "cannot unpack player id")
	else:
		player_id = player_info_model.get_player_id()
		team_id = player_info_model.get_team_id()
		print("Set my id: ", player_id)
		print("Set my team id: ", team_id)
