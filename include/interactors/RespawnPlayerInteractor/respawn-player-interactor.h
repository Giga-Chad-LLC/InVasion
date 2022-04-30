#ifndef RESPAWN_PLAYER_INTERACTOR_H_
#define RESPAWN_PLAYER_INTERACTOR_H_

// game-models
#include "game-models/GameSession/game-session.h"
// request-models
#include "respawn-player-request-model.pb.h"
// response-models
#include "respawn-player-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;
using namespace request_models;

class RespawnPlayerInteractor {
public:
	RespawnPlayerResponseModel execute(const RespawnPlayerRequestModel& req, GameSession& session) const;
};

} // namespace invasion::interactors



#endif // RESPAWN_PLAYER_INTERACTOR_H_
