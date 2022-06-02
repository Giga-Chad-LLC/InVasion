#include "client-connected-response-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player-team-id-enum.h"
// response-models
#include "client-connected-response-model.pb.h"
// util-models
#include "player-team-id.pb.h"


namespace invasion::interactors {
using namespace game_models;
using namespace response_models;


ClientConnectedResponseModel ClientConnectedResponseInteractor::execute(const int playerId, GameSession& session) const {
	ClientConnectedResponseModel response;
	
	response.set_player_id(playerId);
	
	auto player = session.getPlayer(playerId);
	const PlayerTeamId teamId = player->getTeamId();

	if(teamId == PlayerTeamId::FirstTeam) {
		response.set_team_id(util_models::PlayerTeamId::FirstTeam);
	}
	else if(teamId == PlayerTeamId::SecondTeam) {
		response.set_team_id(util_models::PlayerTeamId::SecondTeam);
	}

	return response;
}

}