#include "vector2d.h"

namespace invasion::game_models {


Vector2D::Vector2D(double x, double y)
	: m_x(x), m_y(y) {}


void Vector2D::setX(double x) {
	m_x = x;
}


double Vector2D::getX() const {
	return m_x;
}


void Vector2D::setY(double y) {
	m_y = y;
}


double Vector2D::getY() const {
	return m_y;
}


} // namespace invasion::game_models
