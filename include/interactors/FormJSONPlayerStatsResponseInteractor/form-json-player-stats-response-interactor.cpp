#include <memory>

#include "form-json-player-stats-response-interactor.h"

// libs
#include "libs/json/json.hpp" 
// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/GameSession/game-match-result-enum.h"
#include "game-models/Player/player.h"
#include "game-models/Player/player-team-id-enum.h"
 
 
namespace invasion::interactors {
using namespace game_models;
using json = nlohmann::json;
 

json FormJSONPlayerStatsResponseInteractor::execute(const int playerId, GameSession& session) const {
	
	std::shared_ptr<Player> player = session.getPlayer(playerId);

	json response;

	const PlayerTeamId teamId = player->getTeamId();
	const GameMatchResult matchResult = session.getMatchResult();
	
	// defining whether player has won the match
	if(teamId == PlayerTeamId::FirstTeam  && matchResult == GameMatchResult::FirstTeamVictory ||
	   teamId == PlayerTeamId::SecondTeam && matchResult == GameMatchResult::SecondTeamVictory) {
		   response["is_victory"] = true;
	}
	else {
		response["is_victory"] = false;
	}


	const int kills = player->getGameSessionStats().getKills();
	const int deaths = player->getGameSessionStats().getDeaths();

	response["kills"] = kills;
	response["deaths"] = deaths;

	return response;
}


} // namespace invasion::interactors