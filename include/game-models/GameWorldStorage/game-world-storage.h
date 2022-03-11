#ifndef GAME_WORLD_STORAGE_H_
#define GAME_WORLD_STORAGE_H_

#include <vector>

#include "game-models/Player/player.h"
#include "game-models/Bullet/bullet.h"

namespace invasion::game_models {
	

struct GameWorldStorage {
	
	std::vector<Player>& getPlayers();
	std::vector<Bullet>& getBullets();

private:
	std::vector<Player> m_players;
	std::vector<Bullet> m_bullets;
};


} // namespace invasion::game_models



#endif // GAME_WORLD_STORAGE_H_