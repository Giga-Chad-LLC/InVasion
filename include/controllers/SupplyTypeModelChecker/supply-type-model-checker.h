#ifndef SUPPLY_TYPE_MODEL_CHECKER_H_
#define SUPPLY_TYPE_MODEL_CHECKER_H_

// util-models
#include "supply-model.pb.h"


namespace invasion::controllers {
using namespace util_models;

class SupplyTypeModelChecker {
public:

// StaticSupply
// StaticSupplyType getType(supplyId, session) {
// 	auto& supplies = session.getSupplies();

// 	type = null;
// 	for(supply : supplies) {
// 		if(supply->getId() == supplyId) {
// 			type = supply->getType();
// 		}
// 	}

// 	return type;
// }

};

} // namespace invasion::controllers


#endif // SUPPLY_TYPE_MODEL_CHECKER_H_