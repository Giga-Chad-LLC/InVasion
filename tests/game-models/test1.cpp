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
#include "interactors/ShootInteractor/shoot-interactor.h"
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




// TEST_CASE("Testing players collisions with bullets") {

// 	FixedTimeIntervalInvoker controller(10);
// 	ShootInteractor shoot_interactor;
// 	GameSession session;
	
// 	const int id1 = session.createPlayerAndReturnId();
// 	const int id2 = session.createPlayerAndReturnId();

// 	std::shared_ptr<Player> player1 = session.getPlayer(id1);
// 	player1->setPosition(Vector2D(0, 0));

// 	std::shared_ptr<Player> player2 = session.getPlayer(id2);
// 	player2->setPosition(Vector2D(500, 0));

// 	ShootRequestModel req;
// 	req.set_player_id(player1->getId());

// 	const Vector2D direction = Vector2D(1, 0).normalize();
// 	req.mutable_weapon_direction()->set_x(direction.getX());
// 	req.mutable_weapon_direction()->set_y(direction.getY());

// 	/* ShootingStateResponseSchema res = */ shoot_interactor.execute(req, session);
// 	shoot_interactor.execute(req, session);

// 	req.mutable_weapon_direction()->set_x(-1);
// 	req.mutable_weapon_direction()->set_y(0);

// 	shoot_interactor.execute(req, session);

// 	req.mutable_weapon_direction()->set_x(direction.getX());
// 	req.mutable_weapon_direction()->set_y(direction.getY());

// 	shoot_interactor.execute(req, session);

// 	controller.start([&]() {
// 		session.updateGameState();
// 	});

// 	std::this_thread::sleep_for(std::chrono::milliseconds(1'500));
// 	controller.stop();
// 	std::cout << "HP: " << player2->getHitPoints() << std::endl;
// }


/*
TEST_CASE("Testing collision checking in GameWorldManager | moving diagonal top-right") {
	GameSession session;
	const int id1 = session.createPlayerAndReturnId();
	const int id2 = session.createPlayerAndReturnId();

	Player& player1 = session.getPlayer(id1);
	Player& player2 = session.getPlayer(id2);

	player2.setPosition(Vector2D(100, 100));

	
	MoveInteractor moveInteractor;

	MoveRequestModel req;
	req.set_player_id(id1);

	req.set_current_event(MoveRequestModel::StartMovingRight);
	moveInteractor.execute(req, session);

	req.set_current_event(MoveRequestModel::StartMovingDown);
	moveInteractor.execute(req, session);

	PlayersPositionsResponseInteractor updateInteractor;

	FixedTimeIntervalInvoker controller(100);
	controller.start([&]() mutable {
		// std::cout << "position of player #" << id1 << ": " << player1.getPosition() << std::endl; 
		updateInteractor.execute(session);
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(1500));
}*/


/*
TEST_CASE("Testing collision checking in GameWorldManager | moving straight right") {
	GameSession session;
	const int id1 = session.createPlayerAndReturnId();
	const int id2 = session.createPlayerAndReturnId();

	Player& player1 = session.getPlayer(id1);
	Player& player2 = session.getPlayer(id2);

	player2.setPosition(Vector2D(100, 0));

	
	MoveRequestModel req;
	req.set_player_id(id1);
	req.set_current_event(MoveRequestModel::StartMovingRight);

	MoveInteractor moveInteractor;
	moveInteractor.execute(req, session);

	PlayersPositionsResponseInteractor updateInteractor;

	FixedTimeIntervalInvoker controller(100);
	controller.start([&]() mutable {
		// std::cout << "position of player #" << id1 << ": " << player1.getPosition() << std::endl; 
		updateInteractor.execute(session);
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(1500));
}
*/

/*
TEST_CASE("Testing players positions response model") {
	GameSession session;
	std::vector<Player> players;

	for(int i = 0; i < 100; i++) {
		int id = session.createPlayerAndReturnId();
		auto player = session.getPlayer(id);
		players.push_back(player);
	}

	PlayersPositionsResponseModel res;

	for(int i = 0; i < players.size(); i++) {
		PlayerPositionResponseModel* playerModel = res.add_players();
		
		const int id = players[i].getId();
		const int vel_x = players[i].getVelocity().getX();
		const int vel_y = players[i].getVelocity().getY();
		const int pos_x = players[i].getPosition().getX();
		const int pos_y = players[i].getPosition().getY();

		playerModel->set_playerid(id);
		
		playerModel->mutable_velocity()->set_x(vel_x);
		playerModel->mutable_velocity()->set_y(vel_y);

		playerModel->mutable_position()->set_x(pos_x);
		playerModel->mutable_position()->set_y(pos_y);
	}

	for(const auto player : res.players()) {
		const int id = player.player_id();
		std::cout << id << std::endl;
	} 
}*/


/*
TEST_CASE("Team assigning") {
	GameSession session;
	int id1 = session.createPlayerAndReturnId();
	int id2 = session.createPlayerAndReturnId();

	Player player1 = session.getPlayer(id1);
	Player player2 = session.getPlayer(id2);

	int teamId1 = player1.getTeamId() == PlayerTeamId::SecondTeam;
	int teamId2 = player2.getTeamId() == PlayerTeamId::SecondTeam;

	CHECK(player1.getTeamId() != player2.getTeamId());

	// std::cout << "playerId: " << id1 << " | team id: " << teamId1 << std::endl;
	// std::cout << "playerId: " << id2 << " | team id: " << teamId2 << std::endl;

	int cnt1 = 1;
	int cnt2 = 1;

	for(int i = 0; i < 10'000; i++) {
		int id = session.createPlayerAndReturnId();
		Player player = session.getPlayer(id);
		int teamId = player.getTeamId() == PlayerTeamId::SecondTeam;

		if(teamId == 1) cnt2++;
		else cnt1++;

		if(i % 2 == 0) {
			CHECK(std::abs(cnt1 - cnt2) == 1);
		}
		else {
			CHECK(cnt1 == cnt2);
		}

		// std::cout << "playerId: " << id << " | team id: " << teamId << std::endl;
	}
}
*/


/*
TEST_CASE("ShootInteractor test") {
	FixedTimeIntervalInvoker controller(10);
	ShootInteractor shoot_interactor;
	GameSession session;
	Player& player = session.getPlayer(session.createPlayerAndReturnId());

	ShootRequestModel req;
	req.set_player_id(player.getId());

   std::uniform_real_distribution<double> unif(-1.0, 1.0);
   std::default_random_engine re;

	controller.start([&]() {
		const Vector2D direction = Vector2D(unif(re), unif(re)).normalize();
		req.mutable_weapon_direction()->set_x(direction.getX());
		req.mutable_weapon_direction()->set_y(direction.getY());

		ShootingStateResponseSchema res = shoot_interactor.execute(req, session);
		std::cout << "magazine: " << res.left_magazine() << "   ammo: " << res.left_ammo() << "   ";
		std::cout << "direction: (" << res.weapon_direction().x() << ", " << res.weapon_direction().y() << ")";

		if(res.is_reloading())
			std::cout << "   | reloading...";
		else if(res.is_reloading_required()) {
			std::cout << "   | reloading required!";
			player.getWeapon().reload();
		}
		
		std::cout << std::endl;
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(5'000));
	controller.stop();
}
*/


/*
TEST_CASE("RotateWeaponInteractor test") {
	FixedTimeIntervalInvoker controller(100);
	RotateWeaponInteractor interactor;
	GameSession session;
	Player& player = session.getPlayer(session.createPlayerAndReturnId());

	RotateWeaponRequestModel req;
	req.set_player_id(player.getId());
	req.set_session_id(0);

	std::cout << player.getWeapon().getDirection() << std::endl;

   std::uniform_real_distribution<double> unif(-1.0, 1.0);
   std::default_random_engine re;
	controller.start([&]() {
		const double x = unif(re);
		const double y = unif(re);

		req.mutable_direction()->set_x(x);
		req.mutable_direction()->set_y(y);

		std::cout << "generated: " << x << " " << y << ": " << Vector2D(x, y).normalize() << std::endl;
		interactor.execute(req, session);
		std::cout << player.getWeapon().getDirection() << std::endl;
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	controller.stop();
}*/


/*
TEST_CASE("Weapon testing") {
	int ammo = 23;
	Vector2D pos(0, 0);
	[[maybe_unused]] int bulletId = 0;
	Weapon weapon(0, ammo, 10.0);


	std::cout << "magazine: " << weapon.getLeftMagazine() 
		<< "   ammo: " << weapon.getLeftAmmo() 
		<< "   reloading: " << weapon.isReloading() << std::endl;


	FixedTimeIntervalInvoker controller(100);

	controller.start([&]() mutable {
		if(bulletId % 10 == 0) {
			weapon.reload();
		}

		bool able = weapon.isAbleToShoot();
		if(able) {
			weapon.shoot(pos, bulletId++);
		}
		else {
			if(!weapon.isReloading()) {
				weapon.reload();
			}

			std::cout << "reloading..." << std::endl;
			while(weapon.isReloading()) {}
		}
		std::cout << "magazine: " << weapon.getLeftMagazine() 
			<< "   ammo: " << weapon.getLeftAmmo() 
			<< "   reloading: " << weapon.isReloading() << std::endl;
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(10'000));
	controller.stop();
}*/


/*
TEST_CASE("Collision test") {
	Player p1(Vector2D{-10, 0}, 0);
	Player p2(Vector2D{20, 0}, 1);

	std::cout << p1.collidesWith(&p2) << std::endl;
}



TEST_CASE("Set interval test") {
	FixedTimeIntervalInvoker controller(100);

	int cnt = 0;
	controller.start([&cnt]() {
		std::cout << ++cnt << std::endl;
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	controller.stop();
}

*/


/*
TEST_CASE("Player moving test") {
	int totalSimulationTime = 40;
    int currentTime = 0;
    int dt = 1;

	MoveInteractor move_interactor;
	PlayersPositionsResponseInteractor update_interactor;

	GameSession session;
	Player& player = session.getPlayer(session.createPlayerAndReturnId());

	// creating request
	MoveRequestModel req;
	req.set_player_id(player.getId());
	req.set_current_event(MoveRequestModel::StartMovingUp);

	// start moving
	move_interactor.execute(req, session);

	while(currentTime < totalSimulationTime) {
		//cout << "time: " << currentTime << " velocity: " << player.getVelocity() << endl;
		// stop event
		if(currentTime == 20) {
			std::cout << "start stopping..." << std::endl;
			req.set_current_event(MoveRequestModel::StopMovingUp);
			move_interactor.execute(req, session);
		}

		// session.updateGameState();
		PlayerPositionResponseModel res = update_interactor.execute(session);
		cout << "time: " << currentTime << " position: x=" << res.position().x() << " y=" << res.position().y() << std::endl;
		currentTime += dt;
	}

	//cout << "time: " << currentTime << " velocity: " << player.getVelocity() << endl;
}*/


} // namespace doctest
