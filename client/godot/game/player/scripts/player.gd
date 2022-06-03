#extends KinematicBody2D
extends "res://player/scripts/base_player.gd"

# Godobuf
const MoveRequestModel = preload("res://proto/request-models/move_request_model.gd")
const ShootRequestModel = preload("res://proto/request-models/shoot_request_model.gd")
const RespawnPlayerRequestModel = preload("res://proto/request-models/respawn_player_request_model.gd")
const ChangePlayerSpecializationRequestModel = preload("res://proto/request-models/change_player_specialization_request_model.gd")
const ApplyAbilityRequestModel = preload("res://proto/request-models/apply_ability_request_model.gd")
const UseSupplyRequestModel = preload("res://proto/request-models/use_supply_request_model.gd")
const WeaponDirectionRequestModel = preload("res://proto/request-models/weapon_direction_request_model.gd")
const ReloadWeaponRequestModel = preload("res://proto/request-models/reload_weapon_request_model.gd")
const ClientCredentialsRequestModel = preload("res://proto/request-models/client_credentials_request_model.gd")

const PlayerPositionResponseModel = preload("res://proto/response-models/player_position_response_model.gd")
const GameStateResponseModel = preload("res://proto/response-models/game_state_response_model.gd")

# Network
const NetworkPacket = preload("res://network/data_types.gd")


# Parameters
var previous_action = MoveRequestModel.MoveRequestModel.MoveEvent.Idle
var previous_specialization = -1
var previous_gun_rotation: float = PI
var is_active: bool setget set_is_active
var is_dead: bool setget set_is_dead

# abilities
var is_apply_ability_cooldown: bool = false
onready var apply_ability_cooldown_timer = $ApplyAbilityCooldownTimer

var is_use_supply_cooldown: bool = false
onready var use_supply_cooldown_timer = $UseSupplyCooldownTimer

# gun rotation
var is_gun_rotation_cooldown: bool = false
onready var gun_rotation_cooldown_timer = $GunRotationCooldownTimer

# gun reloading and ammunition
var ammo: int = 0
var magazine: int = 0
var current_magazine: int = 0
var is_reloading: bool = false setget set_is_reloading

func set_is_reloading(value):
	is_reloading = value

func reset_ammo_stats(new_ammo: int, new_magazine: int):
	ammo = new_ammo
	magazine = new_magazine
	current_magazine = new_magazine

func update_ammo_stats(new_ammo: int = ammo, new_magazine: int = current_magazine):
	ammo = new_ammo
	current_magazine = new_magazine

func maximize_magazine():
	current_magazine = magazine

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

func get_client_credentials_model():
	var credencials: ClientCredentialsRequestModel.ClientCredentialsRequestModel = ClientCredentialsRequestModel.ClientCredentialsRequestModel.new()
	credencials.set_player_id(player_id)
	credencials.set_username(username)
	credencials.set_token(Global.access_token)
	var network_packet = NetworkPacket.new()
	network_packet.set_data(credencials.to_bytes(), Global.RequestModels.ClientCredentialsRequestModel)
	if (network_packet):
		return network_packet
	return null


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


func get_reload_gun_request():
	if (Input.is_action_pressed("reload") and current_magazine != magazine and not is_reloading):
		set_is_reloading(true)
		player_gun.play_reloading_sound()
		var reload = ReloadWeaponRequestModel.ReloadWeaponRequestModel.new()
		reload.set_player_id(player_id)
		var network_packet = NetworkPacket.new()
		network_packet.set_data(reload.to_bytes(), Global.RequestModels.ReloadWeaponRequestModel)
		# player_gun.play_reloading_sound()	
		if (network_packet):
			return network_packet
	return null

# Ability managment
func start_apply_ability_cooldown():
	is_apply_ability_cooldown = true
	apply_ability_cooldown_timer.start()

func _on_AbilityCooldownTimer_timeout():
	is_apply_ability_cooldown = false

func start_use_supply_cooldown():
	is_use_supply_cooldown = true
	use_supply_cooldown_timer.start()

func _on_UseSupplyCooldownTimer_timeout():
	is_use_supply_cooldown = false

func has_ability():
	if (
		player_specialization == Global.SpecializationTypes.Medic   or
		player_specialization == Global.SpecializationTypes.Support
	):
		return true
	return false

func get_apply_ability_request():
	if (has_ability() and Input.is_action_pressed("apply_ability") and not is_apply_ability_cooldown):
		var ability = ApplyAbilityRequestModel.ApplyAbilityRequestModel.new()
		ability.set_player_id(player_id)
		var network_packet = NetworkPacket.new()
		network_packet.set_data(ability.to_bytes(), Global.RequestModels.ApplyAbilityRequestModel)
		start_apply_ability_cooldown()
		if (network_packet):
			return network_packet
	return null


func get_use_supply_request():
	if (Input.is_action_pressed("use_supply") and not is_use_supply_cooldown):
		var supply = UseSupplyRequestModel.UseSupplyRequestModel.new()
		supply.set_player_id(player_id)
		var network_packet = NetworkPacket.new()
		network_packet.set_data(supply.to_bytes(), Global.RequestModels.UseSupplyRequestModel)
		start_use_supply_cooldown()
		if (network_packet):
			return network_packet
	return null



func _on_GunRotationCooldownTimer_timeout():
	is_gun_rotation_cooldown = false

func start_gun_rotation_cooldown():
	is_gun_rotation_cooldown = true
	gun_rotation_cooldown_timer.start()

func get_player_gun_rotation_request():
	var new_gun_rotation: Vector2 = player_gun.get_gun_rotation()
	if (not is_gun_rotation_cooldown and previous_gun_rotation != new_gun_rotation.angle()):
		var weapon_direction = WeaponDirectionRequestModel.WeaponDirectionRequestModel.new()
		weapon_direction.set_player_id(player_id)
		weapon_direction.new_direction()
		weapon_direction.get_direction().set_x(new_gun_rotation.x)
		weapon_direction.get_direction().set_y(new_gun_rotation.y)

		var network_packet = NetworkPacket.new()
		network_packet.set_data(weapon_direction.to_bytes(), Global.RequestModels.WeaponDirectionRequestModel)
		# create new gun rotation request model
		start_gun_rotation_cooldown()
		previous_gun_rotation = new_gun_rotation.angle()
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
func set_player_info(new_player_id, new_team_id) -> void:
	player_id = new_player_id
	team_id = new_team_id
	print("Set my id: ", player_id)
	print("Set my team id: ", team_id)


