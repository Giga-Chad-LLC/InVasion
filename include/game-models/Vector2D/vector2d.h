#ifndef VECTOR2D_H_
#define VECTOR2D_H_


namespace invasion::game_models {


struct Vector2D {
	Vector2D() = default;
	explicit Vector2D(double x, double y);

	void setX(double x);
	double getX() const;

	void setY(double y);
	double getY() const;

private:
	double m_x = 0.;
	double m_y = 0.;
};


} // namespace invasion::game_models


#endif // VECTOR2D_H_
