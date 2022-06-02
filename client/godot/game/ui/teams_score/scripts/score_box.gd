extends Node


export(Color) var color = Color(0.39, 0.53, 1) # blue
onready var Background = $Background

func _ready():
	Background.color = color
