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
		{
			Vector2D resultForce = Vector2D::ZERO;
			if(player.isMoving()) {
				const Vector2D appliedForce = Vector2D::clampMagnitude(player.getMovingForce(), appliedForceMagnitude);
				const Vector2D frictionForce = 500.0 * frictionCoef * appliedForce.normalize();

				if(frictionForce.magnitude() < appliedForce.magnitude()) {
					resultForce = appliedForce + frictionForce;
				}
			}
			else {
				// applying friction until player stops
				const Vector2D velocity = player.getVelocity();
				const Vector2D frictionForce = 70.0 * frictionCoef * velocity.normalize();

				const Vector2D acceleration = frictionForce / Player::MASS;
				const Vector2D integratedAcceleration = acceleration * dt;

				if(integratedAcceleration.magnitude() >= velocity.magnitude()) {
					player.setVelocity(Vector2D::ZERO);
				}
				else {
					resultForce = frictionForce;
				}
			}
			player.setResultForce(resultForce);
		}

		

		player.makeMove(dt);
	}
}


Vector2D GameWorldManager::tryUpdatePosition(KinematicObject* object, double dt) const {
	return object->intentMove(dt);
}


} // namespace invasion::game_models
