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


void PlayerGameSessionStats::copy(const PlayerGameSessionStats& stats) {
	m_kills = stats.m_kills;
	m_deaths = stats.m_deaths;
}

} // namespace invasion::game_models
