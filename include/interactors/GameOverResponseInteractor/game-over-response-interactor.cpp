#include "game-over-response-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/GameSession/game-match-result-enum.h"
// response-models
#include "game-over-response-model.pb.h"
#include "game-match-result.pb.h"


namespace invasion::interactors {
using namespace game_models;
using namespace response_models;


GameOverResponseModel GameOverResponseInteractor::execute(GameSession& session) const {
	GameMatchResult result = session.getMatchResult();

	GameOverResponseModel response;

	if(result == GameMatchResult::FirstTeamVictory) {
		response.set_winning_team(util_models::GameMatchResult::FirstTeamVictory);
	}
	else if(result == GameMatchResult::SecondTeamVictory) {
		response.set_winning_team(util_models::GameMatchResult::SecondTeamVictory);
	}
	else if(result == GameMatchResult::Draw) {
		response.set_winning_team(util_models::GameMatchResult::Draw);
	}

	return response;
}


} // namespace invasion::interactors
