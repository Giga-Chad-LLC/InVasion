#include <algorithm>
#include "bullet-manager.h"

// game-models
#include "game-models/Bullet/bullet.h"
#include "game-models/Player/player.h"
#include "game-models/StaticObject/static-object.h"
#include "game-models/Player/player-life-state.h"


namespace invasion::game_models {
	
void BulletManager::updateBulletsPositions(std::vector<std::shared_ptr<Bullet>>& bullets, 
										   const std::vector<std::shared_ptr<Player>>& players,
										   const std::vector<std::shared_ptr<StaticObject>>& obstacles,
										   double dt) const {
	const double appliedForceMagnitude = 500;

	for (const auto& bullet : bullets) {
		// TODO: do not update result force on every update request because the result force is never changing
		const Vector2D resultForce = Vector2D::clampMagnitude(bullet->getMovingForce(), appliedForceMagnitude);
		bullet->setResultForce(resultForce);

		const Vector2D curPosition = bullet->getPosition();
		const Vector2D nextPosition = bullet->intentMove(dt);

		bullet->setPosition(nextPosition);

		// colliding with obstacles
		this->collideBulletWithObstacles(bullet, obstacles);

		// colliding with players
		if(bullet->isInCrushedState() == false) {
			this->collideBulletWithPlayers(bullet, players);
		}

		// returning to previous position
		bullet->setPosition(curPosition);

		if(bullet->isInCrushedState() == false) {
			bullet->makeMove(dt);
		}
	}
}


void BulletManager::removeCrushedAndFlewOutOfBoundsBullets(std::vector<std::shared_ptr<Bullet>>& bullets) const {
	bullets.erase(
		std::remove_if(std::begin(bullets), std::end(bullets), [](const std::shared_ptr<Bullet>& bullet_ptr) {
			const Vector2D pos = bullet_ptr->getPosition();
			bool bulletOutOfMapBounds = std::abs(pos.getX()) > 1000 || std::abs(pos.getY()) > 1000;
			return bullet_ptr->isInCrushedState() || bulletOutOfMapBounds;
		}),
		std::end(bullets)
	);
}


void BulletManager::collideBulletWithObstacles(std::shared_ptr<Bullet> bullet,
											   const std::vector<std::shared_ptr<StaticObject>>& obstacles) const {
	for(const auto& obstacle : obstacles) {
		if(bullet->collidesWithShape(obstacle.get())) {
			bullet->setCrushedState(true);
			break;
		}
	}
}


void BulletManager::collideBulletWithPlayers(std::shared_ptr<Bullet> bullet,
											 const std::vector<std::shared_ptr<Player>>& players) const {
	std::shared_ptr<Player> collidedPlayer = nullptr;

	const int playerId = bullet->getPlayerId();
	const PlayerTeamId teamId = bullet->getPlayerTeamId();
	const double damage = bullet->getDamage();

	// searching for collided player
	for (const auto& player : players) {
		if (
			player->getLifeState().isInDeadState() == false &&
			player->getId() != playerId &&
			player->getTeamId() != teamId &&
			player->collidesWithHitbox(bullet.get(), Player::HITBOX_POSITION_OFFSET)
		) {
			collidedPlayer = player;
			break;
		}
	}

	if (collidedPlayer) {
		PlayerLifeState& lifeState = collidedPlayer->getLifeState();
		lifeState.applyDamage(damage, playerId);
		
		bullet->setCrushedState(true);

		std::cout << "bullet " << bullet->getId() << " damaged player " << collidedPlayer->getId();
		std::cout << " HP: " << lifeState.getHitPoints() << "\n";
	}
}


} // namespace invasion::game_models

