#include "supply-type-model-checker.h"

// util-models
#include "supply-model.pb.h"
#include "supply-type.pb.h"

namespace invasion::controllers {
	
bool SupplyTypeModelChecker::isAidKit(const SupplyModel& supply) const {
	return supply.supply_type() == SupplyType::AidKit;
}


bool SupplyTypeModelChecker::isAmmoCrate(const SupplyModel& supply) const {
	return supply.supply_type() == SupplyType::AmmoCrate;
}


} // namespace invasion::controllers
