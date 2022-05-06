#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <memory>

// game-models
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-models/GameSession/game-session.h"
// interactors

// controllers

// request-models

// response-models

#include "doctest.h"


namespace doctest {
using namespace invasion::game_models;
// using namespace invasion::interactors;
// using namespace invasion::controllers;
// using namespace request_models;
// using namespace response_models;


TEST_CASE("Deleting players from GameSession") {
	GameSession session;
	const int id1 = session.createPlayerAndReturnId();
	const int id2 = session.createPlayerAndReturnId();
	const int id3 = session.createPlayerAndReturnId();
	const int id4 = session.createPlayerAndReturnId();

	std::vector<int> ids = {
		id1, id2, id3, id4
	};

	int prevSize = session.getPlayers().size();
	int deletedCount = 0;

	for(int id : ids) {
		CHECK(session.getPlayers().size() == prevSize - deletedCount);
		session.removePlayerById(id);
		deletedCount++;
		CHECK(session.getPlayers().size() == prevSize - deletedCount);
	}

	CHECK(session.getPlayers().size() == prevSize - deletedCount);

	// deleting non-existing players
	// session.removePlayerById(-1);
}


}
