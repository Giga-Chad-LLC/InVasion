#include <vector>
#include <cassert>
#include <utility>
#include <random>
#include <chrono>
#include <ctime> 
#include <iostream>

#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-session.h"


namespace invasion::game_models {

GameSession::GameSession() {
	lastGameStateUpdate_ms = GameSession::getCurrentTime_ms();
}

// returns id of created player
int GameSession::createPlayerAndReturnId() {
	auto& players = m_storage.getPlayers();
	const int playerId = static_cast<int>(players.size());

	int firstTeamPlayersCount = 0;
	int secondTeamPlayersCount = 0;

	for(const auto& player : players) {
		const Player::TeamId teamId = player.getTeamId();

		if(teamId == Player::TeamId::FirstTeam) {
			firstTeamPlayersCount++;	
		}
		else {
			secondTeamPlayersCount++;	
		}
	}

	// assigning team to new player
	Player::TeamId teamId = Player::TeamId::SecondTeam;

	if(firstTeamPlayersCount < secondTeamPlayersCount) {
		teamId = Player::TeamId::FirstTeam;
	}
	else if(firstTeamPlayersCount == secondTeamPlayersCount) {
		// randomly picking team
		const auto seed = static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
		std::default_random_engine generator(seed);
		std::uniform_real_distribution<double> distribution(0.0, 1.0);
		
		const double value = distribution(generator);

		if(value < 0.5) {
			teamId = Player::TeamId::FirstTeam;
		}
	}

	players.emplace_back(Vector2D::ZERO, playerId, teamId);
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


std::vector<Player>& GameSession::getPlayers() {
	return m_storage.getPlayers();
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

	long long dt = GameSession::getCurrentTime_ms() - lastGameStateUpdate_ms;

	auto& players = m_storage.getPlayers();
	m_manager.updatePlayersPositions(players, dt / 1000.0);

	lastGameStateUpdate_ms = GameSession::getCurrentTime_ms();
}

long long GameSession::getCurrentTime_ms() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now().time_since_epoch()
	).count();
}

} // namespace invasion::game_models
