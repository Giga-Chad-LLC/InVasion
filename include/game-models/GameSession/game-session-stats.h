#ifndef GAME_SESSION_STATS_H_
#define GAME_SESSION_STATS_H_

#include <vector>
#include <memory>

// game-models
#include "game-models/Player/player.h"


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

		int getFirstTeamKillsCount() const;
		int getSecondTeamKillsCount() const;
		void updateTeamsKillsCounts(const std::vector<std::shared_ptr<Player>>& killedPlayers);

	private:
		int m_firstTeamPlayersCount;
		int m_secondTeamPlayersCount;
		int m_firstTeamKillsCount;
		int m_secondTeamKillsCount; 
	};

} // namespace invasion::game_models



#endif // GAME_SESSION_STATS_H_
