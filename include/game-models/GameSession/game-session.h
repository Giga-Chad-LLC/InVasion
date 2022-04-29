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
	

class GameSession {
public:
	explicit GameSession();

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
	long long m_lastGameStateUpdate_ms;
	int m_firstTeamPlayersCount;
	int m_secondTeamPlayersCount;
	int m_nextBulletId;
	int m_nextPlayerId;
	GameWorldStorage m_storage;
	GameWorldManager m_manager;
};


} // namespace invasion::game_models


#endif // GAME_SESSION_H_