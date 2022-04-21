#ifndef OBJECT_H_
#define OBJECT_H_

#include "game-models/Vector2D/vector2d.h"
#include "game-models/BoxCollider/box-collider.h"



namespace invasion::game_models {

struct Object {
	explicit Object(Vector2D shapeColliderSize, Vector2D hitboxColliderSize, Vector2D initialPos);

	Vector2D getPosition() const;
	void setPosition(Vector2D pos);

	bool collidesWithHitbox(const Object* const other) const;
	bool collidesWithShape(const Object* const other) const;

protected:
	const BoxCollider m_shapeCollider;
	const BoxCollider m_hitboxCollider;
	Vector2D m_position;
};


} // namespace invasion::game_models



#endif // OBJECT_H_
