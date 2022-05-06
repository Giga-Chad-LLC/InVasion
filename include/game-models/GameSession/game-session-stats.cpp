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


void GameSessionStats::incrementFirstTeamKillsCount() {
	++m_firstTeamKillsCount;
}


void GameSessionStats::decrementFirstTeamKillsCount() {
	--m_firstTeamKillsCount;
}


// 2nd team kills count
int GameSessionStats::getSecondTeamKillsCount() const {
	return m_secondTeamKillsCount;
}


void GameSessionStats::incrementSecondTeamKillsCount() {
	++m_secondTeamKillsCount;
}


void GameSessionStats::decrementSecondTeamKillsCount() {
	--m_secondTeamKillsCount;
}

} // namespace invasion::game_models
