#ifndef GAME_SESSION_STATS_INTERACTOR_H_
#define GAME_SESSION_STATS_INTERACTOR_H_

#include <optional>
// game-models
#include "game-models/GameSession/game-session.h"
// response-models
#include "game-session-stats-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;
	
class GameSessionStatsInteractor {
public:
	std::optional<GameSessionStatsResponseModel> execute(GameSession& session) const;

private:

};

} // namespace invasion::interactors



#endif // GAME_SESSION_STATS_INTERACTOR_H_
