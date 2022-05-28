#ifndef SET_PLAYER_USERNAME_INTERACTOR_H_
#define SET_PLAYER_USERNAME_INTERACTOR_H_
 
// game-models
#include "game-models/GameSession/game-session.h"
// request-models
#include "client-credentials-request-model.pb.h"
 
 
namespace invasion::interactors {
using namespace game_models;
using namespace request_models;
 

class SetPlayerUsernameInteractor {
public:
	void execute(const ClientCredentialsRequestModel& req, GameSession& session) const;
};


} // namespace invasion::interactors
 
 
 
#endif // SET_PLAYER_USERNAME_INTERACTOR_H_
