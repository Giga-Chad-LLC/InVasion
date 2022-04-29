#include <vector>
#include <cassert>
#include <utility>
#include <random>
#include <chrono>
#include <ctime> 
#include <iostream>
#include <algorithm>
#include <memory>
#include <cmath>

// game-models
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-session.h"
// utils
#include "utils/TimeUtilities/time-utilities.h"


namespace invasion::game_models {

GameSession::GameSession() 
	: m_lastGameStateUpdate_ms(0),
	  m_firstTeamPlayersCount(0),
	  m_secondTeamPlayersCount(0),
	  m_nextBulletId(0),
	  m_nextPlayerId(0) {
	m_lastGameStateUpdate_ms = utils::TimeUtilities::getCurrentTime_ms();
}


// returns id of created player
int GameSession::createPlayerAndReturnId() {
	std::vector<std::shared_ptr<Player>>& players = m_storage.getPlayers();

	// assigning team to new player
	PlayerTeamId teamId = PlayerTeamId::FirstTeam;

	if (m_firstTeamPlayersCount > m_secondTeamPlayersCount) {
		teamId = PlayerTeamId::SecondTeam;
	}
	else if (m_firstTeamPlayersCount == m_secondTeamPlayersCount) {
		// randomly picking team
		const auto seed = static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
		std::default_random_engine generator(seed);
		std::bernoulli_distribution distribution(0.5);
		const bool value = distribution(generator);

		if (value) {
			teamId = PlayerTeamId::SecondTeam;
		}
	}

	if (teamId == PlayerTeamId::FirstTeam) {
		m_firstTeamPlayersCount++;
	}
	else {
		m_secondTeamPlayersCount++;
	}

	players.push_back(std::make_shared<Player>(Vector2D::ZERO, m_nextPlayerId, teamId));
	return m_nextPlayerId++;
}


int GameSession::addBullet(std::shared_ptr<Bullet> bullet) {
	const int id = bullet->getId();
	m_storage.getBullets().push_back(bullet);	
	return id;
}

int GameSession::createIdForNewBullet() {
	return m_nextBulletId++;
}


std::shared_ptr<Player> GameSession::getPlayer(const int playerId) {
	std::vector<std::shared_ptr<Player>>& players = m_storage.getPlayers();
	std::shared_ptr<Player> player_ptr = nullptr;

	for(const auto& player : players) {
		if(playerId == player->getId()) {
			player_ptr = player;
			break;
		}
	}
	
	if(player_ptr == nullptr) {
		std::cout << "Cannot find player with id: " << playerId << " in GameSession::getPlayer" << std::endl;
	}

	assert(player_ptr != nullptr);
	return player_ptr;
}


std::vector<std::shared_ptr<Player>>& GameSession::getPlayers() {
	return m_storage.getPlayers();
}


std::vector<std::shared_ptr<Player>>& GameSession::getDamagedPlayers() {
	return m_storage.getDamagedPlayers();
}


std::vector<std::shared_ptr<Bullet>>& GameSession::getBullets() {
	return m_storage.getBullets();
}


std::shared_ptr<Bullet> GameSession::getBullet(int bulletId) {
	std::vector<std::shared_ptr<Bullet>>& bullets = m_storage.getBullets();
	std::shared_ptr<Bullet> bullet_ptr = nullptr;

	for(const auto& bullet : bullets) {
		if(bulletId == bullet->getId()) {
			bullet_ptr = bullet;
			break;
		}
	}

	if(bullet_ptr == nullptr) {
		std::cout << "Cannot find bullet with id: " << bulletId << " in GameSession::getPlayer" << std::endl;
	}

	assert(bullet_ptr != nullptr);
	return bullet_ptr;
}




void GameSession::updateGameState() {
	// trying update players/bullets positions: manager.tryUpdatePosition(...): TODO
	// checking collisions: TODO
	// deleting objects (killing players, deleting bullets) if needed: TODO
	// making positions update: manager.updatePlayersPositions(...) && manager.updateBulletsPositions(...): TODO

	const double dt_s = (utils::TimeUtilities::getCurrentTime_ms() - m_lastGameStateUpdate_ms) / 1000.0;

	auto& players = m_storage.getPlayers();
	auto& damagedPlayers = m_storage.getDamagedPlayers();
	auto& bullets = m_storage.getBullets();


	m_manager.updatePlayersPositions(players, dt_s);
	m_manager.updateBulletsPositions(bullets, players, dt_s);

	damagedPlayers.clear();
	m_manager.findDamagedPlayers(players, damagedPlayers);

	// deleting crushed or went out of bounds bullets
	bullets.erase(
		std::remove_if(
			std::begin(bullets),
			std::end(bullets), 
			[](const std::shared_ptr<Bullet>& bullet_ptr) {
				const Vector2D pos = bullet_ptr->getPosition();
				bool bulletOutOfMapBounds = std::abs(pos.getX()) > 1000 || std::abs(pos.getY()) > 1000;

				return bullet_ptr->isInCrushedState() || bulletOutOfMapBounds;
			}
		),
		std::end(bullets)
	);


	// --------------- TODO: respawn dead players --------------- //

	m_lastGameStateUpdate_ms = utils::TimeUtilities::getCurrentTime_ms();
}


} // namespace invasion::game_models
