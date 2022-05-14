#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include <iostream>
#include <utility>

namespace invasion::game_models {


class Vector2D {
public:
	explicit Vector2D() = default;
	explicit Vector2D(double x, double y);
	explicit Vector2D(const std::pair<double, double>& coords);

	void setX(double x);
	double getX() const;

	void setY(double y);
	double getY() const;

	double magnitude() const;

	Vector2D normalize() const;

	// static methods
	static Vector2D clampMagnitude(const Vector2D& vector, double magnitude);

	// operators
	Vector2D operator+(const Vector2D& other) const;
	Vector2D& operator+=(const Vector2D& other);

	friend Vector2D operator*(const Vector2D& vector, double value);
	friend Vector2D operator*(double value, const Vector2D& vector);
	friend Vector2D operator/(const Vector2D& vector, double value);
	friend std::ostream& operator<<(std::ostream& os, const Vector2D& vector);

	// static members
	const static Vector2D ZERO;
	static const double EPS;

private:
	double m_x = 0.0;
	double m_y = 0.0;
};

} // namespace invasion::game_models


#endif // VECTOR2D_H_
