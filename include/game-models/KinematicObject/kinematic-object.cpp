#include <utility>

#include "kinematic-object.h"


namespace invasion::game_models {
	

KinematicObject::KinematicObject(Vector2D shapeColliderSize,
								 Vector2D hitboxColliderSize,
								 Vector2D initialPosition,
								 double mass,
								 double maxSpeed)
	: Object(std::move(shapeColliderSize), std::move(hitboxColliderSize), std::move(initialPosition)),
	  m_mass(mass),
	  m_maxSpeed(maxSpeed),
	  m_isMoving(false) {}


Vector2D KinematicObject::getVelocity() const {
	return m_velocity;
}


void KinematicObject::setVelocity(Vector2D velocity) {
	m_velocity = std::move(velocity);
}


Vector2D KinematicObject::getMovingForce() const {
	return m_movingForce;
}


void KinematicObject::setMovingForce(Vector2D force) {
	m_movingForce = std::move(force);
}


Vector2D KinematicObject::getResultForce() const {
	return m_resultForce;
}


void KinematicObject::setResultForce(Vector2D force) {
	m_resultForce = std::move(force);
	m_acceleration = m_resultForce / m_mass;
}


Vector2D KinematicObject::getAcceleration() const {
	return m_acceleration;
}


Vector2D KinematicObject::intentMove(double dt) const {
	Vector2D velocity = m_velocity + m_acceleration * dt;

	if(velocity.magnitude() > m_maxSpeed) {
		velocity = Vector2D::clampMagnitude(velocity, m_maxSpeed);
	}

	return m_position + velocity * dt;
}


void KinematicObject::makeMove(double dt) {
	m_velocity += m_acceleration * dt;
	
	// clamping velocity to max speed
	if(m_velocity.magnitude() > m_maxSpeed) {
		m_velocity = Vector2D::clampMagnitude(m_velocity, m_maxSpeed);
	}

	m_position += m_velocity * dt;
}


void KinematicObject::setMovingState(bool state) {
	m_isMoving = state;
}


bool KinematicObject::isMoving() const {
	return m_isMoving;
}


} // namespace invasion::game_models
