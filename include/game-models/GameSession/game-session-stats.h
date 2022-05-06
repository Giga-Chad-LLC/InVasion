#ifndef GAME_SESSION_STATS_H_
#define GAME_SESSION_STATS_H_



namespace invasion::game_models {
	
	class GameSessionStats {
	public:
		GameSessionStats();

		int getFirstTeamPlayersCount() const;
		void incrementFirstTeamPlayersCount();
		void decrementFirstTeamPlayersCount();

		int getSecondTeamPlayersCount() const;
		void incrementSecondTeamPlayersCount();
		void decrementSecondTeamPlayersCount();

	private:
		int m_firstTeamPlayersCount;
		int m_secondTeamPlayersCount;
	};

} // namespace invasion::game_models



#endif // GAME_SESSION_STATS_H_
