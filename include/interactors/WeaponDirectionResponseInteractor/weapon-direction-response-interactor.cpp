#include <memory>

#include "weapon-direction-response-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
#include "game-models/Vector2D/vector2d.h"
// request-models
#include "weapon-direction-request-model.pb.h"
// response-models
#include "weapon-direction-response-model.pb.h"


namespace invasion::interactors {
using namespace game_models;
using namespace request_models;
using namespace response_models;	


WeaponDirectionResponseModel WeaponDirectionResponseInteractor::execute(
	const WeaponDirectionRequestModel& req, GameSession& session) const {

	const int playerId = req.player_id();
	const double x = req.direction().x();
	const double y = req.direction().y();

	const Vector2D weaponDirection(x, y);

	std::shared_ptr<Player> player = session.getPlayer(playerId);
	player->getWeapon().setDirection(weaponDirection);

	WeaponDirectionResponseModel response;
	response.set_player_id(playerId);
	response.mutable_direction()->set_x(x);
	response.mutable_direction()->set_y(y);

	return response;
}



} // namespace invasion::interactors

