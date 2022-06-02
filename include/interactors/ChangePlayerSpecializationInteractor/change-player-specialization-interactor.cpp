#include "change-player-specialization-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
// utils
#include "utils/PlayerSpecializationUtilities/player-specialization-utilities.h"
// request-models
#include "change-player-specialization-request-model.pb.h"
// response-models
#include "player-specialization-response-model.pb.h"
// util-models
#include "player-specialization.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;
using namespace request_models;
	
PlayerSpecializationResponseModel ChangePlayerSpecializationInteractor::execute(
	const ChangePlayerSpecializationRequestModel& req, GameSession& session) const {
	
	const int playerId = req.player_id();
	const PlayerSpecialization specialization = utils::PlayerSpecializationUtilities::mapSpecializations(req.specialization());

	std::shared_ptr<Player> player = session.getPlayer(playerId);
	
	// if specializations differ
	if(specialization != player->getSpecialization()) {
		session.changePlayerSpecialization(playerId, specialization); 
	}

	// retrieving newly created player
	player = session.getPlayer(playerId);

	PlayerSpecializationResponseModel response;
	response.set_player_id(playerId);
	response.set_specialization(req.specialization());
	response.set_ammo(player->getWeapon().getInitialAmmo());
	response.set_magazine(player->getWeapon().getLeftMagazine());
	response.set_initial_hitpoints(player->getLifeState().getInitialHitPoints());

	std::cout << "Select specialization for player " << player->getId() << ": " << static_cast<int>(player->getSpecialization()) << std::endl;
	std::cout << "His new total hitpoints: " << player->getLifeState().getInitialHitPoints() << std::endl;

	return response;
}


} // namespace invasion::interactors
