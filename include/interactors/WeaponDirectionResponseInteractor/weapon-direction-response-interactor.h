#ifndef WEAPON_DIRECTION_RESPONSE_INTERACTOR_H_
#define WEAPON_DIRECTION_RESPONSE_INTERACTOR_H_

// game-models
#include "game-models/GameSession/game-session.h"
// request-models
#include "weapon-direction-request-model.pb.h"
// response-models
#include "weapon-direction-response-model.pb.h"


namespace invasion::interactors {
using namespace game_models;
using namespace request_models;
using namespace response_models;	


class WeaponDirectionResponseInteractor {
public:
	WeaponDirectionResponseModel execute(const WeaponDirectionRequestModel& req, GameSession& session) const;
};


} // namespace invasion::interactors



#endif // WEAPON_DIRECTION_RESPONSE_INTERACTOR_H_
