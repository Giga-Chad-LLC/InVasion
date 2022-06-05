#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <memory>

// game-models
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/GameSession/game-session.h"
#include "game-models/Bullet/bullet.h"
#include "game-models/Weapon/weapon.h"
// interactors
#include "interactors/MoveInteractor/move-interactor.h"
#include "interactors/PlayersPositionsResponseInteractor/players-positions-response-interactor.h"
#include "interactors/RotateWeaponInteractor/rotate-weapon-interactor.h"
#include "interactors/ShootResponseInteractor/shoot-response-interactor.h"
// controllers
#include "controllers/FixedTimeIntervalInvoker/fixed-time-interval-invoker.h"


// request-models
#include "shoot-request-model.pb.h"
#include "move-request-model.pb.h"
#include "update-game-state-request-model.pb.h"
// response-models
#include "player-position-response-model.pb.h"



#include "doctest.h"


namespace doctest {
using namespace invasion::game_models;
using namespace invasion::interactors;
using namespace invasion::controllers;

using namespace request_models;
using namespace response_models;
using namespace std;




TEST_CASE("Testing players collisions with bullets") {
	FixedTimeIntervalInvoker controller(10);
	ShootResponseInteractor shoot_interactor;
	GameSession session;
	
	const int id1 = session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);
	const int id2 = session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);

	std::shared_ptr<Player> player1 = session.getPlayer(id1);
	player1->setPosition(Vector2D(0, 0));

	std::shared_ptr<Player> player2 = session.getPlayer(id2);
	player2->setPosition(Vector2D(500, 0));

	ShootRequestModel req;
	req.set_player_id(player1->getId());

	const Vector2D direction = Vector2D(1, 0).normalize();
	req.mutable_weapon_direction()->set_x(direction.getX());
	req.mutable_weapon_direction()->set_y(direction.getY());

	// ShootingStateResponseSchema res = shoot_interactor.execute(req, session);
	shoot_interactor.execute(req, session);

	req.mutable_weapon_direction()->set_x(-1);
	req.mutable_weapon_direction()->set_y(0);

	shoot_interactor.execute(req, session);

	req.mutable_weapon_direction()->set_x(direction.getX());
	req.mutable_weapon_direction()->set_y(direction.getY());

	shoot_interactor.execute(req, session);

	controller.start([&]() {
		session.updateGameState();
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(1'500));
	controller.stop();
	std::cout << "HP: " << player2->getLifeState().getHitPoints() << std::endl;
}





TEST_CASE("Team assigning") {
	GameSession session;
	int id1 = session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);
	int id2 = session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);

	std::shared_ptr<Player> player1 = session.getPlayer(id1);
	std::shared_ptr<Player> player2 = session.getPlayer(id2);

	CHECK(player1->getTeamId() != player2->getTeamId());

	int cnt1 = 1;
	int cnt2 = 1;

	for(int i = 0; i < 1000; i++) {
		int id = session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);
		std::shared_ptr<Player> player = session.getPlayer(id);
		int teamId = player->getTeamId() == PlayerTeamId::SecondTeam;

		if(teamId == 1) cnt2++;
		else cnt1++;

		if(i % 2 == 0) {
			CHECK(std::abs(cnt1 - cnt2) == 1);
		}
		else {
			CHECK(cnt1 == cnt2);
		}
	}
}





TEST_CASE("RotateWeaponInteractor test") {
	FixedTimeIntervalInvoker controller(100);
	RotateWeaponInteractor interactor;
	GameSession session;

	const int id = session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);
	std::shared_ptr<Player> player = session.getPlayer(id);

	RotateWeaponRequestModel req;
	req.set_player_id(player->getId());
	req.set_session_id(0);

	std::cout << player->getWeapon().getDirection() << std::endl;

   std::uniform_real_distribution<double> unif(-1.0, 1.0);
   std::default_random_engine re;
	controller.start([&]() {
		const double x = unif(re);
		const double y = unif(re);

		req.mutable_direction()->set_x(x);
		req.mutable_direction()->set_y(y);

		std::cout << "generated: " << x << " " << y << ": " << Vector2D(x, y).normalize() << std::endl;
		interactor.execute(req, session);
		std::cout << player->getWeapon().getDirection() << std::endl;
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	controller.stop();
}



} // namespace doctest
