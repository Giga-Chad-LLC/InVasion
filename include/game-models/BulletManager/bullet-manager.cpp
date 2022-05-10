#include <algorithm>
#include "bullet-manager.h"

// game-models
#include "game-models/Bullet/bullet.h"
#include "game-models/Player/player.h"
#include "game-models/StaticObject/static-object.h"
#include "game-models/Player/player-life-state.h"


namespace invasion::game_models {
	
void BulletManager::updateBulletsPositions(std::vector<std::shared_ptr<Bullet>>& bullets, 
										   std::vector<std::shared_ptr<Player>>& players,
										   std::vector<std::shared_ptr<StaticObject>>& obstacles,
										   double dt) const {
	const double appliedForceMagnitude = 500;

	for (const auto& bullet_ptr : bullets) {
		// TODO: do not update result force on every update request because the result force is never changing
		const Vector2D resultForce = Vector2D::clampMagnitude(bullet_ptr->getMovingForce(), appliedForceMagnitude);
		bullet_ptr->setResultForce(resultForce);

		const Vector2D curPosition = bullet_ptr->getPosition();
		const Vector2D nextPosition = bullet_ptr->intentMove(dt);

		bullet_ptr->setPosition(nextPosition);

		// searching for collided obstacle
		for(const auto& obstacle : obstacles) {
			if(bullet_ptr->collidesWithShape(obstacle.get())) {
				bullet_ptr->setCrushedState(true);
				break;
			}
		}

		// if not crushed
		if(bullet_ptr->isInCrushedState() == false) {
			std::shared_ptr<Player> collidedPlayer_ptr = nullptr;
			const int playerId = bullet_ptr->getPlayerId();
			const PlayerTeamId teamId = bullet_ptr->getPlayerTeamId();
			const double damage = bullet_ptr->getDamage();

			// searching for collided player
			for (const auto& player_ptr : players) {
				if (player_ptr->getLifeState().isInDeadState() == false &&
					player_ptr->getId() != playerId &&
					player_ptr->getTeamId() != teamId &&
					player_ptr->collidesWithHitbox(bullet_ptr.get(), Player::HITBOX_POSITION_OFFSET)) {
					collidedPlayer_ptr = player_ptr;
					break;
				}
			}

			if (collidedPlayer_ptr != nullptr) {
				PlayerLifeState& lifeState = collidedPlayer_ptr->getLifeState();
				lifeState.applyDamage(damage, playerId);
				bullet_ptr->setCrushedState(true);

				std::cout << "bullet " << bullet_ptr->getId() << " damaged player " << collidedPlayer_ptr->getId();
				std::cout << " HP: " << lifeState.getHitPoints() << "\n";
			}
		}

		// returning to previous position
		bullet_ptr->setPosition(curPosition);

		// if not crushed
		if(bullet_ptr->isInCrushedState() == false) {
			bullet_ptr->makeMove(dt);
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

} // namespace invasion::game_models

