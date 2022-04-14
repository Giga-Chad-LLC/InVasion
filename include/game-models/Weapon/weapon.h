#ifndef WEAPON_H_
#define WEAPON_H_

#include <chrono>
#include <memory>

#include "game-models/Bullet/bullet.h"
#include "game-models/Vector2D/vector2d.h"

namespace invasion::game_models {
	

struct Weapon {
	Weapon(int playerId, int ammo, double damage);

	std::shared_ptr<Bullet> shoot(Vector2D playerPos, int bulletId);
	bool isAbleToShoot() const;
	void reload();
	bool isReloading() const;
	void setDirection(const Vector2D& dir);
	void reset();

	// getters
	int getLeftMagazine() const;
	int getLeftAmmo() const;
	long long getReloadingStartTimestamp_ms() const;
	Vector2D getDirection() const;

private:
	static long long getCurrentTime_ms();

private:
	static const long long RELOAD_DURATION_MS;
	static const long long DELAY_BETWEEN_SHOTS_MS;
	static const int MAGAZINE;

	int m_leftMagazine;
	int m_leftAmmo;
	const int m_initialAmmo;
	const double m_damage;

	long long m_reloadingStartTimestamp_ms;
	// long long m_lastShotTimestamp_ms;

	
	Vector2D m_direction;
	const int m_playerId;
};


} // namespace invasion::game_models


#endif // WEAPON_H_