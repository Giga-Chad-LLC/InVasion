#ifndef STATIC_OBJECT_H_
#define STATIC_OBJECT_H_

// game-models
#include "game-models/Object/object.h"
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {
	
class StaticObject : public Object {
public:
	explicit StaticObject(Vector2D shapeColliderSize,
						  Vector2D hitboxColliderSize,
						  Vector2D initialPosition);
};

} // namespace invasion::game_models



#endif // STATIC_OBJECT_H_
