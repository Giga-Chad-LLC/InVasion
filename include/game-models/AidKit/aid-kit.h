#ifndef AID_KIT_H_
#define AID_KIT_H_

#include <memory>

// game-models
#include "game-models/StaticSupply/static-supply.h"
#include "game-models/Player/player.h"
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {
	
class AidKit : public StaticSupply {
public:
	explicit AidKit(Vector2D initialPosition,
					int supplyId,
					int playerId);

	void supply(std::shared_ptr<Player> player) override;

	inline static const double SUPPLY_PORTION = 25.0;

private:
	inline static const Vector2D SHAPE_COLLIDER_SIZE = Vector2D(10.0, 10.0);
	inline static const double INITIAL_SUPPLY_CAPACITY = 200.0;
};

} // namespace invasion::game_models



#endif // AID_KIT_H_
