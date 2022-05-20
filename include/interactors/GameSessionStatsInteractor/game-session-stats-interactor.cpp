#include <optional>
#include <vector>
#include <memory>

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/GameSession/game-session-stats.h"
#include "game-models/Player/player.h"
// response-models
#include "game-session-stats-response-model.pb.h"
#include "player-game-session-stats-response-model.pb.h"


#include "game-session-stats-interactor.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;

std::optional<GameSessionStatsResponseModel> GameSessionStatsInteractor::execute(GameSession& session) const {
	const std::vector<std::shared_ptr<Player>> &killedPlayers = session.getKilledPlayers();

	if (killedPlayers.empty()) {
		return std::nullopt;
	}

	const GameSessionStats& statistics = session.getGameStatistics();

	GameSessionStatsResponseModel response;
	response.set_first_team_kills(statistics.getFirstTeamKillsCount());
	response.set_second_team_kills(statistics.getSecondTeamKillsCount());

	for (const auto& player_ptr : killedPlayers) {
		const int id = player_ptr->getId();
		const int kills = player_ptr->getGameSessionStats().getKills();
		const int deaths = player_ptr->getGameSessionStats().getDeaths();
		
		PlayerGameSessionStatsResponseModel* model = response.add_players_stats();
		model->set_player_id(id);
		model->set_kills(kills);
		model->set_deaths(deaths);
	}

	return response;
}

} // namespace invasion::interactors
