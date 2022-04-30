#ifndef BULLET_MANAGER_H_
#define BULLET_MANAGER_H_

#include <vector>
#include <memory>

// game-models
#include "game-models/Player/player.h"
#include "game-models/Bullet/bullet.h"


namespace invasion::game_models {
	
class BulletManager {
public:
	void updateBulletsPositions(std::vector<std::shared_ptr<Bullet>>& bullets, 
								std::vector<std::shared_ptr<Player>>& players, double dt) const;

	void removeCrushedAndFlewOutOfBoundsBullets(std::vector<std::shared_ptr<Bullet>>& bullets) const;
};
	
} // namespace invasion::game_models



#endif // BULLET_MANAGER_H_
