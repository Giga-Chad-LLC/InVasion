#include <iostream>

#include "medic.h"

// game-models
#include "game-models/AbilityEndowedPlayer/ability-endowed-player.h"
#include "game-models/Player/player.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"

namespace invasion::game_models {
	
Medic::Medic(Vector2D position, int playerId, PlayerTeamId teamId) 
	: AbilityEndowedPlayer(
		Player::SHAPE_COLLIDER_SIZE,
		Player::HITBOX_COLLIDER_SIZE,
		position,
		Player::MASS,
		Player::MAX_SPEED,
		playerId,
		PlayerSpecialization::Medic,
		teamId,
		Medic::INITIAL_HIT_POINTS,
		Player::INITIAL_AMMO,
		Medic::DAMAGE,
		Medic::ABILITY_COOL_DOWN_MS
	) {}

void Medic::applyAbility() {
	std::cout << "applying ability of Medic class" << std::endl;
}

} // namespace invasion::game_models
