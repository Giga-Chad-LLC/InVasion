#include <iostream>
#include <cmath>

#include "game-world-manager.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"

namespace invasion::game_models {
	
void GameWorldManager::updatePlayersPositions(std::vector<Player>& players, double dt) const {

	for(Player& player : players) {

		this->applyFrictionAndSetResultForceOnPlayer(player, dt);
		this->updateResultForceAndVelocityOfPlayerOnCollisionsWithOtherPlayers(players, player, dt);
		
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



void GameWorldManager::updateResultForceAndVelocityOfPlayerOnCollisionsWithOtherPlayers(
	std::vector<Player>& players, 
	Player& consideredPlayer, 
	const double dt
) const {
	Vector2D curPosition = consideredPlayer.getPosition();

	// imitating player's intented move
	Vector2D nextPosition = consideredPlayer.intentMove(dt);
	consideredPlayer.setPosition(nextPosition);

	bool inCollision = false;

	for(Player& other : players) {
		if(consideredPlayer.getId() == other.getId()) continue;

		inCollision = consideredPlayer.collidesWith(&other);
		if(inCollision) {
			break;
		}
	}

	consideredPlayer.setPosition(curPosition);

	if(inCollision) {
		// TODO: make logic to find nearest position where objects do not collide and move player to that position
		consideredPlayer.setResultForce(Vector2D::ZERO);
		consideredPlayer.setVelocity(Vector2D::ZERO);
	}
}


} // namespace invasion::game_models
