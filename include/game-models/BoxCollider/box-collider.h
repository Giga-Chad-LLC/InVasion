#ifndef COLLIDER_H_
#define COLLIDER_H_

#include <utility>
#include <iostream>

#include "game-models/Vector2D/vector2d.h"



namespace invasion::game_models {


class BoxCollider {
public:
	explicit BoxCollider() = default;
	explicit BoxCollider(Vector2D size);
	
	Vector2D size() const;
	bool collidesWith(const Vector2D& center, const BoxCollider& other, const Vector2D& otherCenter) const;

	double calculateClosestDistanceBetween(const Vector2D& center, const BoxCollider& other, const Vector2D& otherCenter) const;

private:
	const Vector2D m_size;
};



} // namespace invasion::game_models




#endif // COLLIDER_H_
