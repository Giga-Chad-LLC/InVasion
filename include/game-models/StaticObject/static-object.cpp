#include <utility>

// game-models
#include "game-models/Vector2D/vector2d.h"

#include "static-object.h"


namespace invasion::game_models {

StaticObject::StaticObject(Vector2D shapeColliderSize,
						   Vector2D hitboxColliderSize,
						   Vector2D initialPosition)
	: Object(std::move(shapeColliderSize), std::move(hitboxColliderSize), std::move(initialPosition)) {
	
}

} // namespace invasion::game_models
