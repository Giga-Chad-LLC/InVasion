#ifndef ROTATE_WEAPON_INTERACTOR_H_
#define ROTATE_WEAPON_INTERACTOR_H_


#include "game-models/GameSession/game-session.h"
#include "rotate-weapon-request-model.pb.h"

namespace invasion::interactors {
using namespace invasion::game_models;
using namespace request_models;

struct RotateWeaponInteractor {
	void execute(const RotateWeaponRequestModel& req, GameSession& session) const;
};


} // namespace invasion::interactors



#endif // ROTATE_WEAPON_INTERACTOR_H_