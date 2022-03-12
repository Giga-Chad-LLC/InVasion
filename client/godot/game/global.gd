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
	PlayerActionRequestModel = 2
}
# Server sends to client
enum ResponseModels {
	PlayerPositionResponseModel = 1000,
	PlayerActionResponseModel = 1002,
	PlayerIdResponseModel = 1003
}


#	var m = Model.PlayerPositionResponseModel.new()
#	var v = m.new_velocity()
#	v.set_x(10.5)
#	v.set_y(32.3213)
#	m.set_playerId(10)
#
#	var p = m.new_position()
#	p.set_x(1000)
#	p.set_y(-100.3213)
#
#	print(m.to_string())
#	print(m.to_bytes())
#	client.send_packed_data(m.to_bytes(), Global.ResponseModels.PlayerPositionResponseModel)
	# Run producer (thread that sends data to server)

