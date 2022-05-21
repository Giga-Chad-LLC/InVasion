#ifndef HANDSHAKE_RESPONSE_INTERACTOR_H_
#define HANDSHAKE_RESPONSE_INTERACTOR_H_

// game-models
#include "game-models/GameSession/game-session.h"
// response-models
#include "handshake-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;

class HandshakeResponseInteractor {
public:
	HandshakeResponseModel execute(GameSession& session, int playerId) const;
};


} // namespace invasion::interactors



#endif // HANDSHAKE_RESPONSE_INTERACTOR_H_
