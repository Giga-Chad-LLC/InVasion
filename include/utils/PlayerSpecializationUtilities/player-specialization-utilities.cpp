#include <stdexcept>

#include "player-specialization-utilities.h"

// game-models
#include "game-models/Player/player-specialization-enum.h"
// util-models
#include "player-specialization.pb.h"


namespace invasion::utils {
	
game_models::PlayerSpecialization PlayerSpecializationUtilities::mapSpecializations(util_models::PlayerSpecialization specializationModel) {
	game_models::PlayerSpecialization specialization;
	
	switch (specializationModel) {
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
			throw std::runtime_error("PlayerSpecializationUtilities cannot map provided specialization");
		}
	}

	return specialization;
}

} // namespace invasion::utils
