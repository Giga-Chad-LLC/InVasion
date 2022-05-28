#include <utility>
#include <cassert>
#include <chrono>
#include <memory>
#include <algorithm>
#include <thread>
#include <mutex>

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
const int Weapon::MAGAZINE = 30;


Weapon::Weapon(int playerId, PlayerTeamId teamId, int ammo, int damage) 
	: m_leftMagazine(Weapon::MAGAZINE),
	  m_leftAmmo(ammo),
	  m_initialAmmo(ammo),
	  m_damage(damage),
	  m_direction(1.0, 0.0),
	  m_playerId(playerId),
	  m_playerTeamId(teamId),
	  m_isReloading(false) {}


// Weapon::shoot may be called only if gun is able to shoot
std::shared_ptr<Bullet> Weapon::shoot(const Vector2D playerPosition, const int bulletId) {
	assert(isAbleToShoot());
	
	m_leftMagazine--;

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
	std::unique_lock ul{ mtx_reload };
	return (
		m_leftMagazine > 0 && !m_isReloading.load()
	);
}


bool Weapon::reload() {
	std::unique_lock ul{ mtx_reload };
	
	if (m_leftAmmo == 0 || m_leftMagazine == Weapon::MAGAZINE || m_isReloading.load()) {
		return false;
	}
	
	m_isReloading.store(true);
	ul.unlock();

	// reloading/sleeping
	std::this_thread::sleep_for(std::chrono::milliseconds(Weapon::RELOAD_DURATION_MS));
	
	if(m_leftAmmo < Weapon::MAGAZINE - m_leftMagazine) {
		m_leftMagazine += m_leftAmmo;
		m_leftAmmo = 0;
	}
	else {
		m_leftAmmo -= (Weapon::MAGAZINE - m_leftMagazine);
		m_leftMagazine = Weapon::MAGAZINE;
	}

	ul.lock();
	m_isReloading.store(false);
	
	return true;
}


void Weapon::setDirection(const Vector2D& dir) {
	m_direction = std::move(dir.normalize());
}


bool Weapon::isReloading() const {
	std::unique_lock ul{ mtx_reload };
	return m_isReloading.load();
}


void Weapon::reset() {
	std::unique_lock ul{ mtx_reload };

	m_leftMagazine = Weapon::MAGAZINE;
	m_leftAmmo = m_initialAmmo;
	m_isReloading.store(false);
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

Vector2D Weapon::getDirection() const {
	return m_direction;
}


} // namespace invasion::game_models
