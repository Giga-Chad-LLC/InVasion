#include "vector2d.h"

namespace invasion::game_models {
	

void Vector2D::setX(long long x) {
	m_x = x;
}

long long Vector2D::getX() const {
	return m_x;
}


void Vector2D::setY(long long y) {
	m_y = y;
}

long long Vector2D::getY() const {
	return m_y;
}



} // namespace invasion::game_models