#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include <iostream>

namespace invasion::game_models {


class Vector2D {
public:
	explicit Vector2D() = default;
	explicit Vector2D(double x, double y);

	void setX(double x);
	double getX() const;

	void setY(double y);
	double getY() const;

	double magnitude() const;

	Vector2D normalize() const;

	// static methods
	static Vector2D clampMagnitude(const Vector2D& vec1, const double limit);

	// operators
	Vector2D operator+(const Vector2D& other) const;
	Vector2D& operator+=(const Vector2D& other);

	friend Vector2D operator*(const Vector2D& vec, double value);
	friend Vector2D operator*(double value, const Vector2D& vec);
	friend Vector2D operator/(const Vector2D& vec, double value);
	friend std::ostream& operator<<(std::ostream& os, const Vector2D& vec);

	// static members
	const static Vector2D ZERO;
	static const double EPS;

private:
	double m_x = 0.0;
	double m_y = 0.0;
};

} // namespace invasion::game_models


#endif // VECTOR2D_H_
