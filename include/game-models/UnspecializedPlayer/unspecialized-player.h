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

	~UnspecializedPlayer() {
		std::cout << "UnspecializedPlayer dtor called" << std::endl;
	}
};


} // namespace invasion::game_models



#endif // STORMTROOPER_H_
