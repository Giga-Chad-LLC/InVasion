#include <memory>

#include "update-player-hitpoints-response-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
// response-models
#include "update-player-hitpoints-response-model.pb.h"


namespace invasion::interactors {
using namespace game_models; 
using namespace response_models;

UpdatePlayerHitpointsResponseModel UpdatePlayerHitpointsResponseInteractor::execute(
	int playerId, GameSession& session) const {
	const std::shared_ptr<Player> player = session.getPlayer(playerId);

	const int hitpoints = player->getLifeState().getHitPoints();

	UpdatePlayerHitpointsResponseModel response;
	response.set_player_id(playerId);
	response.set_new_hitpoints(hitpoints);

	return response;
}


} // namespace invasion::interactors
