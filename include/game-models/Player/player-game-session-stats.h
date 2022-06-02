#ifndef PLAYER_GAME_SESSION_STATS_H_
#define PLAYER_GAME_SESSION_STATS_H_

#include <string>

namespace invasion::game_models {
	
class PlayerGameSessionStats {
public:
	explicit PlayerGameSessionStats();

	int getKills() const;
	void incrementKills();

	int getDeaths() const;
	void incrementDeaths();

	std::string getUsername() const;
	void setUsername(std::string username);

	void copy(const PlayerGameSessionStats& stats);

private:
	int m_kills;
	int m_deaths;
	std::string m_username;
};

} // namespace invasion::game_models



#endif // PLAYER_GAME_SESSION_STATS_H_
