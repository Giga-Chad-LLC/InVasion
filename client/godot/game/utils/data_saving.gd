
var randomGenerator = RandomNumberGenerator.new()
var RAND_GENERATOR_ESTABLISHED = false

func getMapCellSize(tileMap) -> int:
	return tileMap.cell_size



func getTilesGlobalPositions(tileMap) -> Array:
	var tilesRelativePositions = tileMap.get_used_cells()
	var result = Array()
	
	for tileRelativePos in tilesRelativePositions:
		var tilePos = tileMap.map_to_world(tileRelativePos)
		var globalPos = tileMap.to_global(tilePos)
		result.push_back(globalPos)
	return result



func getTileCentersGlobalPositions(tileMap) -> Array:
	var tiles = getTilesGlobalPositions(tileMap) # Array( Vector2 )
	var cellSize = getMapCellSize(tileMap) # Vector2
	
	for i in range(0, tiles.size()):
		tiles[i] += cellSize / 2
		
	return tiles



func create_filename(name) -> String:
	if (not RAND_GENERATOR_ESTABLISHED):
		RAND_GENERATOR_ESTABLISHED = true
		randomGenerator.randomize()
	
	var filename = "res://assets/statics/collision-assets/" + \
					name + "-" + String(randomGenerator.randi()) + \
					"-static-data.invasion.txt"
	return filename



func writeInFile(file, data, delimiter) -> void:
	file.store_string(String(data))
	file.store_string(delimiter)
	
	

func writeChildrenWithRectangularShapeInFile(filename, parent) -> void:
	var file = File.new()
	file.open(filename, File.WRITE)
	
	if(file.is_open()):
		var suitable_children = []
		
		# iterating through children
		# to find CollisionShape2D child with rect shape
		for child in parent.get_children():
			#var global_position = parent.to_global(child.get_position())
			
			# if direct child is CollisionShape2D with rect shape
			if(child is CollisionShape2D and \
			   child.get_shape() is RectangleShape2D):
				var global_position = parent.to_global(child.get_position())
				var shape = child.get_shape().get_extents() * 2
				suitable_children.push_back([global_position, shape])
				continue
			
			# if child contains subchild 
			# with CollisionShape2D type and rect shape
			for subchild in child.get_children():
				if(subchild is CollisionShape2D and \
				   subchild.get_shape() is RectangleShape2D):
					# maybe 'parent' should be replaced with 'child' on the next line
					var global_position = child.to_global(subchild.get_position())
					var shape = subchild.get_shape().get_extents() * 2
					suitable_children.push_back([global_position, shape])
		
		# write size of array
		writeInFile(file, suitable_children.size(), "\n")
		
		for pair in suitable_children:
			var position = pair[0]
			var shape = pair[1]
			writeInFile(file, shape.x, " ")
			writeInFile(file, shape.y, "\n")
			
			writeInFile(file, position.x, " ")
			writeInFile(file, position.y, "\n")
			
		print("Data successfully has been written to '" + filename + "'")
	else:
		print("File '" , filename, "' cannot be opened")
	
	
	
func writeTilemapDataInFile(filename, tiles, cellSize) -> void:
	var file = File.new()
	file.open(filename, File.WRITE)
	
	if (file.is_open()):		
		writeInFile(file, tiles.size(), "\n")
		
		for tile in tiles:
			writeInFile(file, cellSize.x, " ")
			writeInFile(file, cellSize.y, "\n")
		
			writeInFile(file, tile.x, " ")
			writeInFile(file, tile.y, "\n")
			
		print("Data successfully has been written to '" + filename + "'")
	else:
		print("File '" , filename, "' cannot be opened")
	
