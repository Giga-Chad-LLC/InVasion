#ifndef OBJECT_H_
#define OBJECT_H_

#include "game-models/Vector2D/vector2d.h"
#include "game-models/BoxCollider/box-collider.h"



namespace invasion::game_models {

class Object {
public:
	explicit Object(Vector2D shapeColliderSize, Vector2D hitboxColliderSize, Vector2D initialPosition);

	Vector2D getPosition() const;
	void setPosition(Vector2D position);

	bool collidesWithHitbox(const Object* const other, const Vector2D& offset = Vector2D::ZERO) const;
	bool collidesWithShape(const Object* const other) const;

	double calculateClosestDistanceBetween(const Object* const other) const;

protected:
	const BoxCollider m_shapeCollider;
	const BoxCollider m_hitboxCollider;
	Vector2D m_position;
};


} // namespace invasion::game_models



#endif // OBJECT_H_
