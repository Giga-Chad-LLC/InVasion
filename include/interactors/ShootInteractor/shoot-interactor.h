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
using namespace invasion::game_models;
using namespace response_models;
using namespace request_models;

class ShootInteractor {
public:
	ShootingStateResponse execute(const ShootRequestModel& req, GameSession& session) const;
};


} // namespace invasion::interactors



#endif // SHOOT_INTERACTOR_H_