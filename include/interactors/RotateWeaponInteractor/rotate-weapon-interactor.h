#ifndef ROTATE_WEAPON_INTERACTOR_H_
#define ROTATE_WEAPON_INTERACTOR_H_


#include "game-models/GameSession/game-session.h"
#include "rotate-weapon-request-model.pb.h"

namespace invasion::interactors {
	

struct RotateWeaponInteractor {
	void execute(const request_models::RotateWeaponRequestModel& req, game_models::GameSession& session) const;
};


} // namespace invasion::interactors



#endif // ROTATE_WEAPON_INTERACTOR_H_