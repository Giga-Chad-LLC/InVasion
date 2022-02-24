#include <utility>

#include "kinematic-object.h"


namespace invasion::game_models {
	

KinematicObject::KinematicObject(Vector2D collider_size, Vector2D initial_pos)
	: Object(std::move(collider_size), std::move(initial_pos)),
	  m_moving(false) {}


Vector2D KinematicObject::getVelocity() const {
	return m_velocity;
}


void KinematicObject::setVelocity(Vector2D velocity) {
	m_velocity = std::move(velocity);
}


Vector2D KinematicObject::intentMove(double delta) {
	return m_position + delta * m_velocity;
}


} // namespace invasion::game_models
