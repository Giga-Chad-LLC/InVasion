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
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Bullet/bullet.h"
#include "game-models/StaticObject/static-object.h"
#include "game-models/StaticSupply/static-supply.h"

#include "game-session-stats.h"
#include "game-match-result-enum.h"


namespace invasion::game_models {
	

class GameSession {
public:
	explicit GameSession();

	int createPlayerAndReturnId(PlayerSpecialization specialization);
	int createIdForNewBullet();
	int createIdForNewSupply();
	
	int addBullet(std::shared_ptr<Bullet> bullet);

	std::shared_ptr<Player> getPlayer(int playerId);
	std::shared_ptr<Bullet> getBullet(int bulletId);
	std::shared_ptr<StaticSupply> getSupply(int supplyId);

	std::vector<std::shared_ptr<Player>>& getPlayers();
	std::vector<std::shared_ptr<Player>>& getDamagedPlayers();
	std::vector<std::shared_ptr<Player>>& getKilledPlayers();
	std::vector<std::shared_ptr<Bullet>>& getBullets();
	std::vector<std::shared_ptr<StaticObject>>& getObstacles();
	std::vector<std::shared_ptr<StaticSupply>>& getSupplies();

	void removePlayerById(int playerId);
	void changePlayerSpecialization(int playerId, PlayerSpecialization specialization);
	
	void updateGameState();

	bool playerExists(int playerId);
	const GameSessionStats& getGameStatistics();

	GameMatchResult getMatchResult() const;

	Vector2D getRespawnPoint(PlayerTeamId teamId);


	inline static const double MAX_INTERACTIVE_DISTANCE = 20.0;
private:
	void loadCollisionAssets();
	void loadRespawnPoints(const std::string& filepath, std::vector<Vector2D>& respawnPoints);

private:
	inline static const std::string COLLISION_ASSETS_DIRECTORY = "config/game-assets/collision-assets";
	inline static const std::string HUMANS_RESPAWN_POINTS_DIRECTORY = "config/game-assets/humans-respawn-points";
	inline static const std::string ALIENS_RESPAWN_POINTS_DIRECTORY = "config/game-assets/aliens-respawn-points";

	long long m_lastGameStateUpdate_ms;
	int m_nextBulletId;
	int m_nextPlayerId;
	int m_nextSupplyId;
	GameSessionStats m_gameStatistics;
	GameWorldStorage m_storage;
	PlayerManager m_playerManager;
	BulletManager m_bulletManager;
};


} // namespace invasion::game_models


#endif // GAME_SESSION_H_