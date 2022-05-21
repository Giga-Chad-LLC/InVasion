#include <memory>
#include <algorithm>

#include "ammo-crate.h"

// game-models
#include "game-models/StaticSupply/static-supply.h"
#include "game-models/StaticSupply/static-supply-enum.h"
#include "game-models/Player/player.h"
#include "game-models/Weapon/weapon.h"
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {
	
AmmoCrate::AmmoCrate(Vector2D initialPosition,
					int supplyId,
					int playerId)
	: StaticSupply(
		AmmoCrate::SHAPE_COLLIDER_SIZE,
		initialPosition,
		supplyId,
		StaticSupplyType::AidKit,
		playerId,
		AmmoCrate::INITIAL_SUPPLY_CAPACITY
	) {}


void AmmoCrate::supply(std::shared_ptr<Player> player) {
	Weapon& weapon = player->getWeapon();
	// TODO
}

} // namespace invasion::game_models
