#ifndef BULLETS_POSITIONS_RESPONSE_INTERACTOR_H_
#define BULLETS_POSITIONS_RESPONSE_INTERACTOR_H_


// game-models
#include "game-models/GameSession/game-session.h"
// response-models
#include "bullets-positions-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;

struct BulletsPositionsResponseInteractor {
	response_models::BulletsPositionsResponseModel execute(GameSession& session) const;
};

} // namespace invasion::interactors



#endif // BULLETS_POSITIONS_RESPONSE_INTERACTOR_H_