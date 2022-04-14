#ifndef PLAYER_H_
#define PLAYER_H_

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

	enum class TeamId {
		FirstTeam,
		SecondTeam,
	};

	explicit Player(Vector2D initial_pos, int playerId, Player::TeamId teamId);

	int getId() const;
	Player::TeamId Player::getTeamId() const;
	double getHitPoints() const;
	void applyDamage(double damage);
	bool isInDeadState() const;
	void respawn(Vector2D position);
	Weapon& getWeapon();

private:
	static const Vector2D COLLIDER_SIZE;
	const int m_id;
	const Player::TeamId m_teamId;
	double m_hitPoints;
	bool m_isDead;
	Weapon m_weapon;
};


} // namespace invasion::game_models


#endif // PLAYER_H_
