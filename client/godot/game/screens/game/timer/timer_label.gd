extends Label
var initial_mins
var initial_secs
var is_working = false


var cur_time = 0

func init(init_mins, init_secs):
	is_working = true
	initial_mins = init_mins
	initial_secs = init_secs
	
	
	
func _process(delta):
	if (is_working):
		cur_time += delta
		var start_min_ = initial_mins - fmod(cur_time, 60 * 60) / 60
		var start_secs_ = initial_secs - fmod(cur_time, 60)
		var time_passed = "%02d : %02d" % [start_min_, start_secs_]
		text = time_passed
