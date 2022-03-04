#include <vector>
#include <cassert>

#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-session.h"


namespace invasion::game_models {
	
int GameSession::addPlayer() {
	auto& players = m_storage.getPlayers();
	const int playerId = static_cast<int>(players.size());

	players.emplace_back(Vector2D::ZERO, playerId);
	return playerId;
}

Player& GameSession::getPlayer(const int playerId) {
	auto& players = m_storage.getPlayers();
	Player* player_ptr = nullptr;

	for(Player& player : players) {
		if(playerId == player.getId()) {
			player_ptr = &player;
			break;
		}
	}
	assert(player_ptr);
	return *player_ptr;
}

void GameSession::updateGameState() {
	// trying update players/bullets positions: manager.tryUpdatePosition(...): TODO
	// checking collisions: TODO
	// deleting objects (killing players, deleting bullets) if needed: TODO
	// making positions update: manager.updatePlayersPositions(...) && manager.updateBulletsPositions(...): TODO

	auto& players = m_storage.getPlayers();
	m_manager.updatePlayersPositions(players, 1.0);
}


} // namespace invasion::game_models
