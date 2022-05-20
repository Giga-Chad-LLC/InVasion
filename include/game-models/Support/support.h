#ifndef SUPPORT_H_
#define SUPPORT_H_

// game-models
#include "game-models/AbilityEndowedPlayer/ability-endowed-player.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"

namespace invasion::game_models {
	
class Support : public AbilityEndowedPlayer {
public:
	explicit Support(Vector2D position, int playerId, PlayerTeamId teamId);
	void applyAbility() override;

	inline static int INITIAL_AMMO = 120;
private:
	inline static long long ABILITY_COOL_DOWN_MS = 10 * 1000; // 10 sec
};

} // namespace invasion::game_models



#endif // SUPPORT_H_
