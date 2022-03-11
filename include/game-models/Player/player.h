#ifndef PLAYER_H_
#define PLAYER_H_

// game-models
#include "game-models/KinematicObject/kinematic-object.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Weapon/weapon.h"


namespace invasion::game_models {
	

struct Player : KinematicObject {
	explicit Player(Vector2D initial_pos, int playerId);

	int getId() const;
	Weapon& getWeapon();

	static const double MAX_SPEED;
	static const double MASS;
	static const int INITIAL_AMMO;
	static const double DAMAGE;
private:
	static const Vector2D COLLIDER_SIZE;
	const int m_id;
	Weapon m_weapon;
};


} // namespace invasion::game_models


#endif // PLAYER_H_