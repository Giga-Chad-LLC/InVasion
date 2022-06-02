#include <string>
#include <utility>

#include "player-game-session-stats.h"


namespace invasion::game_models {
	
PlayerGameSessionStats::PlayerGameSessionStats()
	: m_kills(0), m_deaths(0) {}

int PlayerGameSessionStats::getKills() const {
	return m_kills;
}


void PlayerGameSessionStats::incrementKills() {
	++m_kills;
}


int PlayerGameSessionStats::getDeaths() const {
	return m_deaths;
}


void PlayerGameSessionStats::incrementDeaths() {
	++m_deaths;
}


std::string PlayerGameSessionStats::getUsername() const {
	return m_username;
}


void PlayerGameSessionStats::setUsername(std::string username) {
	m_username = std::move(username);
}


void PlayerGameSessionStats::copy(const PlayerGameSessionStats& stats) {
	m_kills = stats.m_kills;
	m_deaths = stats.m_deaths;
	m_username = stats.m_username;
}

} // namespace invasion::game_models
