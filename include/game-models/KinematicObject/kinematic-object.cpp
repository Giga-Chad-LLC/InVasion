#include <utility>

#include "kinematic-object.h"


namespace invasion::game_models {
	

KinematicObject::KinematicObject(Vector2D collider_size, Vector2D initial_pos, double mass)
	: Object(std::move(collider_size), std::move(initial_pos)),
	  m_mass(mass),
	  m_moving(false) {}


Vector2D KinematicObject::getVelocity() const {
	return m_velocity;
}

Vector2D KinematicObject::intentMove(double dt) {
	m_velocity += m_acceleration * dt;
	return m_position + m_velocity * dt;
}

Vector2D KinematicObject::getForce() const {
	return m_force;
}

void KinematicObject::setForce(Vector2D force) {
	m_force = std::move(force);
	m_acceleration = m_force / m_mass;
}

Vector2D KinematicObject::getAcceleration() const {
	return m_acceleration;
}


} // namespace invasion::game_models
