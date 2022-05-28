extends Node

onready var magazine_label = $Magazine
onready var ammo_label = $Ammo

var ammo: int = 0
var magazine: int = 0

func update_ammo_stats(new_ammo: int = ammo, new_magazine: int = magazine):
	ammo = new_ammo
	magazine = new_magazine
	show_ammo_stats()

func show_ammo_stats():
	ammo_label.text = str(ammo)
	magazine_label.text = str(magazine)

