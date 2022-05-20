#include <stdexcept>

#include "select-player-specialization-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player-specialization-enum.h"
// request-models
#include "select-player-specialization-request-model.pb.h"
// response-models
#include "player-specialization-response-model.pb.h"
// util-models
#include "player-specialization.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;
using namespace request_models;

PlayerSpecializationResponseModel SelectPlayerSpecializationInteractor::execute(
	const SelectPlayerSpecializationRequestModel& req, GameSession& session) const {
	
	game_models::PlayerSpecialization specialization;

	// selecting specialization according to provided one in req
	switch (req.specialization()) {
		case util_models::PlayerSpecialization::Stormtrooper: {
			specialization = game_models::PlayerSpecialization::Stormtrooper;
			break;
		}
		case util_models::PlayerSpecialization::Sentinel: {
			specialization = game_models::PlayerSpecialization::Sentinel;
			break;
		}
		case util_models::PlayerSpecialization::Support: {
			specialization = game_models::PlayerSpecialization::Support;
			break;
		}
		case util_models::PlayerSpecialization::Medic: {
			specialization = game_models::PlayerSpecialization::Medic;
			break;
		}
		default: {
			throw std::runtime_error("SelectPlayerSpecializationInteractor cannot create player with provided in the request specialization");
		}
	}

	const int playerId = session.createPlayerAndReturnId(specialization);

	PlayerSpecializationResponseModel response;
	response.set_player_id(playerId);
	response.set_specialization(req.specialization());

	return response;
}

} // namespace invasion::interactors
