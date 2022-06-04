#include <vector>

#include "game-world-storage.h"

// game-models
#include "game-models/Player/player.h"
#include "game-models/Bullet/bullet.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/StaticObject/static-object.h"


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

std::vector<std::shared_ptr<StaticSupply>>& GameWorldStorage::getSupplies() {
	return m_supplies;
}

std::vector<Vector2D>& GameWorldStorage::getFirstTeamRespawnPoints() {
	return m_firstTeamRespawnPoints;
}



std::vector<Vector2D>& GameWorldStorage::getSecondTeamRespawnPoints() {
	return m_secondTeamRespawnPoints;
}


} // namespace invasion::game_models
