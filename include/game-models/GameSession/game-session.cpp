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
#include <filesystem>
#include <string>

#include "game-session.h"
#include "game-session-stats.h"
// game-models
#include "game-models/Player/player.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Player/player-game-session-stats.h"
#include "game-models/PlayerManager/player-manager.h"
#include "game-models/PlayersFactory/players-factory.h"
#include "game-models/BulletManager/bullet-manager.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/StaticObject/static-object.h"
// controllers
#include "controllers/StaticObjectsFileReader/static-objects-file-reader.h"
#include "controllers/DirectoryFilesContainer/directory-files-container.h"
// utils
#include "utils/TimeUtilities/time-utilities.h"


namespace invasion::game_models {

GameSession::GameSession() 
	: m_lastGameStateUpdate_ms(0),
	  m_nextBulletId(0),
	  m_nextPlayerId(0) {
	
	controllers::DirectoryFilesContainer container(TILEMAPS_ASSETS_DIRECTORY);
	std::vector<std::filesystem::directory_entry> entries = container.obtainFilesWithExtension(".txt");

	std::vector<std::shared_ptr<StaticObject>>& obstacles = m_storage.getObstacles();

	for(const auto& entry  : entries) {
		controllers::StaticObjectsFileReader reader(entry.path().string());

		const auto& objects = reader.getObjectsData();

		for(const auto& object : objects) {
			const Vector2D shape(object.getShape());
			const Vector2D position(object.getPosition()); 
			obstacles.push_back(std::make_shared<StaticObject>(shape, shape, position));
		}
	}
	std::cout << "Static files uploaded in GameSession, static objects count: " << obstacles.size() << std::endl;

	m_lastGameStateUpdate_ms = utils::TimeUtilities::getCurrentTime_ms();
}


// returns id of created player
int GameSession::createPlayerAndReturnId(PlayerSpecialization specialization) {
	std::vector<std::shared_ptr<Player>>& players = m_storage.getPlayers();

	// assigning team to new player
	PlayerTeamId teamId = PlayerTeamId::FirstTeam;

	const int firstTeamPlayersCount  = m_gameStatistics.getFirstTeamPlayersCount();
	const int secondTeamPlayersCount = m_gameStatistics.getSecondTeamPlayersCount();

	if (firstTeamPlayersCount > secondTeamPlayersCount) {
		teamId = PlayerTeamId::SecondTeam;
	}
	else if (firstTeamPlayersCount == secondTeamPlayersCount) {
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
		m_gameStatistics.incrementFirstTeamPlayersCount();
	}
	else if (teamId == PlayerTeamId::SecondTeam) {
		m_gameStatistics.incrementSecondTeamPlayersCount();
	}

	std::shared_ptr<Player> player = PlayersFactory::createPlayer(
		Vector2D::ZERO,
		m_nextPlayerId,
		teamId,
		specialization
	);
	players.push_back(player);
	return m_nextPlayerId++;
}


int GameSession::createIdForNewBullet() {
	return m_nextBulletId++;
}


int GameSession::addBullet(std::shared_ptr<Bullet> bullet) {
	const int id = bullet->getId();
	m_storage.getBullets().push_back(bullet);	
	return id;
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



std::vector<std::shared_ptr<Player>>& GameSession::getPlayers() {
	return m_storage.getPlayers();
}


std::vector<std::shared_ptr<Player>>& GameSession::getDamagedPlayers() {
	return m_storage.getDamagedPlayers();
}


std::vector<std::shared_ptr<Player>>& GameSession::getKilledPlayers() {
	return m_storage.getKilledPlayers();
}


std::vector<std::shared_ptr<Bullet>>& GameSession::getBullets() {
	return m_storage.getBullets();
}


std::vector<std::shared_ptr<StaticObject>>& GameSession::getObstacles() {
	return m_storage.getObstacles();
}


void GameSession::removePlayerById(const int playerId) {
	auto& players = m_storage.getPlayers();
	
	// asserting player with passed id exists to debugging purposes
	// the following loop must be removed after testing
	 {
		bool playerExists = false;

		for (int i = 0; i < players.size(); i++) {
			if(playerId == players[i]->getId()) {
				playerExists = true;
				break;
			}
		}

		if (!playerExists) {
			std::cout << "In GameSession: removePlayerById() called with playerId == " << playerId
					<< ", but player with the id does not exist" << std::endl;
		}
		assert(playerExists);
	 }

	PlayerTeamId teamId;
	bool playerFound = false;

	for (const auto& player_ptr : players) {
		if (playerId == player_ptr->getId()) {
			teamId = player_ptr->getTeamId();
			playerFound = true;
			break;
		}
	}

	assert(playerFound);

	players.erase(std::remove_if(
		players.begin(),
		players.end(),
		[playerId](const std::shared_ptr<Player>& player_ptr) {
			return playerId == player_ptr->getId();
		}
	), players.end());

	if (teamId == PlayerTeamId::FirstTeam) {
		m_gameStatistics.decrementFirstTeamPlayersCount();
	}
	else {
		m_gameStatistics.decrementSecondTeamPlayersCount();
	}
}


void GameSession::changePlayerSpecialization(int playerId, PlayerSpecialization specialization) {
	std::shared_ptr<Player> player = this->getPlayer(playerId);
	// removing player
	this->removePlayerById(playerId);

	const Vector2D position = player->getPosition();
	const PlayerTeamId teamId = player->getTeamId();

	// updating specialization
	std::shared_ptr<Player> updatedPlayer = PlayersFactory::createPlayer(position, playerId, teamId, specialization);
	
	// copying stats and state
	PlayerGameSessionStats& stats = updatedPlayer->getGameSessionStats();
	stats.copy(player->getGameSessionStats());
	PlayerLifeState& lifeState = updatedPlayer->getLifeState();
	lifeState.copyDeadState(player->getLifeState());

	// inserting updated player
	std::vector<std::shared_ptr<Player>> &players = this->getPlayers();
	players.push_back(updatedPlayer);
	
	if (teamId == PlayerTeamId::FirstTeam) {
		m_gameStatistics.incrementFirstTeamPlayersCount();
	}
	else if (teamId == PlayerTeamId::SecondTeam) {
		m_gameStatistics.incrementSecondTeamPlayersCount();
	}
}


void GameSession::updateGameState() {
	auto& players = m_storage.getPlayers();
	auto& damagedPlayers = m_storage.getDamagedPlayers();
	auto& killedPlayers = m_storage.getKilledPlayers();
	auto& bullets = m_storage.getBullets();
	auto& obstacles = m_storage.getObstacles();

	const double dt_s = (utils::TimeUtilities::getCurrentTime_ms() - m_lastGameStateUpdate_ms) / 1000.0;

	m_playerManager.updatePlayersPositions(players, obstacles, dt_s);
	m_bulletManager.updateBulletsPositions(bullets, players, obstacles, dt_s);
	m_playerManager.findDamagedPlayers(players, damagedPlayers); // vector damagedPlayers cleared inside the method
	m_playerManager.findKilledPlayers(players, killedPlayers); // vector killedPlayers cleared inside the method
	m_bulletManager.removeCrushedAndFlewOutOfBoundsBullets(bullets);

	m_gameStatistics.updateTeamsKillsCounts(killedPlayers);
	m_playerManager.updatePlayersGameSessionStats(players, killedPlayers);

	m_lastGameStateUpdate_ms = utils::TimeUtilities::getCurrentTime_ms();
}

bool GameSession::playerExists(int playerId) {
	const auto& players = getPlayers();
	for (auto player : players) {
		if (player->getId() == playerId) {
			return true;
		}
	}
	return false;
}


const GameSessionStats& GameSession::getGameStatistics() {
	return m_gameStatistics;
}


} // namespace invasion::game_models
