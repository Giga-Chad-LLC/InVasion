#include "player.h"

#include "game-models/Vector2D/vector2d.h"

namespace invasion::game_models {
const double Player::MAX_SPEED = 100;
const double Player::MASS = 60.0;
const int Player::INITIAL_AMMO = 180;
const double Player::DAMAGE = 15.0;
const Vector2D Player::COLLIDER_SIZE(20, 20);

	
Player::Player(Vector2D initial_pos, const int playerId, const Player::TeamId teamId)
	: KinematicObject(
		Player::COLLIDER_SIZE, 
		std::move(initial_pos), 
		Player::MASS,
		Player::MAX_SPEED
	), 
	m_id(playerId),
	m_teamId(teamId),
	m_weapon(playerId, Player::INITIAL_AMMO, Player::DAMAGE) {}


int Player::getId() const {
	return m_id;
}


Player::TeamId Player::getTeamId() const {
	return m_teamId;
}


Weapon& Player::getWeapon() {
	return m_weapon;
}

} // namespace invasion::game_models
