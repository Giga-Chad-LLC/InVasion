#ifndef UNSPECIALIZAED_PLAYER_H_
#define UNSPECIALIZAED_PLAYER_H_

#include <iostream>

// game-models
#include "game-models/Player/player.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"

namespace invasion::game_models {
	

class UnspecializedPlayer : public Player {
public:
	explicit UnspecializedPlayer(Vector2D position, int playerId, PlayerTeamId teamId);

	inline static const int INITIAL_HIT_POINTS = 100;
	inline static int INITIAL_AMMO = 30;
	inline static const int INITIAL_MAGAZINE = 30;
	inline static const int DAMAGE = 15;
};


} // namespace invasion::game_models



#endif // STORMTROOPER_H_
