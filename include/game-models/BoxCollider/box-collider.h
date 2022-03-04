#ifndef COLLIDER_H_
#define COLLIDER_H_

#include <utility>
#include <iostream>

#include "game-models/Vector2D/vector2d.h"



namespace invasion::game_models {


struct BoxCollider {
	BoxCollider() = default;
	explicit BoxCollider(Vector2D size);

	Vector2D size() const;

	bool collidesWith(const Vector2D& center, const BoxCollider* const other, const Vector2D& other_center) const;


private:
	const Vector2D m_size;
};



} // namespace invasion::game_models




#endif // COLLIDER_H_
