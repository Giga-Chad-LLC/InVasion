extends Control

export(ButtonGroup) var specialization_button_group

var selected_specialization: int setget set_specialization

func connect_buttons_signals():
	var buttons = specialization_button_group.get_buttons()
	for i in range(0, buttons.size()):
		buttons[i].connect("pressed", self, "set_specialization", [buttons[i].value])

func set_specialization(value):
	# valuesx: {0, 1, 2, 3}
	selected_specialization = value


func mark_selected_button():
	var buttons = specialization_button_group.get_buttons()
	for i in range(0, buttons.size()):
		if (buttons[i].value == selected_specialization):
			buttons[i].set_pressed_no_signal(true)


func _ready():
	selected_specialization = Global.SpecializationTypes.Stormtrooper
	mark_selected_button()
	connect_buttons_signals()
