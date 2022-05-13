#ifndef KILLED_PLAYERS_RESPONSE_INTERACTOR_H_
#define KILLED_PLAYERS_RESPONSE_INTERACTOR_H_

// game-models
#include "game-models/GameSession/game-session.h"
// response-models
#include "game-state-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;

class KilledPlayersResponseInteractor {
public:
	void execute(GameStateResponseModel& response, GameSession& session) const;
};

} // namespace invasion::interactors



#endif // KILLED_PLAYERS_RESPONSE_INTERACTOR_H_
