#ifndef MEDIC_H_
#define MEDIC_H_

#include <memory>

// game-models
#include "game-models/AbilityEndowedPlayer/ability-endowed-player.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/StaticSupply/static-supply.h"

namespace invasion::game_models {
	
class Medic : public AbilityEndowedPlayer {
public:
	explicit Medic(Vector2D position, int playerId, PlayerTeamId teamId);
	std::shared_ptr<StaticSupply> applyAbility(int supplyId) override;

	inline static const int INITIAL_HIT_POINTS = 110;
	inline static int INITIAL_AMMO = 25;
	inline static const int INITIAL_MAGAZINE = 25;
	inline static const int DAMAGE = 18;
private:
	inline static long long ABILITY_COOL_DOWN_MS = 30 * 1000; // 30 sec
};

} // namespace invasion::game_models



#endif // MEDIC_H_
