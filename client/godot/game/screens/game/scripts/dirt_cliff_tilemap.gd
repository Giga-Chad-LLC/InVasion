extends TileMap

var STORE_TILES_IN_FILE = true

const DataSavingUtils = preload("res://utils/data_saving.gd")

func _ready():
	var tilemap = self
	var utils = DataSavingUtils.new()
	
	var cellSize = utils.getMapCellSize(tilemap)
	var tilesPositions = utils.getTileCentersGlobalPositions(tilemap)
	
	var filename = utils.create_filename(tilemap.name)
	
	if (STORE_TILES_IN_FILE):
		utils.writeTilemapDataInFile(filename, tilesPositions, cellSize)
