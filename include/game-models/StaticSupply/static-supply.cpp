#include "static-supply.h"
#include "static-supply-enum.h"
// game-models
#include "game-models/StaticObject/static-object.h"
#include "game-models/Player/player.h"
#include "game-models/Vector2D/vector2d.h"


namespace invasion::game_models {
	
StaticSupply::StaticSupply(Vector2D shapeColliderSize,
						Vector2D initialPosition,
						int supplyId,
						StaticSupplyType supplyType,
						int playerId,
						PlayerTeamId playerTeamId,
						int initialSupplyCapacity)
	: StaticObject(shapeColliderSize, shapeColliderSize, initialPosition),
		m_leftSupplyCapacity(initialSupplyCapacity),
	  	m_isActive(true),
		m_id(supplyId),
		m_type(supplyType),
		m_playerId(playerId),
		m_playerTeamId(playerTeamId) {}


bool StaticSupply::isActive() const {
	return m_isActive;
}


int StaticSupply::getLeftSupplyCapacity() const {
	return m_leftSupplyCapacity;
}


StaticSupplyType StaticSupply::getType() const {
	return m_type;
}

int StaticSupply::getId() const {
	return m_id;
}

int StaticSupply::getPlayerId() const {
	return m_playerId;
}

PlayerTeamId StaticSupply::getPlayerTeamId() const {
	return m_playerTeamId;
}

} // namespace invasion::game_models
