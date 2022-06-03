#include <string>

#include "username-response-interactor.h"

// response-models
#include "username-response-model.pb.h"


 
namespace invasion::interactors {
using namespace response_models;


UsernameResponseModel UsernameResponseInteractor::execute(int playerId, const std::string& username) const {
	UsernameResponseModel response;
	
	response.set_player_id(playerId);
	response.set_username(username);

	return response;
}

 
} // namespace invasion::interactors