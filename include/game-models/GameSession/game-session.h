#ifndef GAME_SESSION_H_
#define GAME_SESSION_H_

#include <chrono>
#include <vector>
#include <memory>

// game-models
#include "game-models/GameWorldStorage/game-world-storage.h"
#include "game-models/PlayerManager/player-manager.h"
#include "game-models/BulletManager/bullet-manager.h"
#include "game-models/Player/player.h"
#include "game-models/Bullet/bullet.h"

#include "game-session-stats.h"


namespace invasion::game_models {
	

class GameSession {
public:
	explicit GameSession();

	int createPlayerAndReturnId();
	int createIdForNewBullet();
	
	int addBullet(std::shared_ptr<Bullet> bullet);

	std::shared_ptr<Player> getPlayer(int playerId);
	std::shared_ptr<Bullet> getBullet(int bulletId);

	std::vector<std::shared_ptr<Player>>& getPlayers();
	std::vector<std::shared_ptr<Player>>& getDamagedPlayers();
	std::vector<std::shared_ptr<Player>>& getKilledPlayers();
	std::vector<std::shared_ptr<Bullet>>& getBullets();

	void removePlayerById(int playerId);

	void updateGameState();

	const GameSessionStats& getGameStatistics();


private:
	long long m_lastGameStateUpdate_ms;
	int m_nextBulletId;
	int m_nextPlayerId;
	GameSessionStats m_gameStatistics;
	GameWorldStorage m_storage;
	PlayerManager m_playerManager;
	BulletManager m_bulletManager;
};


} // namespace invasion::game_models


#endif // GAME_SESSION_H_