#include <stdexcept>

#include "select-player-specialization-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player-specialization-enum.h"
// utils
#include "utils/PlayerSpecializationUtilities/player-specialization-utilities.h"
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
	
	// selecting specialization according to provided one in req
	game_models::PlayerSpecialization specialization = utils::PlayerSpecializationUtilities::mapSpecializations(req.specialization());

	const int playerId = session.createPlayerAndReturnId(specialization);

	PlayerSpecializationResponseModel response;
	response.set_player_id(playerId);
	response.set_specialization(req.specialization());

	return response;
}

} // namespace invasion::interactors
