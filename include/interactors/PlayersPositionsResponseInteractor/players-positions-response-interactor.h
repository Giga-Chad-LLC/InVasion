#ifndef PLAYERS_POSITIONS_RESPONSE_INTERACTOR_H_
#define PLAYERS_POSITIONS_RESPONSE_INTERACTOR_H_

// game-models
#include "game-models/GameSession/game-session.h"
// response models
#include "players-positions-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;

struct PlayersPositionsResponseInteractor {
	response_models::PlayersPositionsResponseModel execute(GameSession& session) const;
};


} // namespace invasion::interactors



#endif // PLAYERS_POSITIONS_RESPONSE_INTERACTOR_H_