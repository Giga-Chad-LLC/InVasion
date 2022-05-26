#ifndef SUPPLY_TYPE_MODEL_CHECKER_H_
#define SUPPLY_TYPE_MODEL_CHECKER_H_


// game-models
#include "game-models/GameSession/game-session.h"


namespace invasion::controllers {
using namespace game_models;

class SupplyTypeModelChecker {
public:

	bool isAmmoCrateType(int supplyId, GameSession& session) const;

	bool isAidKitType(int supplyId, GameSession& session) const;
};

} // namespace invasion::controllers


#endif // SUPPLY_TYPE_MODEL_CHECKER_H_