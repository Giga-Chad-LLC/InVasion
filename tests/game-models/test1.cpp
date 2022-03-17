#include <iostream>
#include <vector>
#include <random>

// game-models
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-models/GameSession/game-session.h"
#include "game-models/GameWorldManager/game-world-manager.h"
#include "game-models/Bullet/bullet.h"
#include "game-models/Weapon/weapon.h"
// interactors
#include "interactors/MoveInteractor/move-interactor.h"
#include "interactors/UpdateGameStateInteractor/update-game-state-interactor.h"
#include "interactors/RotateWeaponInteractor/rotate-weapon-interactor.h"
#include "interactors/ShootInteractor/shoot-interactor.h"
// controllers
#include "controllers/PhysicsTickController/physics-tick-controller.h"
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


/*
TEST_CASE("ShootInteractor test") {
	PhysicsTickController controller(10);
	ShootInteractor shoot_interactor;
	GameSession session;
	Player& player = session.getPlayer(session.addPlayer());

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
	PhysicsTickController controller(100);
	RotateWeaponInteractor interactor;
	GameSession session;
	Player& player = session.getPlayer(session.addPlayer());

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


	PhysicsTickController controller(100);

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
	PhysicsTickController controller(100);

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
	UpdateGameStateInteractor update_interactor;

	GameSession session;
	Player& player = session.getPlayer(session.addPlayer());

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
