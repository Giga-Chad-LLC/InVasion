#ifndef USE_SUPPLY_INTERACTOR_H_
#define USE_SUPPLY_INTERACTOR_H_

#include <optional>

// game-models
#include "game-models/GameSession/game-session.h"
// request-models
#include "use-supply-request-model.pb.h"
// response-models
#include "use-supply-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;
using namespace request_models;

class UseSupplyInteractor {
public:
	std::optional<UseSupplyResponseModel> execute(const UseSupplyRequestModel& req, GameSession& session) const; 
};

} // namespace invasion::interactors



#endif // USE_SUPPLY_INTERACTOR_H_