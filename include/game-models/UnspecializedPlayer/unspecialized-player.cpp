#include "unspecialized-player.h"

// game-models
#include "game-models/Player/player.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {
	

UnspecializedPlayer::UnspecializedPlayer(Vector2D position, int playerId, PlayerTeamId teamId)
	: Player(
		Player::SHAPE_COLLIDER_SIZE,
		Player::HITBOX_COLLIDER_SIZE,
		position,
		Player::MASS,
		Player::MAX_SPEED,
		playerId,
		PlayerSpecialization::UNDEFINED,
		teamId,
		Player::INITIAL_HIT_POINTS,
		Player::INITIAL_AMMO,
		Player::DAMAGE
	) {
		// setting unactive state
		this->getLifeState().setActiveState(false);
	};


} // namespace invasion::game_models
