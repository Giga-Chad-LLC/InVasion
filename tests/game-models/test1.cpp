#include <iostream>
#include <vector>

#include "game-models/Vector2D/vector2d.h"
#include "game-models//Player/player.h"
#include "game-models/GameWorldManager/game-world-manager.h"

#include "doctest.h"


namespace doctest {


TEST_CASE("creating physics") {
	using namespace invasion::game_models;
	using namespace std;

	int totalSimulationTime = 40;
    int currentTime = 0;
    int dt = 1;

	const double g = -9.81;
	const double nu = 0.9; // coefficient of friction

	Player player(Vector2D::ZERO);

	// forces
	const Vector2D movingForce = Vector2D(500, 500);
	const Vector2D frictionForce = -nu * movingForce.normalize() * Vector2D(0, Player::MASS * g).magnitude();

	Vector2D totalForce = movingForce + frictionForce;
	
	if(frictionForce.magnitude() >= movingForce.magnitude())
		totalForce = Vector2D::ZERO;
	
	cout << "total force: " << totalForce << endl;
	cout << "moving force len: " << movingForce.magnitude() << endl;
	cout << "friction force len: " << frictionForce.magnitude() << endl;

	player.setForce(totalForce);

	while(currentTime < totalSimulationTime) {
		std::cout << "time: " << currentTime << " velocity: " << player.getVelocity() << std::endl;
		// std::cout << "time: " << currentTime << " pos: " << player.getPosition() << std::endl;
		// cout << endl;
		player.makeMove(dt);
		
		if(currentTime == 5)
			player.setForce(Vector2D::ZERO);
		currentTime += dt;
	}

	std::cout << "time: " << currentTime << " velocity: " << player.getVelocity() << std::endl;
	// std::cout << "time: " << currentTime << " pos: " << player.getPosition() << std::endl;
	// cout << endl;
}


} // namespace doctest
