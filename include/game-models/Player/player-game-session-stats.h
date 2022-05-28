#ifndef PLAYER_GAME_SESSION_STATS_H_
#define PLAYER_GAME_SESSION_STATS_H_


namespace invasion::game_models {
	
class PlayerGameSessionStats {
public:
	explicit PlayerGameSessionStats();

	int getKills() const;
	void incrementKills();

	int getDeaths() const;
	void incrementDeaths();

	void copy(const PlayerGameSessionStats& stats);

private:
	int m_kills;
	int m_deaths;
};

} // namespace invasion::game_models



#endif // PLAYER_GAME_SESSION_STATS_H_
