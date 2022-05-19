#include "stormtrooper.h"

// game-models
#include "game-models/Player/player.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {
	

Stormtrooper::Stormtrooper(Vector2D position, int playerId, PlayerTeamId teamId)
	: Player(
		Player::SHAPE_COLLIDER_SIZE,
		Player::HITBOX_COLLIDER_SIZE,
		position,
		Player::MASS,
		Stormtrooper::MAX_SPEED,
		playerId,
		PlayerSpecialization::Stormtrooper,
		teamId,
		Stormtrooper::INITIAL_HIT_POINTS,
		Stormtrooper::INITIAL_AMMO,
		Stormtrooper::DAMAGE
	) {};


} // namespace invasion::game_models
