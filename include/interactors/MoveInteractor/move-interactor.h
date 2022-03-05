#ifndef INTERACTOR_H_
#define INTERACTOR_H_

#include "game-models/GameSession/game-session.h"
#include "move-request-model.pb.h"

namespace invasion::interactors {

struct MoveInteractor {
	void execute(const request_models::MoveRequestModel& req, game_models::GameSession& session) const;
};

} // namespace invasion::interactors


#endif // INTERACTOR_H_