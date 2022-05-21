#ifndef AMMO_CRATE_H_
#define AMMO_CRATE_H_

#include <memory>

// game-models
#include "game-models/StaticSupply/static-supply.h"
#include "game-models/Player/player.h"
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {
	
class AmmoCrate : public StaticSupply {
public:
	explicit AmmoCrate(Vector2D initialPosition,
					int supplyId,
					int playerId);

	void supply(std::shared_ptr<Player> player) override;

	inline static const double SUPPLY_PORTION = 25.0;

private:
	inline static const Vector2D SHAPE_COLLIDER_SIZE = Vector2D(10.0, 10.0);
	inline static const double INITIAL_SUPPLY_CAPACITY = 200.0;
};

} // namespace invasion::game_models



#endif // AMMO_CRATE_H_
