#include <utility>
#include "player.h"

#include "player-team-id-enum.h"
#include "player-life-state.h"
#include "player-game-session-stats.h"
#include "player-specialization-enum.h"
// game-models
#include "game-models/KinematicObject/kinematic-object.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Weapon/weapon.h"


namespace invasion::game_models {
	
Player::Player(
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
		bool hasAbility
	) :
		KinematicObject(shapeColliderSize, hitboxColliderSize, position, mass, maxSpeed), 
		m_id(playerId),
		m_hasAbility(hasAbility),
		m_specialization(specialization),
		m_teamId(teamId),
		m_lifeState(initialHitPoints),
		m_weapon(playerId, teamId, initialAmmo, initialMagazine, damage) {}


int Player::getId() const {
	return m_id;
}


PlayerSpecialization Player::getSpecialization() const {
	return m_specialization;
}


PlayerTeamId Player::getTeamId() const {
	return m_teamId;
}


PlayerLifeState& Player::getLifeState() {
	return m_lifeState;
}


PlayerGameSessionStats& Player::getGameSessionStats() {
	return m_gameSessionStats;
}


Weapon& Player::getWeapon() {
	return m_weapon;
}


void Player::respawn(Vector2D position) {
	m_lifeState.reset();
	m_weapon.reset();
	
	this->setPosition(std::move(position));
	// cleaning applied forces
	this->setMovingForce(Vector2D::ZERO);
	this->setMovingState(false);
	this->setResultForce(Vector2D::ZERO);
	this->setVelocity(Vector2D::ZERO);
}


bool Player::hasAbility() const {
	return m_hasAbility;
}


} // namespace invasion::game_models
