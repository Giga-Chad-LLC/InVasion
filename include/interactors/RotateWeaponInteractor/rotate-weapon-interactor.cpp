#include <utility>

#include "rotate-weapon-interactor.h"
// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
// protos
#include "rotate-weapon-request-model.pb.h"


namespace invasion::interactors {
	
void RotateWeaponInteractor::execute(
	const request_models::RotateWeaponRequestModel& req, 
	game_models::GameSession& session
) const {
	game_models::Player& player = session.getPlayer(req.player_id());
	const game_models::Vector2D direction(req.direction().x(), req.direction().y());

	player.getWeapon().setDirection(std::move(direction));
}

} // namespace invasion::interactors
