extends YSort

var STORE_DATA_IN_FILE = true

const DataSavingUtils = preload("res://utils/data_saving.gd")

func _ready():
	if(STORE_DATA_IN_FILE):
		var utils = DataSavingUtils.new()
		
		var filename = utils.create_filename(self.name)
		utils.writeChildrenWithRectangularShapeInFile(filename, self)
