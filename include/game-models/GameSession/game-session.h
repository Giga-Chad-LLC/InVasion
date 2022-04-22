#ifndef GAME_SESSION_H_
#define GAME_SESSION_H_

#include <chrono>
#include <vector>
#include <memory>

#include "game-models/GameWorldStorage/game-world-storage.h"
#include "game-models/GameWorldManager/game-world-manager.h"
#include "game-models/Player/player.h"
#include "game-models/Bullet/bullet.h"

namespace invasion::game_models {
	

struct GameSession {
	GameSession();

	int createPlayerAndReturnId();
	
	int addBullet(std::shared_ptr<Bullet> bullet);
	int createIdForNewBullet();

	std::shared_ptr<Player> getPlayer(int playerId);
	std::shared_ptr<Bullet> getBullet(int bulletId);

	std::vector<std::shared_ptr<Player>>& getPlayers();
	std::vector<std::shared_ptr<Player>>& getDamagedPlayers();
	std::vector<std::shared_ptr<Bullet>>& getBullets();

	void updateGameState();


private:
	static long long getCurrentTime_ms();

	long long lastGameStateUpdate_ms;
	GameWorldStorage m_storage;
	GameWorldManager m_manager;
};


} // namespace invasion::game_models


#endif // GAME_SESSION_H_