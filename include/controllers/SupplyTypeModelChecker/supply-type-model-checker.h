#ifndef SUPPLY_TYPE_MODEL_CHECKER_H_
#define SUPPLY_TYPE_MODEL_CHECKER_H_

// util-models
#include "supply-model.pb.h"


namespace invasion::controllers {
using namespace util_models;

class SupplyTypeModelChecker {
public:
	bool isAidKit(const SupplyModel& supply) const;
	bool isAmmoCrate(const SupplyModel& supply) const;
};

} // namespace invasion::controllers


#endif // SUPPLY_TYPE_MODEL_CHECKER_H_