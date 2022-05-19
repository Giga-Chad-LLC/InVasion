#include "sentinel.h"

// game-models
#include "game-models/Player/player.h"
#include "game-models/Player/player-specialization-type-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {
	
Sentinel::Sentinel(Vector2D position, int playerId, PlayerTeamId teamId)
	: Player(
		Player::SHAPE_COLLIDER_SIZE,
		Player::HITBOX_COLLIDER_SIZE,
		position,
		Sentinel::MASS,
		Sentinel::MAX_SPEED,
		playerId,
		PlayerSpecializationType::Sentinel,
		teamId,
		Sentinel::INITIAL_HIT_POINTS,
		Sentinel::INITIAL_AMMO,
		Sentinel::DAMAGE
	) {}

} // namespace invasion::game_models
