#include <memory>

#include "update-player-ammo-response-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
// response-models
#include "update-player-ammo-response-model.pb.h"



namespace invasion::interactors {
using namespace game_models; 
using namespace response_models;

UpdatePlayerAmmoResponseModel UpdatePlayerAmmoResponseInteractor::execute(
	int playerId, GameSession& session) const {
	const std::shared_ptr<Player> player = session.getPlayer(playerId);

	const int ammo = player->getWeapon().getLeftAmmo();

	UpdatePlayerAmmoResponseModel response;
	response.set_player_id(playerId);
	response.set_new_ammo(ammo);

	return response;
}


} // namespace invasion::interactors
