extends TileMap

var STORE_TILES_IN_FILE = true

const TilemapUtils = preload("res://utils/tilemap_saving.gd")

func _ready():
	var tilemap = self
	var utils = TilemapUtils.new()
	
	var cellSize = utils.getMapCellSize(tilemap)
	var tilesPositions = utils.getTileCentersGlobalPositions(tilemap)
	
	var filename = utils.create_filename(tilemap.name)
	
	if (STORE_TILES_IN_FILE):
		utils.writeTilemapDataInFile(filename, tilesPositions, cellSize)
