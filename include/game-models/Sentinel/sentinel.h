#ifndef SENTINEL_H_
#define SENTINEL_H_

// game-models
#include "game-models/Player/player.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {
	

class Sentinel : public Player {
public:
	explicit Sentinel(Vector2D position, int playerId, PlayerTeamId teamId);

	inline static const double MASS = 80.0;
	inline static const double MAX_SPEED = 90.0;
	inline static const int INITIAL_HIT_POINTS = 200;
	inline static const double INITIAL_AMMO = 180;
	inline static const double DAMAGE = 18;
};


} // namespace invasion::game_models



#endif // SENTINEL_H_
