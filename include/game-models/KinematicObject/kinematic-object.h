#ifndef KINEMATIC_OBJECT_H_
#define KINEMATIC_OBJECT_H_


#include "game-models/Vector2D/vector2d.h"
#include "game-models/Object/object.h"



namespace invasion::game_models {

struct KinematicObject : Object {

	explicit KinematicObject(Vector2D collider_size, Vector2D initial_pos);

	Vector2D getVelocity() const;
	void setVelocity(Vector2D velocity);

	Vector2D intentMove(double delta);

protected:
	Vector2D m_velocity;
	Vector2D m_acceleration;
	bool m_moving;
};


} // namespace invasion::game_models



#endif // KINEMATIC_OBJECT_H_