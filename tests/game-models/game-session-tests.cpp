#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <memory>

// game-models
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-models/GameSession/game-session.h"
#include "game-models/GameSession/game-session-stats.h"
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


TEST_CASE("Distributing players by teams") {
	GameSession session;

	for(int i = 0; i < 1000; i++) {
		session.createPlayerAndReturnId();
		// temporary method
		GameSessionStats stats = session.getStats_debug();
		const int count1 = stats.getFirstTeamPlayersCount();
		const int count2 = stats.getSecondTeamPlayersCount();

		// std::cout << "players count: " << session.getPlayers().size() 
		// 		  << "  first team: " << count1
		// 		  << "  second team: " << count2 << '\n';

		CHECK(std::abs(count1 - count2) <= 1);

		if(i % 2 == 0) {
			CHECK(std::abs(count1 - count2) == 1); 
		}
		else {
			CHECK(std::abs(count1 - count2) == 0); 
		}
	}
}


/*
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
}*/


}
