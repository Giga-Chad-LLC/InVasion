extends Reference

var head_stack: Array = []
var tail_stack: Array = []

func push_back(item):
	tail_stack.push_back(item)

func clear():
	head_stack.clear()
	tail_stack.clear()

func pop_front():
	if (head_stack.empty()):
		_rebalance()
	return head_stack.pop_back()

func size():
	return head_stack.size() + tail_stack.size()

func _rebalance():
	while (!tail_stack.empty()):
		head_stack.push_back(tail_stack.pop_back())







