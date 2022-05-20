#ifndef SELECT_PLAYER_SPECIALIZATION_INTERACTOR_H_
#define SELECT_PLAYER_SPECIALIZATION_INTERACTOR_H_

// game-models
#include "game-models/GameSession/game-session.h"
// request-models
#include "select-player-specialization-request-model.pb.h"
// response-models
#include "player-specialization-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;
using namespace request_models;

class SelectPlayerSpecializationInteractor {
public:
	PlayerSpecializationResponseModel execute(const SelectPlayerSpecializationRequestModel& req, GameSession& session) const;
};

} // namespace invasion::interactors


#endif // SELECT_PLAYER_SPECIALIZATION_INTERACTOR_H_
