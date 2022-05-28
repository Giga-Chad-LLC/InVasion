#include <utility>

#include "object.h"


namespace invasion::game_models {

Object::Object(Vector2D shapeColliderSize, Vector2D hitboxColliderSize, Vector2D initialPosition)
	: m_shapeCollider(std::move(shapeColliderSize)),
	  m_hitboxCollider(std::move(hitboxColliderSize)),
	  m_position(std::move(initialPosition))
	{}

Vector2D Object::getPosition() const {
	return m_position;
}

void Object::setPosition(Vector2D position) {
	m_position = std::move(position);
}

bool Object::collidesWithHitbox(const Object* const other, const Vector2D& offset) const {
	return m_hitboxCollider.collidesWith(m_position + offset, other->m_shapeCollider, other->m_position);
}

bool Object::collidesWithShape(const Object* const other) const {
	return m_shapeCollider.collidesWith(m_position, other->m_shapeCollider, other->m_position);
}

double Object::calculateClosestDistanceBetween(const Object* const other) const {
	return m_shapeCollider.calculateClosestDistanceBetween(m_position, other->m_shapeCollider, other->m_position);
}

} // namespace invasion::game_models