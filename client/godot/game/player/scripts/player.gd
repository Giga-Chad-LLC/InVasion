#extends KinematicBody2D
extends "res://player/scripts/base_player.gd"

# Animations
onready var player_gun = $Gun

# Godobuf
const MoveRequestModel = preload("res://proto/request-models/move_request_model.gd")
const ShootRequestModel = preload("res://proto/request-models/shoot_request_model.gd")
const RespawnPlayerRequestModel = preload("res://proto/request-models/respawn_player_request_model.gd")
const ChangePlayerSpecializationRequestModel = preload("res://proto/request-models/change_player_specialization_request_model.gd")
const ApplyAbilityRequestModel = preload("res://proto/request-models/apply_ability_request_model.gd")

const PlayerPositionResponseModel = preload("res://proto/response-models/player_position_response_model.gd")
const GameStateResponseModel = preload("res://proto/response-models/game_state_response_model.gd")
const HandshakeResponseModel = preload("res://proto/response-models/handshake_response_model.proto.gd")

# Parameters
var previous_action = MoveRequestModel.MoveRequestModel.MoveEvent.Idle
var previous_specialization = -1
var is_active: bool setget set_is_active
var is_dead: bool setget set_is_dead

# abilities
var is_ability_cooldown: bool = false
onready var ability_cooldown_timer = $AbilityCooldownTimer

# Network
const NetworkPacket = preload("res://network/data_types.gd")


# Built-in functions
func _ready():
	self.is_active = false # call the setter (gun will start rotating as well)
	is_dead = false # don't call the setter
	visible = false

func set_is_dead(value):
	is_dead = value
	visible = !value

func set_is_active(value):
	is_active = value
	$Gun.should_follow_mouse = is_active

func get_respawn_player_request():
	var action: RespawnPlayerRequestModel.RespawnPlayerRequestModel = RespawnPlayerRequestModel.RespawnPlayerRequestModel.new()
	action.set_player_id(player_id)
	var network_packet = NetworkPacket.new()
	network_packet.set_data(action.to_bytes(), Global.RequestModels.RespawnPlayerRequestModel)
	if (network_packet):
		return network_packet 
	return null

func get_player_specialization_request(new_specialization):
	if (player_specialization != new_specialization):
		var spec_model = ChangePlayerSpecializationRequestModel.ChangePlayerSpecializationRequestModel.new()
		spec_model.set_specialization(new_specialization)
		spec_model.set_player_id(player_id)

		var network_packet = NetworkPacket.new()
		network_packet.set_data(spec_model.to_bytes(), Global.RequestModels.ChangePlayerSpecializationRequestModel)
		if (network_packet):
			return network_packet
	return null



func get_player_move_request():
	var action: MoveRequestModel.MoveRequestModel = get_packed_move_action()
	if (action.get_current_event() != MoveRequestModel.MoveRequestModel.MoveEvent.Idle
		and previous_action != action.get_current_event()):
		previous_action = action.get_current_event()
		var network_packet = NetworkPacket.new()
		network_packet.set_data(action.to_bytes(), Global.RequestModels.MoveRequestModel)
		
		if (network_packet):
			return network_packet
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
			return network_packet 
		return null


# Ability managment
func start_ability_cooldown():
	is_ability_cooldown = true
	ability_cooldown_timer.start()

func _on_AbilityCooldownTimer_timeout():
	is_ability_cooldown = false

func get_apply_ability_request():
	if (Input.is_action_pressed("apply_ability") and not is_ability_cooldown):
		print("Use ability!")
		var ability = ApplyAbilityRequestModel.ApplyAbilityRequestModel.new()
		ability.set_player_id(player_id)
		var network_packet = NetworkPacket.new()
		network_packet.set_data(ability.to_bytes(), Global.RequestModels.ApplyAbilityRequestModel)
		start_ability_cooldown()
		if (network_packet):
			return network_packet
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
	var handshake_model = HandshakeResponseModel.HandshakeResponseModel.new()
	var result_code = handshake_model.from_bytes(packet.get_bytes())
	if (result_code != HandshakeResponseModel.PB_ERR.NO_ERRORS):
		print("Error while receiving: ", "cannot unpack handshake")
	else:
		player_id = handshake_model.get_player_id()
		team_id = handshake_model.get_team_id()
		print("Set my id: ", player_id)
		print("Set my team id: ", team_id)


