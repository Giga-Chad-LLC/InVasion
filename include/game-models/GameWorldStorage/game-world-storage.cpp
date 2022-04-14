#include <vector>

#include "game-models/Player/player.h"
#include "game-world-storage.h"



namespace invasion::game_models {

std::vector<Player>& GameWorldStorage::getPlayers() {
	return m_players;
}

std::vector<std::shared_ptr<Bullet>>& GameWorldStorage::getBullets() {
	return m_bullets;
}

} // namespace invasion::game_models
