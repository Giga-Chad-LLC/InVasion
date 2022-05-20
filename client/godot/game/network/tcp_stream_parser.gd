extends Reference

class_name TCPStreamParser


var data: PoolByteArray = [] # buffer with the unhandled bytes
var caret = 0 # pointer to the current byte
var byte_encoder: StreamPeer = StreamPeerBuffer.new() # used to decode message length

func add_data(new_data: PoolByteArray) -> void:
	for i in range(0, new_data.size()):
		self.data.push_back(new_data[i])

func get_next_packet_sequence() -> PoolByteArray:
	var chunk: PoolByteArray = []
	if (self.data.size() - caret < 8):
		return chunk
	
	var length = self.get_length()
	if (self.data.size() - caret < length + 8):
		return chunk
	
	for i in range(0, length + 8):
		chunk.push_back(self.data[self.caret])
		self.caret += 1
	return chunk

func get_length() -> int:
	self.byte_encoder.data_array = [
		data[caret],
		data[caret + 1],
		data[caret + 2],
		data[caret + 3]
	]
	return self.byte_encoder.get_32()

func flush() -> void:
	var left_data: PoolByteArray = []
	while (self.caret < self.data.size()):
		left_data.push_back(self.data[self.caret])
		self.caret += 1
	self.data = left_data
	self.caret = 0
