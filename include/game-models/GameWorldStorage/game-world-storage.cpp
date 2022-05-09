#include <vector>

// game-models
#include "game-models/Player/player.h"
#include "game-models/Bullet/bullet.h"
#include "game-models/StaticObject/static-object.h"

#include "game-world-storage.h"


namespace invasion::game_models {

std::vector<std::shared_ptr<Player>>& GameWorldStorage::getPlayers() {
	return m_players;
}

std::vector<std::shared_ptr<Player>>& GameWorldStorage::getDamagedPlayers() {
	return m_damagedPlayers;
}

std::vector<std::shared_ptr<Player>>& GameWorldStorage::getKilledPlayers() {
	return m_killedPlayers;
}

std::vector<std::shared_ptr<Bullet>>& GameWorldStorage::getBullets() {
	return m_bullets;
}

std::vector<std::shared_ptr<StaticObject>>& GameWorldStorage::getObstacles() {
	return m_obstacles;
}

} // namespace invasion::game_models
