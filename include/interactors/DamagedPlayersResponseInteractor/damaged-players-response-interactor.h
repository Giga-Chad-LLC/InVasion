#ifndef DAMAGED_PLAYERS_RESPONSE_INTERACTOR_H_
#define DAMAGED_PLAYERS_RESPONSE_INTERACTOR_H_


#include <optional>
// game-models
#include "game-models/GameSession/game-session.h"
// response-models
#include "damaged-players-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;

struct DamagedPlayersResponseInteractor {
	std::optional<response_models::DamagedPlayersResponseModel> execute(GameSession& session) const;
};


} // namespace invasion::interactors


#endif // DAMAGED_PLAYERS_RESPONSE_INTERACTOR_H_