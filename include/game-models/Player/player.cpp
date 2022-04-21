#include <utility>
#include "player.h"

#include "player-team-id-enum.h"
// game-models
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {
const double Player::MAX_SPEED = 100;
const double Player::MASS = 60.0;
const int Player::INITIAL_AMMO = 180;
const double Player::DAMAGE = 15.0;
const double Player::INITIAL_HIT_POINTS = 100.0;
const Vector2D Player::COLLIDER_SIZE(12, 6);

	
Player::Player(Vector2D initial_pos, const int playerId, const PlayerTeamId teamId)
	: KinematicObject(
		Player::COLLIDER_SIZE, 
		std::move(initial_pos), 
		Player::MASS,
		Player::MAX_SPEED
	), 
	m_id(playerId),
	m_teamId(teamId),
	m_hitPoints(Player::INITIAL_HIT_POINTS),
	m_isDead(false),
	m_weapon(playerId, teamId, Player::INITIAL_AMMO, Player::DAMAGE) {}


int Player::getId() const {
	return m_id;
}


PlayerTeamId Player::getTeamId() const {
	return m_teamId;
}


double Player::getHitPoints() const {
	return m_hitPoints;
}


void Player::applyDamage(const double damage) {
	if(m_hitPoints <= damage) {
		m_hitPoints = 0.0;
		m_isDead = true;
	}
	else {
		m_hitPoints -= damage;
	}
}


bool Player::isInDeadState() const {
	return m_isDead;
}


void Player::respawn(Vector2D position) {
	m_hitPoints = Player::INITIAL_HIT_POINTS;
	m_isDead = false;
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
