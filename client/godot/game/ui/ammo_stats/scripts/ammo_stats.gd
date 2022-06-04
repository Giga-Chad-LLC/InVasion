extends Node

onready var magazine_label = $Magazine
onready var ammo_label = $Ammo

var ammo: int = 0
var magazine: int = 0
var current_ammo: int = 0
var current_magazine: int = 0

func reset_ammo_stats(new_ammo: int, new_magazine: int):
	ammo = new_ammo
	magazine = new_magazine
	current_magazine = new_magazine
	current_ammo = new_ammo
	show_ammo_stats()

func update_ammo_stats(new_ammo: int = current_ammo, new_magazine: int = current_magazine):
	current_ammo = new_ammo
	current_magazine = new_magazine
	show_ammo_stats()

func maximize_magazine():
	current_ammo = ammo
	current_magazine = magazine
	show_ammo_stats()


func show_ammo_stats():
	ammo_label.text = str(current_ammo)
	magazine_label.text = str(current_magazine)
