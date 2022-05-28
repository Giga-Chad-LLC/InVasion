#ifndef WEAPON_H_
#define WEAPON_H_

#include <chrono>
#include <memory>
#include <iostream>
#include <atomic>
#include <mutex>

// game-models
#include "game-models/Bullet/bullet.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player-team-id-enum.h"

namespace invasion::game_models {


class Weapon {
public:
	explicit Weapon(int playerId, PlayerTeamId teamId, int ammo, int damage);

	std::shared_ptr<Bullet> shoot(Vector2D playerPosition, int bulletId);
	bool isAbleToShoot() const;
	bool reload();
	bool isReloading() const;
	void setDirection(const Vector2D& dir);
	void reset();

	void addAmmo(int ammo);

	// getters
	int getLeftMagazine() const;
	int getLeftAmmo() const;
	int getInitialAmmo() const;
	Vector2D getDirection() const;

private:
	static const long long RELOAD_DURATION_MS;
	static const int MAGAZINE;

	int m_leftMagazine;
	int m_leftAmmo;
	const int m_initialAmmo;
	const int m_damage;
	
	Vector2D m_direction;
	const int m_playerId;
	const PlayerTeamId m_playerTeamId;

	mutable std::atomic_bool m_isReloading;
	mutable std::mutex mtx_reload;
};


} // namespace invasion::game_models


#endif // WEAPON_H_
