#include <vector>
#include <memory>

#include "player-manager.h"
// game-models
#include "game-models/Player/player.h"
#include "game-models/StaticObject/static-object.h"
#include "game-models/Player/player-game-session-stats.h"
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {

void PlayerManager::updatePlayersPositions(
	std::vector<std::shared_ptr<Player>>& players,
	std::vector<std::shared_ptr<StaticObject>>& obstacles,
	double dt) const {
	
	for (auto& player_ptr : players) {
		const bool killed = player_ptr->getLifeState().isInDeadState();
		const bool active = player_ptr->getLifeState().isInActiveState();

		if(!killed && active) {
			this->applyFrictionAndSetResultForceOnPlayer(player_ptr, dt);
			this->updatePlayerPhysicsOnPlayerCollision(players, player_ptr, dt);
			this->updatePlayerPhysicsOnObstacleCollision(obstacles, player_ptr, dt);
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
		const bool active = player_ptr->getLifeState().isInActiveState();

		if (damaged && !killed && active) {
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
		const bool active = player_ptr->getLifeState().isInActiveState();

		if(damaged && killed && active) {
			killedPlayers.push_back(player_ptr);
			lifeState.removeDamagedState();
		}
	}
}


void PlayerManager::updatePlayersGameSessionStats(std::vector<std::shared_ptr<Player>>& players,
								  				 std::vector<std::shared_ptr<Player>>& killedPlayers) const {
	
	for (const auto& killedPlayer : killedPlayers) {
		const int killedPlayerId = killedPlayer->getId();
		const int killedBy = killedPlayer->getLifeState().killedBy();

		for (auto& player : players) {
			const bool active = player->getLifeState().isInActiveState();
			
			if(active) {
				const int playerId = player->getId();
				PlayerGameSessionStats& stats = player->getGameSessionStats();

				if (playerId == killedPlayerId) {
					stats.incrementDeaths();
				}

				if (playerId == killedBy) {
					stats.incrementKills();
				}
			}

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
	std::shared_ptr<Player> consideredPlayer,
	double dt) const {
	
	Vector2D curPosition = consideredPlayer->getPosition();

	// imitating player's intented move
	Vector2D nextPosition = consideredPlayer->intentMove(dt);
	consideredPlayer->setPosition(nextPosition);

	std::shared_ptr<Player> collidedPlayer;
	bool inCollision = false;

	for(const auto other : players) {
		// if other player is dead/inactive or he is considered player
		const bool dead = other->getLifeState().isInDeadState();
		const bool active = other->getLifeState().isInActiveState();

		if(!active || dead || consideredPlayer->getId() == other->getId()) {
			continue;
		}

		inCollision = consideredPlayer->collidesWithShape(other.get());
		if(inCollision) {
			collidedPlayer = other;
			break;
		}
	}

	consideredPlayer->setPosition(curPosition);

	if(inCollision) {
		const bool penetrationOccurred = consideredPlayer->collidesWithShape(collidedPlayer.get());
		
		// prevent moving forward only if objects do not penetrate
		// otherwise let the object leave penetrating zone
		if(!penetrationOccurred) {
			consideredPlayer->setResultForce(Vector2D::ZERO);
			consideredPlayer->setVelocity(Vector2D::ZERO);
		}

	}
}


void PlayerManager::updatePlayerPhysicsOnObstacleCollision(
	std::vector<std::shared_ptr<StaticObject>>& obstacles,
	std::shared_ptr<Player> consideredPlayer,
	double dt) const {
	
	Vector2D curPosition = consideredPlayer->getPosition();

	// imitating player's intented move
	Vector2D nextPosition = consideredPlayer->intentMove(dt);
	consideredPlayer->setPosition(nextPosition);

	std::shared_ptr<StaticObject> collidedObject;
	bool inCollision = false;

	for(const auto obstacle : obstacles) {
		inCollision = consideredPlayer->collidesWithShape(obstacle.get());
		if(inCollision) {
			collidedObject = obstacle;
			break;
		}
	}

	consideredPlayer->setPosition(curPosition);

	if(inCollision) {
		const bool penetrationOccurred = consideredPlayer->collidesWithShape(collidedObject.get());

		// prevent moving forward only if objects do not penetrate
		// otherwise let the object leave penetrating zone
		if(!penetrationOccurred) {
			consideredPlayer->setResultForce(Vector2D::ZERO);
			consideredPlayer->setVelocity(Vector2D::ZERO);
		}
	
	}
}


} // namespace invasion::game_models
