#ifndef GAME_WORLD_STORAGE_H_
#define GAME_WORLD_STORAGE_H_

#include <vector>
#include <memory>

// game-models
#include "game-models/Player/player.h"
#include "game-models/Bullet/bullet.h"
#include "game-models/StaticObject/static-object.h"
#include "game-models/StaticSupply/static-supply.h"
#include "game-models/Vector2D/vector2d.h"

namespace invasion::game_models {
	

class GameWorldStorage {
public:
	std::vector<std::shared_ptr<Player>>& getPlayers();
	std::vector<std::shared_ptr<Player>>& getDamagedPlayers();
	std::vector<std::shared_ptr<Player>>& getKilledPlayers();
	std::vector<std::shared_ptr<Bullet>>& getBullets();
	std::vector<std::shared_ptr<StaticObject>>& getObstacles();
	std::vector<std::shared_ptr<StaticSupply>>& getSupplies();
	std::vector<Vector2D>& getFirstTeamRespawnPoints();
	std::vector<Vector2D>& getSecondTeamRespawnPoints();

private:
	std::vector<std::shared_ptr<Player>> m_players;
	std::vector<std::shared_ptr<Player>> m_damagedPlayers;
	std::vector<std::shared_ptr<Player>> m_killedPlayers;
	std::vector<std::shared_ptr<Bullet>> m_bullets;
	std::vector<std::shared_ptr<StaticObject>> m_obstacles;
	std::vector<std::shared_ptr<StaticSupply>> m_supplies;
	std::vector<Vector2D> m_firstTeamRespawnPoints;
	std::vector<Vector2D> m_secondTeamRespawnPoints;
};


} // namespace invasion::game_models



#endif // GAME_WORLD_STORAGE_H_