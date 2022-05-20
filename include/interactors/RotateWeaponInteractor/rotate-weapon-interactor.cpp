#include <utility>
#include <memory>

#include "rotate-weapon-interactor.h"
// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
// protos
#include "rotate-weapon-request-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace request_models;
	
void RotateWeaponInteractor::execute(const RotateWeaponRequestModel& req, GameSession& session) const {
	std::shared_ptr<Player> player_ptr = session.getPlayer(req.player_id());
	const game_models::Vector2D direction(req.direction().x(), req.direction().y());

	player_ptr->getWeapon().setDirection(std::move(direction));
}

} // namespace invasion::interactors
