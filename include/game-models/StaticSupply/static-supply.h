#ifndef STATIC_SUPPLY_H_
#define STATIC_SUPPLY_H_

#include <memory>

#include "static-supply-enum.h"
// game-models
#include "game-models/StaticObject/static-object.h"
#include "game-models/Player/player.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {
	
class StaticSupply : public StaticObject {
public:
	explicit StaticSupply(Vector2D shapeColliderSize,
						Vector2D initialPosition,
						int supplyId,
						StaticSupplyType supplyType,
						int playerId,
						PlayerTeamId playerTeamId,
						int initialSupplyCapacity);

	bool isActive() const;

	virtual void supply(std::shared_ptr<Player> player) = 0;

	int getLeftSupplyCapacity() const;

	StaticSupplyType getType() const;

	int getId() const;

	int getPlayerId() const;

	PlayerTeamId getPlayerTeamId() const;

protected:
	int m_leftSupplyCapacity;
	bool m_isActive;

private:
	const int m_id;
	const StaticSupplyType m_type;
	const int m_playerId;
	const PlayerTeamId m_playerTeamId;
};

} // namespace invasion::game_models



#endif // STATIC_SUPPLY_H_
