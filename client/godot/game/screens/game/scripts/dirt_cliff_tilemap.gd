extends TileMap

var STORE_DATA_IN_FILE = false

const DataSavingUtils = preload("res://utils/data_saving.gd")

func _ready():
	var tilemap = self
	var utils = DataSavingUtils.new()
	
	var cellSize = utils.getMapCellSize(tilemap)
	var tilesPositions = utils.getTileCentersGlobalPositions(tilemap)
	
	var filename = utils.create_filename(tilemap.name)
	
	if (STORE_DATA_IN_FILE):
		utils.writeTilemapDataInFile(filename, tilesPositions, cellSize)
