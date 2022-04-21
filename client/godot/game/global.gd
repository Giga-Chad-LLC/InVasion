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
	UnknownRequestModel
}
# Server sends to client
enum ResponseModels {
	PlayerPositionResponseModel = 1000,
	PlayerInfoResponseModel = 1003,
	ShootingStateResponseModel = 1005,
	GameStateResponseModel = 1007,
	UnknownResponseModel
}
