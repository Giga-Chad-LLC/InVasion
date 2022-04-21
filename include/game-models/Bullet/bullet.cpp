#include "bullet.h"

// game-models
#include "game-models/KinematicObject/kinematic-object.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player-team-id-enum.h"


namespace invasion::game_models {
const double Bullet::MAX_SPEED = Vector2D(500, 0).magnitude();
const double Bullet::MASS = 0.04;
const Vector2D Bullet::COLLIDER_SIZE(1, 1);



Bullet::Bullet(Vector2D initialPos, int bulletId, int playerId, PlayerTeamId teamId, double damage)
	: KinematicObject(
		Bullet::COLLIDER_SIZE, 
		std::move(initialPos), 
		Bullet::MASS,
		Bullet::MAX_SPEED
	), 
	m_id(bulletId),
	m_playerId(playerId),
	m_playerTeamId(teamId),
	m_damage(damage),
	m_isCrushed(false) {}


int Bullet::getId() const {
	return m_id;
}

int Bullet::getPlayerId() const {
	return m_playerId;
}

PlayerTeamId Bullet::getPlayerTeamId() const {
	return m_playerTeamId;
}

double Bullet::getDamage() const {
	return m_damage;
}


bool Bullet::isInCrushedState() const {
	return m_isCrushed; 
}

void Bullet::setCrushedState(const bool state) {
	m_isCrushed = state;
}


} // namespace invasion::game_models
