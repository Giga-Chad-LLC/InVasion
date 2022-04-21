#ifndef PLAYER_H_
#define PLAYER_H_


#include "player-team-id-enum.h"
// game-models
#include "game-models/KinematicObject/kinematic-object.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Weapon/weapon.h"


namespace invasion::game_models {
	

struct Player : KinematicObject {
	static const double MAX_SPEED;
	static const double MASS;
	static const int INITIAL_AMMO;
	static const double DAMAGE;
	static const double INITIAL_HIT_POINTS;

	explicit Player(Vector2D initial_pos, int playerId, PlayerTeamId teamId);

	int getId() const;
	PlayerTeamId getTeamId() const;
	double getHitPoints() const;
	void applyDamage(double damage);
	bool isInDeadState() const;
	void respawn(Vector2D position);
	Weapon& getWeapon();

private:
	static const Vector2D COLLIDER_SIZE;
	const int m_id;
	const PlayerTeamId m_teamId;
	double m_hitPoints;
	bool m_isDead;
	Weapon m_weapon;
};


} // namespace invasion::game_models


#endif // PLAYER_H_
