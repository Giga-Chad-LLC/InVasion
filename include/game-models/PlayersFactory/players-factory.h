#ifndef PLAYERS_FACTORY_H_
#define PLAYERS_FACTORY_H_

#include <memory>

// game-models
#include "game-models/Player/player.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {
	
class PlayersFactory {
public:
	static std::shared_ptr<Player> createPlayer(Vector2D position, 
										 int playerId,
										 PlayerTeamId teamId,
										 PlayerSpecialization specialization);
};

} // namespace invasion::game_models



#endif // PLAYERS_FACTORY_H_