#ifndef GAME_SESSION_H_
#define GAME_SESSION_H_

#include "game-models/GameWorldStorage/game-world-storage.h"
#include "game-models/GameWorldManager/game-world-manager.h"
#include "game-models/Player/player.h"

namespace invasion::game_models {
	

struct GameSession {
	int addPlayer();
	Player& getPlayer(int playerId);
	void updateGameState();

private:
	GameWorldStorage m_storage;
	GameWorldManager m_manager;
};


} // namespace invasion::game_models


#endif // GAME_SESSION_H_