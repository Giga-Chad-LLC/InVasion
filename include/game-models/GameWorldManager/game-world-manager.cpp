#include <iostream>
#include <cmath>
#include <memory>

#include "game-world-manager.h"
// game-models
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Player/player-life-state.h"


namespace invasion::game_models {
	
// public:
void GameWorldManager::updatePlayersPositions(std::vector<std::shared_ptr<Player>>& players, double dt) const {
	for(auto& player_ptr : players) {
		this->applyFrictionAndSetResultForceOnPlayer(player_ptr, dt);
		this->updateResultForceAndVelocityOfPlayerOnCollisionsWithOtherPlayers(players, player_ptr, dt);
		player_ptr->makeMove(dt);
	}
}


void GameWorldManager::updateBulletsPositions(std::vector<std::shared_ptr<Bullet>>& bullets, 
											  std::vector<std::shared_ptr<Player>>& players,
											  double dt) const {
	const double appliedForceMagnitude = 500;

	for(const auto& bullet_ptr : bullets) {
		// TODO: do not update result force on every update request because the result force is never changing
		const Vector2D resultForce = Vector2D::clampMagnitude(bullet_ptr->getMovingForce(), appliedForceMagnitude);
		bullet_ptr->setResultForce(resultForce);


		// checking collisions
		std::shared_ptr<Player> collidedPlayer_ptr = nullptr;

		const Vector2D curPosition = bullet_ptr->getPosition();
		const Vector2D nextPosition = bullet_ptr->intentMove(dt);

		bullet_ptr->setPosition(nextPosition);

		const int playerId = bullet_ptr->getPlayerId();
		const PlayerTeamId teamId = bullet_ptr->getPlayerTeamId();
		const double damage = bullet_ptr->getDamage();

		// searching for collided player
		for(const auto& player_ptr : players) {
			if(player_ptr->getId() != playerId &&
			   player_ptr->getTeamId() != teamId &&
			   player_ptr->collidesWithHitbox(bullet_ptr.get())) {
				collidedPlayer_ptr = player_ptr;
				break;
			}
		}

		bullet_ptr->setPosition(curPosition);

		if(collidedPlayer_ptr != nullptr) {
			std::cout << "bullet " << bullet_ptr->getId() << " damaged player " << collidedPlayer_ptr->getId();

			PlayerLifeState& lifeState = collidedPlayer_ptr->getLifeState();
			lifeState.applyDamage(damage, playerId);

			std::cout << " HP: " << lifeState.getHitPoints() << "\n";

			bullet_ptr->setCrushedState(true);
		}
		else {
			bullet_ptr->makeMove(dt);
		}

	}
}


void GameWorldManager::findDamagedPlayers(std::vector<std::shared_ptr<Player>>& players,
										  std::vector<std::shared_ptr<Player>>& damagedPlayers) const {
	for(auto& player_ptr : players) {
		if(player_ptr->getLifeState().isInDamagedState()) {
			damagedPlayers.push_back(player_ptr);
			player_ptr->getLifeState().removeDamagedState();
		}
	}
}


// private:
void GameWorldManager::applyFrictionAndSetResultForceOnPlayer(std::shared_ptr<Player> player_ptr, const double dt) const {
	const double g = -9.81;
	const double nu = 0.9;
	const double frictionCoef = -nu * Player::MASS * std::abs(g);
	const double appliedForceMagnitude = 31'0000;

	Vector2D resultForce = Vector2D::ZERO;
	
	if(player_ptr->isMoving()) {
		const Vector2D appliedForce = Vector2D::clampMagnitude(player_ptr->getMovingForce(), appliedForceMagnitude);
		const Vector2D frictionForce = 500.0 * frictionCoef * appliedForce.normalize();

		if(frictionForce.magnitude() < appliedForce.magnitude()) {
			resultForce = appliedForce + frictionForce;
		}
	}
	else {
		// applying friction until player stops
		const Vector2D velocity = player_ptr->getVelocity();
		const Vector2D frictionForce = 70.0 * frictionCoef * velocity.normalize();

		const Vector2D acceleration = frictionForce / Player::MASS;
		const Vector2D integratedAcceleration = acceleration * dt;

		if(integratedAcceleration.magnitude() >= velocity.magnitude()) {
			player_ptr->setVelocity(Vector2D::ZERO);
		}
		else {
			resultForce = frictionForce;
		}
	}
	
	player_ptr->setResultForce(resultForce);
}


void GameWorldManager::updateResultForceAndVelocityOfPlayerOnCollisionsWithOtherPlayers(
	std::vector<std::shared_ptr<Player>>& players,
	std::shared_ptr<Player> consideredPlayer_ptr,
	const double dt) const {
	
	Vector2D curPosition = consideredPlayer_ptr->getPosition();

	// imitating player's intented move
	Vector2D nextPosition = consideredPlayer_ptr->intentMove(dt);
	consideredPlayer_ptr->setPosition(nextPosition);

	bool inCollision = false;

	for(const auto& other : players) {
		if(consideredPlayer_ptr->getId() == other->getId()) continue;

		inCollision = consideredPlayer_ptr->collidesWithShape(other.get());
		if(inCollision) {
			break;
		}
	}

	consideredPlayer_ptr->setPosition(curPosition);

	if(inCollision) {
		// TODO: make logic to find nearest position where objects do not collide and move player to that position
		consideredPlayer_ptr->setResultForce(Vector2D::ZERO);
		consideredPlayer_ptr->setVelocity(Vector2D::ZERO);
	}
}


} // namespace invasion::game_models
