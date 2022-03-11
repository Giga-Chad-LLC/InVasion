#ifndef SHOOT_INTERACTOR_H_
#define SHOOT_INTERACTOR_H_

// request-models
#include "shoot-request-model.pb.h"
// response-models
#include "shooting-state-response-model.pb.h"
// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
#include "game-models/Bullet/bullet.h"


namespace invasion::interactors {
	

struct ShootInteractor {
	response_models::ShootingStateResponseSchema execute(
		const request_models::ShootRequestModel& req, 
		game_models::GameSession& session
	) const;
};


} // namespace invasion::interactors



#endif // SHOOT_INTERACTOR_H_