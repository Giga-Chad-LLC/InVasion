extends Node2D

export(int) var left_bound = -1000
export(int) var right_bound = 1000
export(int) var up_bound = -1000
export(int) var down_bound = 1000



# Test out godobuf functionality
const TestProto = preload("res://test_proto.gd")

func test_godobuf():
	# Create packed class (message)
	var a = TestProto.A.new()
	# Example, set field f1
	a.set_f1(3.14159)
	# Pack message A
	# Use to_bytes() method, it's return PoolByteArray
	var packed_bytes = a.to_bytes()
	print("Serialized: ", packed_bytes)
	
	# Create unpacked class (message)
	var b = TestProto.A.new()
	# Unpack byte sequence to class (message) A.
	# Use from_bytes(PoolByteArray my_byte_sequence) method
	var result_code = b.from_bytes(packed_bytes)
	# result_code must be checked (see Unpack result codes section)
	if result_code == TestProto.PB_ERR.NO_ERRORS:
		print("OK")
	else:
		print("Failed")
		return
	# Use class 'a' fields. Example, get field f1
	var f1 = b.get_f1()
	print("Deserialized: ", f1)
	


func _ready():
	# Должно напечатать число Пи (по какой-то причине дважды, тут уже вопросы к методу _ready())
	test_godobuf()
