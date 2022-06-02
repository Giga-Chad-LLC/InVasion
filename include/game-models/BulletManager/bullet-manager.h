#ifndef BULLET_MANAGER_H_
#define BULLET_MANAGER_H_

#include <vector>
#include <memory>

// game-models
#include "game-models/Player/player.h"
#include "game-models/Bullet/bullet.h"
#include "game-models/StaticObject/static-object.h"


namespace invasion::game_models {
	
class BulletManager {
public:
	void updateBulletsPositions(std::vector<std::shared_ptr<Bullet>>& bullets, 
								const std::vector<std::shared_ptr<Player>>& players,
								const std::vector<std::shared_ptr<StaticObject>>& obstacles,
								double dt) const;

	void removeCrushedAndFlewOutOfBoundsBullets(std::vector<std::shared_ptr<Bullet>>& bullets) const;

private:
	void collideBulletWithObstacles(std::shared_ptr<Bullet> bullet,
									const std::vector<std::shared_ptr<StaticObject>>& obstacles) const;

	void collideBulletWithPlayers(std::shared_ptr<Bullet> bullet,
								  const std::vector<std::shared_ptr<Player>>& players) const;
};
	
} // namespace invasion::game_models



#endif // BULLET_MANAGER_H_
