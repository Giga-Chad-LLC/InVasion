#ifndef HANDSHAKE_RESPONSE_INTERACTOR_H_
#define HANDSHAKE_RESPONSE_INTERACTOR_H_

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/StaticSupply/static-supply-enum.h"
// response-models
#include "handshake-response-model.pb.h"
// util-models
#include "player-team-id.pb.h"
#include "supply-type.pb.h"
#include "player-specialization.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;

class HandshakeResponseInteractor {
public:
	HandshakeResponseModel execute(std::size_t remainingSessionTime_ms, GameSession& session, int playerId) const;

private:
	util_models::PlayerTeamId mapPlayerTeamIds(PlayerTeamId teamId) const;
	util_models::SupplyType mapSupplyTypes(StaticSupplyType supplyType) const;
	util_models::PlayerSpecialization mapPlayerSpecializations(PlayerSpecialization specialization) const;
};


} // namespace invasion::interactors



#endif // HANDSHAKE_RESPONSE_INTERACTOR_H_
