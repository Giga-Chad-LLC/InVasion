#include "move-interactor.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"


namespace invasion::interactors {
using namespace invasion::game_models;


void MoveInteractor::execute(const MoveRequestModel& req, Player& player) const {
	MoveRequestModel::MoveEvent event = req.getEvent();
	
	const double force_magnitude = 600;
	Vector2D moving_force = player.getMovingForce();

	switch (event) {
		case MoveRequestModel::MoveEvent::StartMovingUp: {
			moving_force += Vector2D::clampMagnitude(Vector2D(1, 0), force_magnitude);
			player.setMovingForce(moving_force);
			player.setMovingState(true);
			break;
		}	
		case MoveRequestModel::MoveEvent::StopMovingUp: {
			player.setMovingForce(Vector2D::ZERO);
			player.setMovingState(false);
		}
	}
}


} // namespace invasion::interactors
