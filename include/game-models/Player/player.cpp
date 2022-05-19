#include <utility>
#include "player.h"

#include "player-team-id-enum.h"
#include "player-life-state.h"
#include "player-game-session-stats.h"
// game-models
#include "game-models/KinematicObject/kinematic-object.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Weapon/weapon.h"


namespace invasion::game_models {
// const double Player::MAX_SPEED = 100.0;
// const double Player::MASS = 60.0;
// const int Player::INITIAL_AMMO = 180;
// const int Player::DAMAGE = 15;
// const int Player::INITIAL_HIT_POINTS = 100;
// const Vector2D Player::HITBOX_POSITION_OFFSET(0, -8);

// const Vector2D Player::SHAPE_COLLIDER_SIZE(12, 6);
// const Vector2D Player::HITBOX_COLLIDER_SIZE(10, 14.5);


	
Player::Player(
		Vector2D shapeColliderSize,
		Vector2D hitboxColliderSize,
		Vector2D position,
		double mass,
		double maxSpeed,
		int playerId,
		PlayerTeamId teamId,
		int initialHitPoints,
		int initialAmmo,
		double damage
	) :
		KinematicObject(shapeColliderSize, hitboxColliderSize, position, mass, maxSpeed), 
		m_id(playerId),
		m_teamId(teamId),
		m_lifeState(initialHitPoints),
		m_weapon(playerId, teamId, initialAmmo, damage) {}


int Player::getId() const {
	return m_id;
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


} // namespace invasion::game_models
