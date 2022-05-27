#include <memory>

#include "retrieve-weapon-direction-response-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
#include "game-models/Vector2D/vector2d.h"
// response-models
#include "weapon-direction-response-model.pb.h"
 
 
namespace invasion::interactors {
using namespace game_models;
using namespace response_models;    


WeaponDirectionResponseModel RetrieveWeaponDirectionResponseInteractor::execute(const int playerId, GameSession& session) const {
	std::shared_ptr<Player> player = session.getPlayer(playerId);

	const Vector2D direction = player->getWeapon().getDirection();

	WeaponDirectionResponseModel response;
	
	response.set_player_id(playerId);
	response.mutable_direction()->set_x(direction.getX());
	response.mutable_direction()->set_y(direction.getY());

	return response;
}

 
} // namespace invasion::interactors
 