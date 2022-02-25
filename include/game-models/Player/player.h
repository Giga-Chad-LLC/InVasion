#ifndef PLAYER_H_
#define PLAYER_H_

#include "game-models/KinematicObject/kinematic-object.h"
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {
	

struct Player : KinematicObject {
	explicit Player(Vector2D initial_pos);

	static const double MAX_SPEED;
	static const double MASS;
private:
	static const Vector2D COLLIDER_SIZE;
};


} // namespace invasion::game_models


#endif // PLAYER_H_