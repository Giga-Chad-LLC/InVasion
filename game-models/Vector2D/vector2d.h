#ifndef VECTOR2D_H_
#define VECTOR2D_H_


namespace invasion::game_models {


struct Vector2D {
	Vector2D() = default;

	void setX(long long x);
	long long getX() const;

	void setY(long long y);
	long long getY() const;

private:
	long long m_x = 0;
	long long m_y = 0;
};


} // namespace invasion::game_models


#endif