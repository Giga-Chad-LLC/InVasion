#ifndef KINEMATIC_OBJECT_H_
#define KINEMATIC_OBJECT_H_


#include "game-models/Vector2D/vector2d.h"
#include "game-models/Object/object.h"



namespace invasion::game_models {

class KinematicObject : public Object {
public:
	explicit KinematicObject(Vector2D shapeColliderSize,
							 Vector2D hitboxColliderSize,
							 Vector2D initialPosition,
							 double mass,
							 double maxSpeed);

	Vector2D getVelocity() const;
	void setVelocity(Vector2D velocity);

	Vector2D getMovingForce() const;
	void setMovingForce(Vector2D force);

	Vector2D getResultForce() const;
	void setResultForce(Vector2D force);

	Vector2D getAcceleration() const;

	Vector2D intentMove(double dt) const;
	void makeMove(double dt);

	void setMovingState(bool state);
	bool isMoving() const;

private:
	Vector2D m_velocity;
	Vector2D m_acceleration;
	Vector2D m_movingForce;
	Vector2D m_resultForce;
	const double m_mass;
	const double m_maxSpeed;
	bool m_isMoving;
};


} // namespace invasion::game_models



#endif // KINEMATIC_OBJECT_H_