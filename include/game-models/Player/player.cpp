#include <utility>
#include "player.h"

#include "player-team-id-enum.h"
#include "player-life-state.h"
// game-models
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {
const double Player::MAX_SPEED = 100;
const double Player::MASS = 60.0;
const int Player::INITIAL_AMMO = 180;
const double Player::DAMAGE = 15.0;
const double Player::INITIAL_HIT_POINTS = 100.0;
const Vector2D Player::HITBOX_POSITION_OFFSET(0, -8);

const Vector2D Player::SHAPE_COLLIDER_SIZE(12, 6);
const Vector2D Player::HITBOX_COLLIDER_SIZE(10, 14.5);


	
Player::Player(Vector2D initialPosition, const int playerId, const PlayerTeamId teamId)
	: KinematicObject(
		Player::SHAPE_COLLIDER_SIZE,
		Player::HITBOX_COLLIDER_SIZE,
		std::move(initialPosition),
		Player::MASS,
		Player::MAX_SPEED
	), 
	m_id(playerId),
	m_teamId(teamId),
	m_lifeState(Player::INITIAL_HIT_POINTS),
	m_weapon(playerId, teamId, Player::INITIAL_AMMO, Player::DAMAGE) {}


int Player::getId() const {
	return m_id;
}


PlayerTeamId Player::getTeamId() const {
	return m_teamId;
}


PlayerLifeState& Player::getLifeState() {
	return m_lifeState;
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


Weapon& Player::getWeapon() {
	return m_weapon;
}

} // namespace invasion::game_models
