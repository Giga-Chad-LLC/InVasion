#include <utility>

#include "object.h"


namespace invasion::game_models {

Object::Object(Vector2D collider_size, Vector2D initial_pos)
	: m_collider(std::move(collider_size)),
	  m_position(std::move(initial_pos))
	{}

Vector2D Object::getPosition() const {
	return m_position;
}

void Object::setPosition(Vector2D pos) {
	m_position = std::move(pos);
}

} // namespace invasion::game_models