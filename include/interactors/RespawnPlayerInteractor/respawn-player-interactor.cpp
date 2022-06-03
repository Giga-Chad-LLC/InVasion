#include <memory>

#include "respawn-player-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
#include "game-models/Vector2D/vector2d.h"
// request-models
#include "respawn-player-request-model.pb.h"
// response-models
#include "respawn-player-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;
using namespace request_models;

RespawnPlayerResponseModel RespawnPlayerInteractor::execute(const RespawnPlayerRequestModel& req,
															GameSession& session) const {
	const int playerId = req.player_id();
	std::shared_ptr<Player> player_ptr = session.getPlayer(playerId);

	Vector2D point = session.getRespawnPoint(player_ptr->getTeamId());
	player_ptr->respawn(point);

	RespawnPlayerResponseModel response;
	response.set_player_id(playerId);
	
	return response;
}

} // namespace invasion::interactors
