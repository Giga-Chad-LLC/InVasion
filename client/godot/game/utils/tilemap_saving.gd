

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



func create_filename(tilemap_name) -> String:
	var filename = "res://assets/tilemaps/" + \
					tilemap_name + \
					"-tiles-data.invasion.txt"
	return filename



func writeInFile(file, data, delimiter) -> void:
	file.store_string(String(data))
	file.store_string(delimiter)
	

	
func writeTilemapDataInFile(filename, tiles, cellSize) -> void:
	var file = File.new()
	file.open(filename, File.WRITE)
	
	if (file.is_open()):
		writeInFile(file, cellSize.x, " ")
		writeInFile(file, cellSize.y, "\n")
		
		writeInFile(file, tiles.size(), "\n")
		
		for tile in tiles:
			writeInFile(file, tile.x, " ")
			writeInFile(file, tile.y, "\n")
			
		print("Data successfully has been written to '" + filename + "'")
	else:
		print("File '" , filename, "' cannot be opened")
	
