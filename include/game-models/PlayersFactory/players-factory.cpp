#include <memory>
#include <stdexcept>

#include "players-factory.h"

// game-models
#include "game-models/Player/player.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Stormtrooper/stormtrooper.h"
#include "game-models/Sentinel/sentinel.h"
#include "game-models/Support/support.h"
#include "game-models/Medic/medic.h"


namespace invasion::game_models {
	
std::shared_ptr<Player> PlayersFactory::createPlayer(const Vector2D position, 
													 const int playerId,
													 const PlayerTeamId teamId,
													 const PlayerSpecialization specialization) {
	switch (specialization) {
		case PlayerSpecialization::Stormtrooper: {
			return std::make_shared<Stormtrooper>(position, playerId, teamId);
		}
		case PlayerSpecialization::Sentinel: {
			return std::make_shared<Sentinel>(position, playerId, teamId);
		}
		case PlayerSpecialization::Support: {
			return std::make_shared<Support>(position, playerId, teamId);
		}
		case PlayerSpecialization::Medic: {
			return std::make_shared<Medic>(position, playerId, teamId);
		}
		default: {
			throw std::runtime_error("PlayersFactory could not create player with provided specialization"); 
		}
	}
}

} // namespace invasion::game_models
