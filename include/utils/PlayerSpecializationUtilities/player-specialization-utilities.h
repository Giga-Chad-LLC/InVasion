#ifndef PLAYER_SPECIALIZATION_UTILITIES_H_
#define PLAYER_SPECIALIZATION_UTILITIES_H_

// game-models
#include "game-models/Player/player-specialization-enum.h"
// util-models
#include "player-specialization.pb.h"


namespace invasion::utils {
	

class PlayerSpecializationUtilities {
public:
	PlayerSpecializationUtilities() = delete;
	static game_models::PlayerSpecialization mapSpecializations(util_models::PlayerSpecialization specializationModel);
};


} // namespace invasion::utils



#endif // PLAYER_SPECIALIZATION_UTILITIES_H_