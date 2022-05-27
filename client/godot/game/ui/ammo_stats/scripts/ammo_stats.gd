extends Node

onready var magazine_label = $Magazine
onready var ammo_label = $Ammo

func show_ammo_stats(ammo: int, mazagine: int):
	magazine_label.text = str(mazagine)
	ammo_label.text = str(ammo)
