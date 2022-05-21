#ifndef MEDIC_H_
#define MEDIC_H_

// game-models
#include "game-models/AbilityEndowedPlayer/ability-endowed-player.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"

namespace invasion::game_models {
	
class Medic : public AbilityEndowedPlayer {
public:
	explicit Medic(Vector2D position, int playerId, PlayerTeamId teamId);
	void applyAbility() override;

	inline static const double DAMAGE = 12.0;
	inline static const double INITIAL_HIT_POINTS = 150.0;
private:
	inline static long long ABILITY_COOL_DOWN_MS = 10 * 1000; // 10 sec
};

} // namespace invasion::game_models



#endif // MEDIC_H_
