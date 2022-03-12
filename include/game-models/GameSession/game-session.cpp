#include <vector>
#include <cassert>
#include <utility>

#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-session.h"


namespace invasion::game_models {

// returns id of created player
int GameSession::addPlayer() {
	auto& players = m_storage.getPlayers();
	const int playerId = static_cast<int>(players.size());

	players.emplace_back(Vector2D::ZERO, playerId);
	return playerId;
}


int GameSession::addBullet(Bullet bullet) {
	const int id = bullet.getId();
	m_storage.getBullets().push_back(std::move(bullet));	
	return id;
}

int GameSession::createIdForNewBullet() {
	return static_cast<int>(m_storage.getBullets().size());
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
	assert(player_ptr != nullptr);
	return *player_ptr;
}


Bullet& GameSession::getBullet(int bulletId) {
	auto& bullets = m_storage.getBullets();
	Bullet* bullet_ptr = nullptr;

	for(Bullet& bullet : bullets) {
		if(bulletId == bullet.getId()) {
			bullet_ptr = &bullet;
			break;
		}
	}

	assert(bullet_ptr != nullptr);
	return *bullet_ptr;
}


void GameSession::updateGameState() {
	// trying update players/bullets positions: manager.tryUpdatePosition(...): TODO
	// checking collisions: TODO
	// deleting objects (killing players, deleting bullets) if needed: TODO
	// making positions update: manager.updatePlayersPositions(...) && manager.updateBulletsPositions(...): TODO

	long long dt = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now().time_since_epoch()
	).count() - lastGameStateUpdate_ms;

	auto& players = m_storage.getPlayers();
	m_manager.updatePlayersPositions(players, dt / 1000.0);
}


} // namespace invasion::game_models
