extends Reference
class_name NetworkPacket

var bytes: PoolByteArray = [] setget set_bytes, get_bytes
var message_type: int = 0 setget set_message_type


func set_bytes(bytes_array: PoolByteArray):
	bytes = bytes_array

func get_bytes():
	return bytes

func set_data(bytes_array: PoolByteArray, type: int):
	self.set_bytes(bytes_array)
	self.set_message_type(type)

func set_message_type(type: int):
	message_type = type

