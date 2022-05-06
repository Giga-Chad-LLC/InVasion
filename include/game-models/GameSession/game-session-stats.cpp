#include "game-session-stats.h"

namespace invasion::game_models {

GameSessionStats::GameSessionStats() 
	: m_firstTeamPlayersCount(0),
	  m_secondTeamPlayersCount(0) {}
	  

int GameSessionStats::getFirstTeamPlayersCount() const {
	return m_firstTeamPlayersCount;
}


void GameSessionStats::incrementFirstTeamPlayersCount() {
	++m_firstTeamPlayersCount;
}


void GameSessionStats::decrementFirstTeamPlayersCount() {
	--m_firstTeamPlayersCount;
}


int GameSessionStats::getSecondTeamPlayersCount() const {
	return m_secondTeamPlayersCount;
}


void GameSessionStats::incrementSecondTeamPlayersCount() {
	++m_secondTeamPlayersCount;
}


void GameSessionStats::decrementSecondTeamPlayersCount() {
	--m_secondTeamPlayersCount;
}


} // namespace invasion::game_models
