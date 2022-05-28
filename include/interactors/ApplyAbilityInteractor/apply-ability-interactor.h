#ifndef APPLY_ABILITY_INTERACTOR_H_
#define APPLY_ABILITY_INTERACTOR_H_

#include <optional>

// game-models
#include "game-models/GameSession/game-session.h"
// request-models
#include "apply-ability-request-model.pb.h"
// response-models
#include "supply-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;
using namespace request_models;

class ApplyAbilityInteractor {
public:
	std::optional<SupplyResponseModel> execute(const ApplyAbilityRequestModel& req, GameSession& session) const;
};
	
} // namespace invasion::interactors



#endif // APPLY_ABILITY_INTERACTOR_H_
