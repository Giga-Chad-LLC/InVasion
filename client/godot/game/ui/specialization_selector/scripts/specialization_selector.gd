extends Control

export(ButtonGroup) var specialization_button_group

signal change_specialization(player_specialization)

var selected_specialization: int setget set_specialization

func connect_buttons_signals():
	var buttons = specialization_button_group.get_buttons()
	for i in range(0, buttons.size()):
		buttons[i].connect("pressed", self, "set_specialization", [buttons[i].value])

func set_specialization(value):
	# valuesx: {0, 1, 2, 3}
	selected_specialization = value
	emit_signal("change_specialization", selected_specialization)

func set_default_specialization(value):
	selected_specialization = value
	var buttons = specialization_button_group.get_buttons()
	for i in range(0, buttons.size()):
		if (buttons[i].value == selected_specialization):
			buttons[i].set_pressed_no_signal(true)

func _ready():
	set_default_specialization(Global.SpecializationTypes.Stormtrooper)
	connect_buttons_signals()
