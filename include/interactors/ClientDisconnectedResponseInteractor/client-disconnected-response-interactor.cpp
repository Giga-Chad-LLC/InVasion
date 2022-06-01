#include "client-disconnected-response-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
// response-models
#include "client-disconnected-response-model.pb.h"


namespace invasion::interactors {
using namespace game_models;
using namespace response_models;

ClientDisconnectedResponseModel ClientDisconnectedResponseInteractor::execute(int playerId, GameSession& session) const {
	ClientDisconnectedResponseModel response;
	response.set_player_id(playerId);

	return response;
}

} // namespace invasion::interactors
