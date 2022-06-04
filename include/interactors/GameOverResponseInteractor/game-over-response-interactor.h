#ifndef GAME_OVER_RESPONSE_INTERACTOR_H_
#define GAME_OVER_RESPONSE_INTERACTOR_H_


// game-models
#include "game-models/GameSession/game-session.h"
// response-models
#include "game-over-response-model.pb.h"


namespace invasion::interactors {
using namespace game_models;
using namespace response_models;


class GameOverResponseInteractor {
public:
	GameOverResponseModel execute(GameSession& session) const;
};

} // namespace invasion::interactors



#endif // GAME_OVER_RESPONSE_INTERACTOR_H_