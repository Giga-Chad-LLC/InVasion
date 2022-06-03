extends TileMap

func _ready():
	if (Global.STORE_DATA_IN_FILE):
		Global.saveTilemapCellsInFile(self)
