#ifndef BULLET_H_
#define BULLET_H_

#include "game-models/KinematicObject/kinematic-object.h"
#include "game-models/Vector2D/vector2d.h"

namespace invasion::game_models {
	

struct Bullet : KinematicObject {
	Bullet(Vector2D initialPos, int bulletId, int playerId, double damage);

	int getId() const;
	int getPlayerId() const;

	static const double MAX_SPEED;
	static const double MASS;
private:
	static const Vector2D COLLIDER_SIZE;

	const int m_id;
	const int m_playerId;
	const double m_damage;
};


} // namespace invasion::game_models



#endif // BULLET_H_