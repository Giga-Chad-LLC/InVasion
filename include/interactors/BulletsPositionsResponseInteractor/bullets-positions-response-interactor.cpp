#include <vector>
#include <memory>

#include "bullets-positions-response-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Bullet/bullet.h"
#include "game-models/Vector2D/vector2d.h"
// response-models
#include "bullet-position-response-model.pb.h"
#include "game-state-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;	
using namespace response_models;

void BulletsPositionsResponseInteractor::execute(GameStateResponseModel& response, GameSession& session) const {	
	const std::vector<std::shared_ptr<Bullet>>& bullets = session.getBullets();

	for (const auto& bullet_ptr : bullets) {
		BulletPositionResponseModel* bulletModel = response.add_bullets();

		bulletModel->set_bullet_id(bullet_ptr->getId());
		bulletModel->set_player_id(bullet_ptr->getPlayerId());

		const Vector2D velocity = bullet_ptr->getVelocity();
		bulletModel->mutable_velocity()->set_x(velocity.getX());
		bulletModel->mutable_velocity()->set_y(velocity.getY());

		const Vector2D position = bullet_ptr->getPosition();
		bulletModel->mutable_position()->set_x(position.getX());
		bulletModel->mutable_position()->set_y(position.getY());
	}
}

} // namespace invasion::interactors
