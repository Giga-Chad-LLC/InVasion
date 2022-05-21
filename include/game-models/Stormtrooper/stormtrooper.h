#ifndef STORMTROOPER_H_
#define STORMTROOPER_H_

#include <iostream>

// game-models
#include "game-models/Player/player.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"

namespace invasion::game_models {
	

class Stormtrooper : public Player {
public:
	explicit Stormtrooper(Vector2D position, int playerId, PlayerTeamId teamId);

	inline static const double MAX_SPEED = 110.0;
	inline static const double INITIAL_HIT_POINTS = 120;
	inline static const int INITIAL_AMMO = 120;
	inline static const double DAMAGE = 20;
};


} // namespace invasion::game_models



#endif // STORMTROOPER_H_
