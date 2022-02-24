#include <iostream>

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

// operators
Vector2D Vector2D::operator+(const Vector2D& other) const {
	return Vector2D(m_x + other.m_x, m_y + other.m_y);
}

Vector2D& Vector2D::operator+=(const Vector2D& other) {
	m_x += other.m_x;
	m_y += other.m_y;
	return *this;
}

Vector2D operator*(const Vector2D& vec, double value) {
	return Vector2D(vec.m_x * value, vec.m_y * value);
}

Vector2D operator*(double value, const Vector2D& vec) {
	return Vector2D(vec.m_x * value, vec.m_y * value);
}

std::ostream& operator<<(std::ostream& os, const Vector2D& vec) {
	os << "Vector2D (" << vec.m_x << ", " << vec.m_y << ")" << std::endl;
	return os;
}

} // namespace invasion::game_models
