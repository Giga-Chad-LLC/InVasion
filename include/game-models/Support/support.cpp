#include <iostream>

#include "support.h"

// game-models
#include "game-models/AbilityEndowedPlayer/ability-endowed-player.h"
#include "game-models/Player/player.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"

namespace invasion::game_models {
	
Support::Support(Vector2D position, int playerId, PlayerTeamId teamId) 
	: AbilityEndowedPlayer(
		Player::SHAPE_COLLIDER_SIZE,
		Player::HITBOX_COLLIDER_SIZE,
		position,
		Player::MASS,
		Player::MAX_SPEED,
		playerId,
		PlayerSpecialization::Support,
		teamId,
		Player::INITIAL_HIT_POINTS,
		Support::INITIAL_AMMO,
		Player::DAMAGE,
		Support::ABILITY_COOL_DOWN_MS
	) {}

std::shared_ptr<StaticSupply> Support::applyAbility(const int supplyId) {
	std::cout << "applying ability of Support class" << std::endl;
	this->setCoolDown();
	const int playerId = this->getId();
	return std::make_shared<AmmoCrate>(m_position, supplyId, playerId);
}

} // namespace invasion::game_models
