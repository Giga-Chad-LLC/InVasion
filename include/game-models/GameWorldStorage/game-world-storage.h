#ifndef GAME_WORLD_STORAGE_H_
#define GAME_WORLD_STORAGE_H_

#include <vector>
#include <memory>

#include "game-models/Player/player.h"
#include "game-models/Bullet/bullet.h"

namespace invasion::game_models {
	

class GameWorldStorage {
public:
	std::vector<std::shared_ptr<Player>>& getPlayers();
	std::vector<std::shared_ptr<Player>>& getDamagedPlayers();
	std::vector<std::shared_ptr<Bullet>>& getBullets();

private:
	std::vector<std::shared_ptr<Player>> m_players;
	std::vector<std::shared_ptr<Player>> m_damagedPlayers;
	std::vector<std::shared_ptr<Bullet>> m_bullets;
};


} // namespace invasion::game_models



#endif // GAME_WORLD_STORAGE_H_