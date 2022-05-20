#include <vector>
#include <memory>

// game-models
#include "game-models/Player/player.h"
#include "game-models/Player/player-team-id-enum.h"

#include "game-session-stats.h"


namespace invasion::game_models {

GameSessionStats::GameSessionStats() 
	: m_firstTeamPlayersCount(0),
	  m_secondTeamPlayersCount(0),
	  m_firstTeamKillsCount(0),
	  m_secondTeamKillsCount(0) {}

// 1st team players count
int GameSessionStats::getFirstTeamPlayersCount() const {
	return m_firstTeamPlayersCount;
}


void GameSessionStats::incrementFirstTeamPlayersCount() {
	++m_firstTeamPlayersCount;
}


void GameSessionStats::decrementFirstTeamPlayersCount() {
	--m_firstTeamPlayersCount;
}


// 2nd team players count
int GameSessionStats::getSecondTeamPlayersCount() const {
	return m_secondTeamPlayersCount;
}


void GameSessionStats::incrementSecondTeamPlayersCount() {
	++m_secondTeamPlayersCount;
}


void GameSessionStats::decrementSecondTeamPlayersCount() {
	--m_secondTeamPlayersCount;
}


// 1st team kills count
int GameSessionStats::getFirstTeamKillsCount() const {
	return m_firstTeamKillsCount;
}


// 2nd team kills count
int GameSessionStats::getSecondTeamKillsCount() const {
	return m_secondTeamKillsCount;
}


void GameSessionStats::updateTeamsKillsCounts(const std::vector<std::shared_ptr<Player>>& killedPlayers) {
	for (const auto& player_ptr : killedPlayers) {
		const PlayerTeamId teamId = player_ptr->getTeamId();

		if (teamId == PlayerTeamId::FirstTeam) {
			++m_secondTeamKillsCount;
		}
		else if (teamId == PlayerTeamId::SecondTeam) {
			++m_firstTeamKillsCount;
		}
	}
}

} // namespace invasion::game_models
