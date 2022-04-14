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

	Player& getPlayer(int playerId);
	std::shared_ptr<Bullet> getBullet(int bulletId);

	std::vector<Player>& getPlayers();
<<<<<<< HEAD
	std::vector<std::shared_ptr<Bullet>>& getBullets();
=======
>>>>>>> d8e9207e49fb07c02e80a07b0223a7ecb26b2c04

	void updateGameState();


private:
	static long long getCurrentTime_ms();

	long long lastGameStateUpdate_ms;
	GameWorldStorage m_storage;
	GameWorldManager m_manager;
};


} // namespace invasion::game_models


#endif // GAME_SESSION_H_