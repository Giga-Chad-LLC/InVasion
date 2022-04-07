#include "move-interactor.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-models/GameSession/game-session.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace request_models;

void MoveInteractor::execute(const request_models::MoveRequestModel& req, GameSession& session) const {
	const request_models::MoveRequestModel::MoveEvent event = req.current_event();
	Player& player = session.getPlayer(req.player_id());

	Vector2D moving_force = player.getMovingForce();

	switch (event) {
		// starti moving
		case request_models::MoveRequestModel::StartMovingUp: {
			moving_force.setY(-1);
			player.setMovingState(true);
			break;
		}
		case request_models::MoveRequestModel::StartMovingRight: {
			moving_force.setX(1);
			player.setMovingState(true);
			break;
		}
		case request_models::MoveRequestModel::StartMovingDown: {
			moving_force.setY(1);
			player.setMovingState(true);
			break;
		}
		case request_models::MoveRequestModel::StartMovingLeft: {
			moving_force.setX(-1);
			player.setMovingState(true);
			break;
		}
		// stop moving
		case request_models::MoveRequestModel::StopMovingUp:
		case request_models::MoveRequestModel::StopMovingDown: {
			moving_force.setY(0);
			break;
		}
		case request_models::MoveRequestModel::StopMovingRight:
		case request_models::MoveRequestModel::StopMovingLeft: {
			moving_force.setX(0);
			break;
		}
		default: {
			break;
		}
	}
	
	// if both components are zeros
	if(moving_force.magnitude() < Vector2D::EPS) {
		player.setMovingState(false);
	}

	player.setMovingForce(moving_force);
}


} // namespace invasion::interactors
