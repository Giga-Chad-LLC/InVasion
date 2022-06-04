#include <memory>
#include <iostream>

#include "medic.h"

// game-models
#include "game-models/AbilityEndowedPlayer/ability-endowed-player.h"
#include "game-models/Player/player.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/StaticSupply/static-supply.h"
#include "game-models/AidKit/aid-kit.h"

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
		Medic::INITIAL_AMMO,
		Medic::INITIAL_MAGAZINE,
		Medic::DAMAGE,
		Medic::ABILITY_COOL_DOWN_MS
	) {}

std::shared_ptr<StaticSupply> Medic::applyAbility(const int supplyId) {
	std::cout << "applying ability of Medic class" << std::endl;
	this->setCoolDown();
	
	const int playerId = this->getId();
	const PlayerTeamId teamId = this->getTeamId();

	return std::make_shared<AidKit>(m_position, supplyId, playerId, teamId);
}

} // namespace invasion::game_models
