#include <vector>

#include "game-models/Player/player.h"
#include "game-world-storage.h"



namespace invasion::game_models {

std::vector<std::shared_ptr<Player>>& GameWorldStorage::getPlayers() {
	return m_players;
}

std::vector<std::shared_ptr<Bullet>>& GameWorldStorage::getBullets() {
	return m_bullets;
}

std::vector<std::shared_ptr<Player>>& GameWorldStorage::getDamagedPlayers() {
	return m_damagedPlayers;
}

} // namespace invasion::game_models
