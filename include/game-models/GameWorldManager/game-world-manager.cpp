
#include "game-world-manager.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"

namespace invasion::game_models {
	

void GameWorldManager::updatePlayersPositions(std::vector<Player>& players, double dt) const {
	for(Player& player : players) {
		player.makeMove(dt);
	}
}


Vector2D GameWorldManager::tryUpdatePosition(KinematicObject* object, double dt) const {
	return object->intentMove(dt);
}


} // namespace invasion::game_models
