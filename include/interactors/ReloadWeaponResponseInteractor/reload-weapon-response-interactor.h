#ifndef RELOAD_WEAPON_RESPONSE_INTERACTOR_H_
#define RELOAD_WEAPON_RESPONSE_INTERACTOR_H_

#include <memory>

// game-models
#include "game-models/GameSession/game-session.h"
// request-models
#include "reload-weapon-request-model.pb.h"
// response-models
#include "weapon-state-response-model.pb.h"


namespace invasion::interactors {
using namespace game_models;
using namespace request_models;
using namespace response_models;


class ReloadWeaponResponseInteractor {
public:
	WeaponStateResponseModel execute(const ReloadWeaponRequestModel& req, std::shared_ptr<GameSession> session) const;
};

} // namespace invasion::interactors



#endif // RELOAD_WEAPON_RESPONSE_INTERACTOR_H_
