#ifndef UPDATE_PLAYER_HITPOINTS_RESPONSE_INTERACTOR_H_
#define UPDATE_PLAYER_HITPOINTS_RESPONSE_INTERACTOR_H_

// game-models
#include "game-models/GameSession/game-session.h"
// response-models
#include "update-player-hitpoints-response-model.pb.h"


namespace invasion::interactors {
using namespace game_models; 
using namespace response_models;


class UpdatePlayerHitpointsResponseInteractor {
public:
	UpdatePlayerHitpointsResponseModel execute(int playerId, GameSession& session) const;
};


} // namespace invasion::interactors



#endif // UPDATE_PLAYER_HITPOINTS_RESPONSE_INTERACTOR_H_
