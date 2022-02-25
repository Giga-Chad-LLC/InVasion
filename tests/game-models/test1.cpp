#include <iostream>
#include <vector>

#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-models/GameWorldManager/game-world-manager.h"

#include "interactors/MoveInteractor/move-interactor.h"

#include "doctest.h"


namespace doctest {


TEST_CASE("creating physics") {
	using namespace invasion::game_models;
	using namespace invasion::interactors;
	using namespace std;

	int totalSimulationTime = 40;
    int currentTime = 0;
    int dt = 1;

	[[maybe_unused]] const double g = -9.81;
	[[maybe_unused]] const double nu = 0.9;

	MoveInteractor interactor;
	GameWorldManager manager;
	MoveRequestModel req(MoveRequestModel::MoveEvent::StartMovingUp);
	
	vector<Player> players = { Player(Vector2D::ZERO) };

	// start moving
	interactor.execute(req, players[0]);


	while(currentTime < totalSimulationTime) {
		cout << "time: " << currentTime << " velocity: " << players[0].getVelocity() << endl;

		// stop event
		if(currentTime == 10) {
			std::cout << "start stopping..." << std::endl;
			req.setEvent(MoveRequestModel::MoveEvent::StopMovingUp);
			interactor.execute(req, players[0]);
		}

		manager.updatePlayersPositions(players, dt);

		// std::cout << player.getForce() << " " << player.getAcceleration() << endl;

		currentTime += dt;
	}

	cout << "time: " << currentTime << " velocity: " << players[0].getVelocity() << endl;
}

// std::cout << "time: " << currentTime << " velocity: " << player.getVelocity() << std::endl;
// std::cout << "time: " << currentTime << " pos: " << player.getPosition() << std::endl;
// cout << endl;


// std::cout << "time: " << currentTime << " velocity: " << player.getVelocity() << std::endl;
// std::cout << "time: " << currentTime << " pos: " << player.getPosition() << std::endl;
// cout << endl;



// // forces
// const Vector2D movingForce = Vector2D(500, 500);
// const Vector2D frictionForce = -nu * movingForce.normalize() * Vector2D(0, Player::MASS * g).magnitude();

// Vector2D totalForce = movingForce + frictionForce;

// if(frictionForce.magnitude() >= movingForce.magnitude())
// 	totalForce = Vector2D::ZERO;

// cout << "total force: " << totalForce << endl;
// cout << "moving force len: " << movingForce.magnitude() << endl;
// cout << "friction force len: " << frictionForce.magnitude() << endl;

// player.setForce(totalForce);


} // namespace doctest
