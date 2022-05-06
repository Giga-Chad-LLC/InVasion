#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <memory>
#include <thread>
#include <chrono>


// game-models
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Weapon/weapon.h"
#include "game-models/Bullet/bullet.h"
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


TEST_CASE("Updating kills counts of teams") {
	using namespace std::chrono_literals;
	GameSession session;
	std::shared_ptr<Player> player1 = session.getPlayer(session.createPlayerAndReturnId());
	Weapon& weapon = player1->getWeapon();
	weapon.setDirection(Vector2D(0, 1));
	const Vector2D position = player1->getPosition();

	std::shared_ptr<Player> player2 = session.getPlayer(session.createPlayerAndReturnId());
	player2->setPosition(Vector2D(0, 15));

	for(int i = 0; i < 10; i++) {
		const int bulletId = session.createIdForNewBullet();		
		std::shared_ptr<Bullet> bullet = weapon.shoot(position, bulletId);

		session.addBullet(bullet);

		std::this_thread::sleep_for(30ms);
		session.updateGameState();
	}
	
	for(int i = 0; i < 10; i++) {
		std::this_thread::sleep_for(30ms);
		session.updateGameState();
	}

	// logging
	{
		std::vector<std::shared_ptr<Bullet>>& bullets = session.getBullets();
		for(const auto& bullet_ptr : bullets) {
			std::cout << "bullet id: " << bullet_ptr->getId() << " position: " << bullet_ptr->getPosition() << '\n';
		}
		std::cout << "player2 position: " << player2->getPosition() << std::endl;
		std::cout << "player2 life state: " << player2->getLifeState().isInDeadState() << std::endl;
		std::cout << "player2 HP: " << player2->getLifeState().getHitPoints() << std::endl;
	}

	GameSessionStats stats = session.getStats_debug();

	if (player1->getTeamId() == PlayerTeamId::FirstTeam) {
		CHECK(stats.getFirstTeamKillsCount() == 1);
		CHECK(stats.getSecondTeamKillsCount() == 0);
	}
	else if (player1->getTeamId() == PlayerTeamId::SecondTeam) {
		CHECK(stats.getFirstTeamKillsCount() == 0);
		CHECK(stats.getSecondTeamKillsCount() == 1);
	}
}


/*
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
}*/


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
