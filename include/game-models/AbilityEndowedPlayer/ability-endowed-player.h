#ifndef ABILITY_ENDOWED_PLAYER_H_
#define ABILITY_ENDOWED_PLAYER_H_

#include <memory>

// game-models
#include "game-models/Player/player.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/StaticSupply/static-supply.h"


namespace invasion::game_models {


class AbilityEndowedPlayer : public Player {
public:
	explicit AbilityEndowedPlayer(
		Vector2D shapeColliderSize,
		Vector2D hitboxColliderSize,
		Vector2D position,
		double mass,
		double maxSpeed,
		int playerId,
		PlayerSpecialization specialization,
		PlayerTeamId teamId,
		int initialHitPoints,
		int initialAmmo,
		int initialMagazine,
		int damage,
		long long abilityCoolDown_ms
	);

	bool isAbleToApplyAbility() const;

	// TODO: change return type to the one specified in UML-board
	virtual std::shared_ptr<StaticSupply> applyAbility(int supplyId) = 0;

protected:
	void setCoolDown();

private:
	long long m_lastAbilityUseTimestamp_ms;
	const long long m_abilityCoolDown_ms;
};

	
} // namespace invasion::game_models


#endif // ABILITY_ENDOWED_PLAYER_H_
