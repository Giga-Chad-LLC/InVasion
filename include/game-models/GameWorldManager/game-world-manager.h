#ifndef GAME_WORLD_MANAGER_H_
#define GAME_WORLD_MANAGER_H_

#include <vector>
#include <memory>

// game-models
#include "game-models/Player/player.h"
#include "game-models/Bullet/bullet.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/KinematicObject/kinematic-object.h"


namespace invasion::game_models {
	

class GameWorldManager {
public:
	void updatePlayersPositions(std::vector<std::shared_ptr<Player>>& players, double dt) const;
	
	void updateBulletsPositions(std::vector<std::shared_ptr<Bullet>>& bullets, 
								std::vector<std::shared_ptr<Player>>& players, double dt) const;
	
	void findDamagedPlayers(std::vector<std::shared_ptr<Player>>& players,
							std::vector<std::shared_ptr<Player>>& damagedPlayers) const;


private:
	void applyFrictionAndSetResultForceOnPlayer(std::shared_ptr<Player> player_ptr, double dt) const;
	
	void updatePlayerPhysicsOnPlayerCollision(std::vector<std::shared_ptr<Player>>& players,
											  std::shared_ptr<Player> consideredPlayer_ptr,
											  double dt) const;
};


} // namespace invasion::game_models

#endif // GAME_WORLD_MANAGER_H_
