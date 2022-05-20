extends Node

var mutex: Mutex = Mutex.new()
var semaphore: Semaphore = Semaphore.new()
var thread: Thread = Thread.new()
var exit_thread: bool = false

const ThreadSafeQueue = preload("res://network/thread_safe_queue.gd")
var queue: ThreadSafeQueue = ThreadSafeQueue.new()


# The thread will start here.
func init(function: FuncRef):
	thread.start(self, "_worker_function", function)

func _worker_function(function: FuncRef):
	if (function):
		function.call_funcv([self])
	else:
		print("Worker didn't run the function: ", function)

func push_data(data):
	if (data != null):
		queue.push(data)
		semaphore.post()


func pop_data():
	return queue.pop()

# Thread must be disposed (or "joined"), for portability.
func _exit_tree():
	if (thread.is_active()):
		# Set exit condition to true.
		mutex.lock()
		exit_thread = true # Protect with Mutex.
		mutex.unlock()

		# Unblock by posting.
		semaphore.post()
		# Wait until it exits.
		thread.wait_to_finish()
		queue.clear()
