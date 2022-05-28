#ifndef SUPPORT_H_
#define SUPPORT_H_

#include <memory>

// game-models
#include "game-models/AbilityEndowedPlayer/ability-endowed-player.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/StaticSupply/static-supply.h"
#include "game-models/AmmoCrate/ammo-crate.h"


namespace invasion::game_models {
	
class Support : public AbilityEndowedPlayer {
public:
	explicit Support(Vector2D position, int playerId, PlayerTeamId teamId);
	std::shared_ptr<StaticSupply> applyAbility(int supplyId) override;

	inline static int INITIAL_AMMO = 120;
private:
	inline static long long ABILITY_COOL_DOWN_MS = 1 * 1000; // 10 sec
};

} // namespace invasion::game_models



#endif // SUPPORT_H_
