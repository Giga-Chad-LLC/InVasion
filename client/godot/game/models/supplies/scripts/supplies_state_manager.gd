extends Reference

class_name SuppliesStateManager

const human_aid_kit_scene = preload("res://models/supplies/human_aid_kit.tscn")
const human_ammo_crate_scene = preload("res://models/supplies/human_ammo_crate.tscn")
const alien_aid_kit_scene = preload("res://models/supplies/alien_aid_kit.tscn")
const alien_ammo_crate_scene = preload("res://models/supplies/alien_ammo_crate.tscn")

func spawn_supply(data, supplies_parent_node, location):
	# data = { supply_type, supply_id }
	var supply_type = data["supply_type"]
	var supply_id = data["supply_id"]
	var team_id = data["team_id"]
	
	var supply_scene = null
	if (supply_type == Global.SupplyType.AidKit):
		if (team_id == Global.TeamId.Humans):
			supply_scene = human_aid_kit_scene
		else:
			supply_scene = alien_aid_kit_scene
	elif (supply_type == Global.SupplyType.AmmoCrate):
		if (team_id == Global.TeamId.Humans):
			supply_scene = human_ammo_crate_scene
		else:
			supply_scene = alien_ammo_crate_scene
	else:
		print("Unknown supply type: ", supply_type)
		return
	
	var supply = Global.instance_node_at_location(supply_scene, supplies_parent_node, location)
	supply.name = str(supply_id)
	supply.supply_id = supply_id
	supply.supply_type = supply_type

var supplies_on_map = [] # ids of supplies on map
func update_supplies_states(supplies: Array, supplies_parent_node):
	for i in range(0, supplies.size()):
		var supply = supplies[i]
		if (supplies_on_map.find(supply.get_supply_id()) == -1):
			var data = {
				"supply_type": supply.get_supply_type(),
				"supply_id": supply.get_supply_id(),
				"team_id": supply.get_player_team_id()
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

func update_used_supply(supply_id, supplies_parent_node):
	var supply = supplies_parent_node.get_node(str(supply_id))
	if (supply):
		supply.play_use_animation()
	
