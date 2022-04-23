#ifndef BULLETS_POSITIONS_RESPONSE_INTERACTOR_H_
#define BULLETS_POSITIONS_RESPONSE_INTERACTOR_H_


// game-models
#include "game-models/GameSession/game-session.h"
// response-models
#include "game-state-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;

struct BulletsPositionsResponseInteractor {
	void execute(GameStateResponseModel& response, GameSession& session) const;
};

} // namespace invasion::interactors



#endif // BULLETS_POSITIONS_RESPONSE_INTERACTOR_H_