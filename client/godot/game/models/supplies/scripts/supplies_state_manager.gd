extends Reference

class_name SuppliesStateManager

var aid_kit_scene = preload("res://models/supplies/aid_kit.tscn")
var ammo_crate_scene = preload("res://models/supplies/ammo_crate.tscn")

func spawn_supply(data, supplies_parent_node, location):
	# data = { supply_type, supply_id }
	print("Supply: ", data)
	var supply_type = data["supply_type"]
	var supply_id = data["supply_id"]
	
	var supply_scene = null
	if (supply_type == Global.SupplyType.AidKit):
		supply_scene = aid_kit_scene
		print("Aid kit")
	elif (supply_type == Global.SupplyType.AmmoCrate):
		supply_scene = ammo_crate_scene
		print("Ammo crate")
	else:
		print("Unknown supply type: ", supply_type)
		return
	
	var supply = Global.instance_node_at_location(supply_scene, supplies_parent_node, location)
	supply.name = str(supply_id)

var supplies_on_map = [] # ids of supplies on map
func update_supplies_states(supplies: Array, supplies_parent_node):
	for i in range(0, supplies.size()):
		var supply = supplies[i]
		if (supplies_on_map.find(supply.get_supply_id()) == -1):
			var data = {
				"supply_type": supply.get_supply_type(),
				"supply_id": supply.get_supply_id()
			}
			supplies_on_map.push_back(supply.get_supply_id())
			spawn_supply(
				data,
				supplies_parent_node,
				Vector2(
					supply.get_position().get_x(),
					supply.get_position().get_y()
				)
			)
