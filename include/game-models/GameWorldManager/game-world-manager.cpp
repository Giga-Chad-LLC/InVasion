#include <iostream>
#include <cmath>

#include "game-world-manager.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"

namespace invasion::game_models {
	
void GameWorldManager::updatePlayersPositions(std::vector<Player>& players, double dt) const {

	for(Player& player : players) {

		this->applyFrictionAndSetResultForceOnPlayer(player, dt);
		// applying friction force
		/*
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
		*/


		// checking collisions with other players
		{
			Vector2D curPosition = player.getPosition();

			// imitating player's move
			Vector2D nextPosition = player.intentMove(dt);
			player.setPosition(nextPosition);

			bool inCollision = false;

			for(Player& other : players) {
				if(player.getId() == other.getId()) continue;

				inCollision = player.collidesWith(&other);
				if(inCollision) {
					break;
				}
			}

			player.setPosition(curPosition);

			if(inCollision) {
				player.setResultForce(Vector2D::ZERO);
				player.setVelocity(Vector2D::ZERO);
				// TODO: make binary search to find nearest position where objects do not collide and set player to that position
			}
		}

		player.makeMove(dt);
	}
}


void GameWorldManager::applyFrictionAndSetResultForceOnPlayer(Player& player, const double dt) const {
	const double g = -9.81;
	const double nu = 0.9;
	const double frictionCoef = -nu * Player::MASS * std::abs(g);
	const double appliedForceMagnitude = 31'0000;


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


} // namespace invasion::game_models
