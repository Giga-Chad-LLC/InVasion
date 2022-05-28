#ifndef FORM_JSON_PLAYER_STATS_RESPONSE_INTERACTOR_H_
#define FORM_JSON_PLAYER_STATS_RESPONSE_INTERACTOR_H_


/**
 * Forms json response with the following structure:
 * {
 * 	  is_victory: bool,
 *    kills: int,
 *    deaths: int
 * }
*/


// libs
#include "libs/json/json.hpp"
// game-models
#include "game-models/GameSession/game-session.h"
 
 
namespace invasion::interactors {
using namespace game_models;
using json = nlohmann::json;


class FormJSONPlayerStatsResponseInteractor {
public:
	json execute(int playerId, GameSession& session) const;
};

 
} // namespace invasion::interactors
 
 
 
#endif // FORM_JSON_PLAYER_STATS_RESPONSE_INTERACTOR_H_
