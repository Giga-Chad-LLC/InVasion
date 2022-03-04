#ifndef GAME_WORLD_STORAGE_H_
#define GAME_WORLD_STORAGE_H_

#include <vector>

#include "game-models/Player/player.h"

namespace invasion::game_models {
	

struct GameWorldStorage {
	
	std::vector<Player>& getPlayers();

private:
	std::vector<Player> m_players;
};


} // namespace invasion::game_models



#endif // GAME_WORLD_STORAGE_H_