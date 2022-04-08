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

	enum class TeamId {
		FirstTeam,
		SecondTeam,
	};

	explicit Player(Vector2D initial_pos, int playerId, Player::TeamId teamId);

	int getId() const;
	Player::TeamId Player::getTeamId() const;
	Weapon& getWeapon();

private:
	static const Vector2D COLLIDER_SIZE;
	const int m_id;
	const Player::TeamId m_teamId;
	Weapon m_weapon;
};


} // namespace invasion::game_models


#endif // PLAYER_H_