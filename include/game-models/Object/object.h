#ifndef OBJECT_H_
#define OBJECT_H_

#include "game-models/Vector2D/vector2d.h"
#include "game-models/BoxCollider/box-collider.h"



namespace invasion::game_models {

struct Object {
	explicit Object(Vector2D collider_size, Vector2D initial_pos);

	Vector2D getPosition() const;
	void setPosition(Vector2D pos);

	bool collidesWith(const Object* const other) const;

protected:
	const BoxCollider m_collider;
	Vector2D m_position;
};


} // namespace invasion::game_models



#endif // OBJECT_H_
