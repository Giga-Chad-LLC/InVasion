#include <iostream>

#include "game-world-manager.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"

namespace invasion::game_models {
	
void GameWorldManager::updatePlayersPositions(std::vector<Player>& players, double dt) const {
	const double g = -9.81;
	const double nu = 0.6;
	const double friction_coef = -nu * Vector2D(0, Player::MASS * g).magnitude();
	const double applied_force_magnitude = 375;

	for(Player& player : players) {
		// applying friction force
		if(player.isMoving()) {
			const Vector2D applied_force = Vector2D::clampMagnitude(player.getMovingForce(), applied_force_magnitude);
			const Vector2D friction_force = applied_force.normalize() * friction_coef;
			
			//std::cout << "applied_force magnitude: " << applied_force.magnitude() << std::endl;
			// std::cout << "friction_force magnitude: " << friction_force.magnitude() << std::endl;

			Vector2D result_force = Vector2D::ZERO;
		
			if(friction_force.magnitude() < applied_force.magnitude())
				result_force = applied_force + friction_force;

			//std::cout << "result_force: " << result_force << std::endl;
			
			player.setResultForce(result_force);
		}
		else {
			// applying friction until player stops
			const Vector2D cur_velocity = player.getVelocity();
			const Vector2D friction_force = cur_velocity.normalize() * friction_coef;
			const Vector2D acceleration = friction_force / Player::MASS;

			// std::cout << "friction_force magnitude: " << friction_force.magnitude() << std::endl;

			if((acceleration * dt).magnitude() >= cur_velocity.magnitude()) {
				player.setResultForce(Vector2D::ZERO);
				player.setVelocity(Vector2D::ZERO);
			}
			else {
				player.setResultForce(friction_force);
			}
		}

		player.makeMove(dt);
	}
}


Vector2D GameWorldManager::tryUpdatePosition(KinematicObject* object, double dt) const {
	return object->intentMove(dt);
}


} // namespace invasion::game_models
