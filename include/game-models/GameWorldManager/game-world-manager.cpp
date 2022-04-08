#include <iostream>
#include <cmath>

#include "game-world-manager.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"

namespace invasion::game_models {
	
void GameWorldManager::updatePlayersPositions(std::vector<Player>& players, double dt) const {
	const double g = -9.81;
	const double nu = 0.9;
	const double frictionCoef = -nu * Player::MASS * std::abs(g);
	const double appliedForceMagnitude = 31'0000;


	for(Player& player : players) {
		// applying friction force
		if(player.isMoving()) {
			const Vector2D appliedForce = Vector2D::clampMagnitude(player.getMovingForce(), appliedForceMagnitude);
			const Vector2D frictionForce = 500.0 * frictionCoef * appliedForce.normalize();

			//std::cout << "appliedForce magnitude: " << appliedForce.magnitude() << std::endl;
			//std::cout << "frictionForce magnitude: " << frictionForce.magnitude() << std::endl;

			Vector2D resultForce = Vector2D::ZERO;
		
			if(frictionForce.magnitude() < appliedForce.magnitude()) {
				resultForce = appliedForce + frictionForce;
			}

			player.setResultForce(resultForce);

			//std::cout << "resultForce: " << resultForce << std::endl;			
		}
		else {
			// applying friction until player stops
			const Vector2D velocity = player.getVelocity();
			const Vector2D frictionForce = 70.0 * frictionCoef * velocity.normalize();
			const Vector2D acceleration = frictionForce / Player::MASS;

			const Vector2D integratedAcceleration = acceleration * dt;
			// std::cout << "frictionForce magnitude: " << frictionForce.magnitude() << std::endl;

			if(integratedAcceleration.magnitude() >= velocity.magnitude()) {
				player.setResultForce(Vector2D::ZERO);
				player.setVelocity(Vector2D::ZERO);
			}
			else {
				player.setResultForce(frictionForce);
			}
		}

		//std::cout << "velocity: " << player.getVelocity() << std::endl;
		player.makeMove(dt);
		//std::cout << "speed magnitude: " << player.getVelocity().magnitude() << std::endl;
	}
}


Vector2D GameWorldManager::tryUpdatePosition(KinematicObject* object, double dt) const {
	return object->intentMove(dt);
}


} // namespace invasion::game_models
