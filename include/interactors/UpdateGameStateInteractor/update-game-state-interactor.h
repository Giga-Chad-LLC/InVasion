#ifndef UPDATE_GAME_STATE_INTERACTOR_H_
#define UPDATE_GAME_STATE_INTERACTOR_H_

// game-models
#include "game-models/GameSession/game-session.h"
// response models
#include "player-position-response-model.pb.h"
#include "players-positions-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;

struct UpdateGameStateInteractor {
	response_models::PlayersPositionsResponseModel execute(GameSession& session) const;
};


} // namespace invasion::interactors



#endif // UPDATE_GAME_STATE_INTERACTOR_H_