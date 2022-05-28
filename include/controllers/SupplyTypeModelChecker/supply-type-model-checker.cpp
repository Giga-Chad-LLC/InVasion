#include <memory>

#include "supply-type-model-checker.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/StaticSupply/static-supply.h"
#include "game-models/StaticSupply/static-supply-enum.h"


namespace invasion::controllers {
using namespace game_models;	


bool SupplyTypeModelChecker::isAmmoCrateType(int supplyId, GameSession& session) const {
	std::shared_ptr<StaticSupply> supply = session.getSupply(supplyId);
	return supply->getType() == StaticSupplyType::AmmoCrate;
}


bool SupplyTypeModelChecker::isAidKitType(int supplyId, GameSession& session) const {
	std::shared_ptr<StaticSupply> supply = session.getSupply(supplyId);
	return supply->getType() == StaticSupplyType::AidKit;
}


} // namespace invasion::controllers
