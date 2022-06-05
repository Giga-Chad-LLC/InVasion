#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <filesystem>

#include "doctest.h"

// game-models
#include "game-models/Vector2D/vector2d.h"
#include "game-models/GameSession/game-session.h"
#include "game-models/StaticObject/static-object.h"
// controllers
#include "controllers/StaticObjectsFileReader/static-objects-file-reader.h"
#include "controllers/DirectoryFilesContainer/directory-files-container.h"

namespace doctest {
using namespace invasion::controllers;
using namespace invasion::game_models;




TEST_CASE("Obtaining files by extension") {
	namespace fs = std::filesystem;
	const std::string directory("config/game-assets/collision-assets");

	DirectoryFilesContainer container(directory);

	std::vector<fs::directory_entry> entries = container.obtainFilesWithExtension(".txt");

	for(const auto& entry : entries) {
		StaticObjectsFileReader reader(entry.path().string());
	}
}



}
