#include <vector>
#include <memory>

#include "player-manager.h"
// game-models
#include "game-models/Player/player.h"
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {

void PlayerManager::updatePlayersPositions(std::vector<std::shared_ptr<Player>>& players, double dt) const {
	for (auto& player_ptr : players) {
		const bool killed = player_ptr->getLifeState().isInDeadState();
		if(!killed) {
			this->applyFrictionAndSetResultForceOnPlayer(player_ptr, dt);
			this->updatePlayerPhysicsOnPlayerCollision(players, player_ptr, dt);
			player_ptr->makeMove(dt);
		}
	}
}


void PlayerManager::findDamagedPlayers(std::vector<std::shared_ptr<Player>>& players,
									   std::vector<std::shared_ptr<Player>>& damagedPlayers) const {
	damagedPlayers.clear();

	for (auto& player_ptr : players) {
		PlayerLifeState& lifeState = player_ptr->getLifeState();
		
		const bool damaged = lifeState.isInDamagedState();
		const bool killed = lifeState.isInDeadState();

		if (damaged && !killed) {
			damagedPlayers.push_back(player_ptr);
			lifeState.removeDamagedState();
		}
	}
}


void PlayerManager::findKilledPlayers(std::vector<std::shared_ptr<Player>>& players,
									  std::vector<std::shared_ptr<Player>>& killedPlayers) const {
	killedPlayers.clear();

	for (auto& player_ptr : players) {
		PlayerLifeState& lifeState = player_ptr->getLifeState();

		const bool damaged = lifeState.isInDamagedState();
		const bool killed = lifeState.isInDeadState();

		if(damaged && killed) {
			killedPlayers.push_back(player_ptr);
			lifeState.removeDamagedState();
		}
	}
}


void PlayerManager::applyFrictionAndSetResultForceOnPlayer(std::shared_ptr<Player> player_ptr, double dt) const {
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


void PlayerManager::updatePlayerPhysicsOnPlayerCollision(
	std::vector<std::shared_ptr<Player>>& players,
	std::shared_ptr<Player> consideredPlayer_ptr,
	double dt) const {
	
	Vector2D curPosition = consideredPlayer_ptr->getPosition();

	// imitating player's intented move
	Vector2D nextPosition = consideredPlayer_ptr->intentMove(dt);
	consideredPlayer_ptr->setPosition(nextPosition);

	bool inCollision = false;

	for(const auto& other : players) {
		// if other player is dead or he is considered player
		if(other->getLifeState().isInDeadState() ||
		   consideredPlayer_ptr->getId() == other->getId()) {
			continue;
		}

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
