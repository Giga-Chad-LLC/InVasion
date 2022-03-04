#include "move-interactor.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"


namespace invasion::interactors {
using namespace invasion::game_models;


void MoveInteractor::execute(const MoveRequestModel& req, Player& player) const {
	MoveRequestModel::MoveEvent event = req.getEvent();
	Vector2D moving_force = player.getMovingForce();

	switch (event) {
		// starting
		case MoveRequestModel::MoveEvent::StartMovingUp: {
			moving_force.setY(1);
			player.setMovingState(true);
			break;
		}
		case MoveRequestModel::MoveEvent::StartMovingRight: {
			moving_force.setX(1);
			player.setMovingState(true);
			break;
		}
		case MoveRequestModel::MoveEvent::StartMovingDown: {
			moving_force.setY(-1);
			player.setMovingState(true);
			break;
		}
		case MoveRequestModel::MoveEvent::StartMovingLeft: {
			moving_force.setX(-1);
			player.setMovingState(true);
			break;
		}
		// stopping
		case MoveRequestModel::MoveEvent::StopMovingUp:
		case MoveRequestModel::MoveEvent::StopMovingDown: {
			moving_force.setY(0);
			player.setMovingState(false);
			break;
		}
		case MoveRequestModel::MoveEvent::StopMovingRight:
		case MoveRequestModel::MoveEvent::StopMovingLeft: {
			moving_force.setX(0);
			player.setMovingState(false);			
		}
	}

	player.setMovingForce(moving_force);
}


} // namespace invasion::interactors
