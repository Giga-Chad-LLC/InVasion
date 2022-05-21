#include <optional>
#include <vector>
#include <memory>

#include "use-supply-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/StaticSupply/static-supply.h"
#include "game-models/Player/player.h"
// request-models
#include "use-supply-request-model.pb.h"
// response-models
#include "use-supply-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;
using namespace request_models;

std::optional<UseSupplyResponseModel> UseSupplyInteractor::execute(const UseSupplyRequestModel& req, GameSession& session) const {
	const int playerId = req.player_id();
	std::shared_ptr<Player> player = session.getPlayer(playerId);
	
	std::shared_ptr<StaticSupply> closestSupply;
	double currentDistance = 0.0;
	bool found = false;

	std::vector<std::shared_ptr<StaticSupply>>& supplies = session.getSupplies();

	// searching for closest supply
	for(const auto supply : supplies) {
		if(!supply->isActive()) {
			continue;
		}

		const double distance = player->calculateClosestDistanceBetween(supply.get());

		if(distance <= GameSession::MAX_INTERACTIVE_DISTANCE && (distance <= currentDistance || !found)) {
			closestSupply = supply;
			currentDistance = distance;
			found = true;
		}
	}

	if(!found) {
		return std::nullopt;
	}

	closestSupply->supply(player);

	UseSupplyResponseModel response;

	response.set_player_id(playerId);
	response.set_supply_id(closestSupply->getId());

	// setting supply type
	const StaticSupplyType supplyType = closestSupply->getType();
	if(supplyType == StaticSupplyType::AidKit) {
		response.set_applied_supply_type(util_models::SupplyType::AidKit);
	}
	else if(supplyType == StaticSupplyType::AmmoCrate) {
		response.set_applied_supply_type(util_models::SupplyType::AmmoCrate);
	}

	response.set_left_supply_capacity(closestSupply->getLeftSupplyCapacity());
	response.set_is_active(closestSupply->isActive());

	return response;
}

} // namespace invasion::interactors
