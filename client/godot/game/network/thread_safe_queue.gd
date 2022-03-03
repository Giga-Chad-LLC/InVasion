extends Reference
# Refactor it with the more efficient version based on two arrays 
var queue: Array = []
var mutex = Mutex.new()

func size():
	mutex.lock()
	var size = queue.size()
	mutex.unlock()
	return size

func is_empty():
	return queue.size() == 0

func push(object: Object):
	mutex.lock()
	queue.push_back(object)
	mutex.unlock()

func pop():
	if queue.size() == 0:
		return null
		
	mutex.lock()
	var item: Object = queue.pop_front()
	mutex.unlock()
	return item


func clear():
	mutex.lock()
	queue.clear()
	mutex.unlock()
