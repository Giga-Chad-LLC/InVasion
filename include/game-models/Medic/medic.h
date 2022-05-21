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

	~Medic() {
		std::cout << "Medic dtor called" << std::endl;
	}

	inline static const int DAMAGE = 12;
	inline static const int INITIAL_HIT_POINTS = 150;
private:
	inline static long long ABILITY_COOL_DOWN_MS = 10 * 1000; // 10 sec
};

} // namespace invasion::game_models



#endif // MEDIC_H_
