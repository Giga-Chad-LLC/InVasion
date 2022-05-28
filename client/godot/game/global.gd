extends Node

func instance_node_at_location(node: Object, parent: Object, location: Vector2) -> Object:
	var node_instance = instance_node(node, parent)
	node_instance.global_position = location
	return node_instance


func instance_node(node: Object, parent: Object) -> Object:
	var node_instance = node.instance()
	parent.add_child(node_instance)
	return node_instance


# Proto files ids
# Client sends to server
enum RequestModels {
	MoveRequestModel = 0,
	UpdateGameStateRequestModel = 1,
	ShootRequestModel = 2,
	RespawnPlayerRequestModel = 3,
	SelectPlayerSpecializationRequestModel = 4,
	ChangePlayerSpecializationRequestModel = 5,
	ApplyAbilityRequestModel = 6,
	UseSupplyRequestModel = 7,
	WeaponDirectionRequestModel = 8,
	ReloadWeaponRequestModel = 9,
	UnknownRequestModel
}
# Server sends to client
enum ResponseModels {
	PlayerPositionResponseModel = 1001,
	WeaponStateResponseModel = 1002,
	GameStateResponseModel = 1003,
	RespawnPlayerResponseModel = 1004,
	GameOverResponseModel = 1005,
	PlayerSpecializationResponseModel = 1006,
	HandshakeResponseModel = 1007,
	SupplyResponseModel = 1008,
	UseSupplyResponseModel = 1009,
	UpdatePlayerAmmoResponseModel = 1010,
	UpdatePlayerHitpointsResponseModel = 1011,
	WeaponDirectionResponseModel = 1012,
	UnknownResponseModel
}

enum SpecializationTypes {
	Stormtrooper = 0,
	Sentinel = 1,
	Support = 2,
	Medic = 3,
	UNDEFINED,
}

enum SupplyType {
	AidKit = 0,
	AmmoCrate = 1,
}

