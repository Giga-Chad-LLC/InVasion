#include <utility>
#include <cassert>
#include <chrono>
#include <memory>
#include <algorithm>

#include "weapon.h"

// game-models
#include "game-models/Bullet/bullet.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player-team-id-enum.h"
// utils
#include "utils/TimeUtilities/time-utilities.h"


namespace invasion::game_models {
// following data was taken from M16A4 shooting stats (Call of Duty)
const long long Weapon::RELOAD_DURATION_MS = 2100;
const long long Weapon::DELAY_BETWEEN_SHOTS_MS = 74;
const int Weapon::MAGAZINE = 1000;


Weapon::Weapon(int playerId, PlayerTeamId teamId, int ammo, int damage) 
	: m_leftMagazine(Weapon::MAGAZINE),
	  m_leftAmmo(ammo),
	  m_initialAmmo(ammo),
	  m_damage(damage),
	  m_reloadingStartTimestamp_ms(0),
	//   m_lastShotTimestamp_ms(0),
	  m_direction(1.0, 0.0),
	  m_playerId(playerId),
	  m_playerTeamId(teamId) {}


// Weapon::shoot may be called only if gun is able to shoot
std::shared_ptr<Bullet> Weapon::shoot(const Vector2D playerPosition, const int bulletId) {
	assert(isAbleToShoot());
	
	m_leftMagazine--; 
	// m_lastShotTimestamp_ms = utils::TimeUtilities::getCurrentTime_ms();

	std::shared_ptr<Bullet> bullet_ptr = std::make_shared<Bullet>(
		std::move(playerPosition), 
		bulletId, 
		m_playerId,
		m_playerTeamId,
		m_damage
	);

	bullet_ptr->setMovingForce(m_direction);
	bullet_ptr->setMovingState(true);

	return bullet_ptr;
}


bool Weapon::isAbleToShoot() const {
	const long long now = utils::TimeUtilities::getCurrentTime_ms();
	return (
		m_leftMagazine > 0 && 
		// now > m_lastShotTimestamp_ms + Weapon::DELAY_BETWEEN_SHOTS_MS &&
		now > m_reloadingStartTimestamp_ms + Weapon::RELOAD_DURATION_MS
	);
}


void Weapon::reload() {
	if(m_leftAmmo == 0 || m_leftMagazine == Weapon::MAGAZINE) {
		return;
	}

	m_reloadingStartTimestamp_ms = utils::TimeUtilities::getCurrentTime_ms();
	
	if(m_leftAmmo < Weapon::MAGAZINE - m_leftMagazine) {
		m_leftMagazine += m_leftAmmo;
		m_leftAmmo = 0;
	}
	else {
		m_leftAmmo -= (Weapon::MAGAZINE - m_leftMagazine);
		m_leftMagazine = Weapon::MAGAZINE;
	}
}


void Weapon::setDirection(const Vector2D& dir) {
	m_direction = std::move(dir.normalize());
}


bool Weapon::isReloading() const {
	const long long now = utils::TimeUtilities::getCurrentTime_ms();
	return now < m_reloadingStartTimestamp_ms + Weapon::RELOAD_DURATION_MS;
}


void Weapon::reset() {
	m_leftMagazine = Weapon::MAGAZINE;
	m_leftAmmo = m_initialAmmo;
	m_reloadingStartTimestamp_ms = 0;
	// m_lastShotTimestamp_ms = 0;
	m_direction = std::move(Vector2D(1.0, 0.0));
}


void Weapon::addAmmo(int ammo) {
	assert(ammo >= 0);
	m_leftAmmo = std::min(m_leftAmmo + ammo, m_initialAmmo);
}


// getters
int Weapon::getLeftMagazine() const {
	return m_leftMagazine;
}


int Weapon::getLeftAmmo() const {
	return m_leftAmmo;
}

int Weapon::getInitialAmmo() const {
	return m_initialAmmo;
}

long long Weapon::getReloadingStartTimestamp_ms() const {
	return m_reloadingStartTimestamp_ms;
}


Vector2D Weapon::getDirection() const {
	return m_direction;
}


} // namespace invasion::game_models
