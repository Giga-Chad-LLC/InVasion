#ifndef CLIENT_DISCONNECTED_RESPONSE_INTERACTOR_H_
#define CLIENT_DISCONNECTED_RESPONSE_INTERACTOR_H_

// game-models
#include "game-models/GameSession/game-session.h"
// response-models
#include "client-disconnected-response-model.pb.h"


namespace invasion::interactors {
using namespace game_models;
using namespace response_models;

class ClientDisconnectedResponseInteractor {
public:
	ClientDisconnectedResponseModel execute(int playerId, GameSession& session) const;
};

} // namespace invasion::interactors



#endif // CLIENT_DISCONNECTED_RESPONSE_INTERACTOR_H_
