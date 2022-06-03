extends YSort

func _ready():
	if (Global.STORE_DATA_IN_FILE):
		Global.saveNodesChildrenCollisionShapesInFile(self)
