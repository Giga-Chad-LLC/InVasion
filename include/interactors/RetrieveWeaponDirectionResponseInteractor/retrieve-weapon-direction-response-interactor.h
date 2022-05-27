#ifndef RETRIEVE_WEAPON_DIRECTION_RESPONSE_INTERACTOR_H_
#define RETRIEVE_WEAPON_DIRECTION_RESPONSE_INTERACTOR_H_
 
// game-models
#include "game-models/GameSession/game-session.h"
// response-models
#include "weapon-direction-response-model.pb.h"
 
 
namespace invasion::interactors {
using namespace game_models;
using namespace response_models;    
 

class RetrieveWeaponDirectionResponseInteractor {
public:
	WeaponDirectionResponseModel execute(int playerId, GameSession& session) const;
};


} // namespace invasion::interactors
 
 
 
#endif // RETRIEVE_WEAPON_DIRECTION_RESPONSE_INTERACTOR_H_