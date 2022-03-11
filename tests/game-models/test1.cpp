#include <iostream>
#include <vector>

// game-models
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-models/GameSession/game-session.h"
#include "game-models/GameWorldManager/game-world-manager.h"
#include "game-models/Bullet/bullet.h"
#include "game-models/Weapon/weapon.h"
// move_interactor
#include "interactors/MoveInteractor/move-interactor.h"
#include "interactors/UpdateGameStateInteractor/update-game-state-interactor.h"
// controllers
#include "controllers/PhysicsTickController/physics-tick-controller.h"

#include "doctest.h"

namespace doctest {
using namespace invasion::game_models;
using namespace invasion::interactors;
using namespace invasion::controllers;

using namespace request_models;
using namespace response_models;
using namespace std;



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
}



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
