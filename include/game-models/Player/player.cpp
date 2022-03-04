#include "player.h"

#include "game-models/Vector2D/vector2d.h"

namespace invasion::game_models {
const double Player::MAX_SPEED = Vector2D(10, 0).magnitude();
const double Player::MASS = 60.0;
const Vector2D Player::COLLIDER_SIZE(20, 20);

	
Player::Player(Vector2D initial_pos)
	: KinematicObject(
		Player::COLLIDER_SIZE, 
		std::move(initial_pos), 
		Player::MASS,
		Player::MAX_SPEED
	) {}



} // namespace invasion::game_models
