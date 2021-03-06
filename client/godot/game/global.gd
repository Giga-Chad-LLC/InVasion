extends Node

var NETWORK_HOST: String = "127.0.0.1"
var TCP_PORT: int = 8000
var HTTP_PORT: int = 8001

var access_token: String = ""
var username: String = ""

var STORE_STATIC_ASSETS_COLLISION_SHAPES_IN_FILE = false
var STORE_RESPAWN_POINTS_IN_FILE = false

const DataSavingUtils = preload("res://utils/data_saving.gd")

func saveNodesChildrenCollisionShapesInFile(node):
	var utils = DataSavingUtils.new()
	
	var filename = utils.create_collision_asset_filename(node.name)
	utils.writeChildrenWithRectangularShapeInFile(filename, node)



func saveTilemapCellsInFile(tilemap):
	var utils = DataSavingUtils.new()
	
	var cellSize = utils.getMapCellSize(tilemap)
	var tilesPositions = utils.getTileCentersGlobalPositions(tilemap)
	
	var filename = utils.create_collision_asset_filename(tilemap.name)
	
	utils.writeTilemapDataInFile(filename, tilesPositions, cellSize)



func save_aliens_respawn_points_in_file(node):
	var utils = DataSavingUtils.new()
	var filename = utils.create_aliens_respawn_points_filename(node.name)
	utils.writeChildrenPointsDataInFile(filename, node)



func save_humans_respawn_points_in_file(node):
	var utils = DataSavingUtils.new()
	var filename = utils.create_humans_respawn_points_filename(node.name)
	utils.writeChildrenPointsDataInFile(filename, node)



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
	ClientCredentialsRequestModel = 10,
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
	UsernameResponseModel = 1013,
	ClientConnectedResponseModel = 1014,
	ClientDisconnectedResponseModel = 1015,
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


enum TeamId {
	Humans = 0,
	Aliens = 1,
}
