#include "ability-endowed-player.h"

// game-models
#include "game-models/Player/player.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"
//utils
#include "utils/TimeUtilities/time-utilities.h"


namespace invasion::game_models {
	
AbilityEndowedPlayer::AbilityEndowedPlayer(
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
	int damage,
	long long abilityCoolDown_ms
) :
	Player(
		shapeColliderSize,
		hitboxColliderSize,
		position,
		mass,
		maxSpeed,
		playerId,
		specialization,
		teamId,
		initialHitPoints,
		initialAmmo,
		damage
	),
	m_lastAbilityUseTimestamp_ms(utils::TimeUtilities::getCurrentTime_ms()),
	m_abilityCoolDown_ms(abilityCoolDown_ms) {}


bool AbilityEndowedPlayer::isAbleToApplyAbility() const {
	const long long currentTime_ms = utils::TimeUtilities::getCurrentTime_ms();
	return (currentTime_ms - m_lastAbilityUseTimestamp_ms >= m_abilityCoolDown_ms);
}


} // namespace invasion::game_models
