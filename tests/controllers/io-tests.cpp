#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <memory>

#include "doctest.h"

// game-models
#include "game-models/Vector2D/vector2d.h"
#include "game-models/GameSession/game-session.h"
#include "game-models/StaticObject/static-object.h"
// controllers
#include "controllers/TilemapsFileReader/tilemaps-file-reader.h"

namespace doctest {
using namespace invasion::controllers;
using namespace invasion::game_models;


TEST_CASE("Iterating through directory in GameSession") {
	GameSession session;

	const auto& obstacles = session.getObstacles();

	for(std::shared_ptr<StaticObject> object : obstacles) {
		std::cout << object->getPosition() << '\n';
	}
}

/*
TEST_CASE("TilemapsFileReader testing") {
	// client/godot/game/assets/tilemaps/DirtCliffTileMap-tiles-data.invasion.txt

	std::string filepath("client/godot/game/assets/tilemaps/DirtCliffTileMap-tiles-data.invasion.txt");
	
	try {
		TilemapsFileReader reader(filepath);

		std::pair<double, double> dims_ = reader.getTileDimensions();
		int tilesNumber = reader.getTilesNumber();
		std::vector<std::pair<double, double>> centers = reader.getTileCentersPositions();

		Vector2D dims(dims_.first, dims_.second);
		
		std::cout << dims << std::endl;
		std::cout << tilesNumber << std::endl;

		for(auto center_ : centers) {
			Vector2D center(center_.first, center_.second);
			std::cout << center << '\n';
		}

		std::cout << reader.getFilePath() << std::endl;
	}
	catch(const std::exception& err) {
		std::cout << err.what() << '\n';
		CHECK_MESSAGE(false, "Error occured");
	}
}
*/


}
