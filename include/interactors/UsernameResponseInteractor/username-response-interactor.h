#ifndef USERNAME_RESPONSE_INTERACTOR_H_
#define USERNAME_RESPONSE_INTERACTOR_H_
 
#include <string>

// response-models
#include "username-response-model.pb.h"
 
 
namespace invasion::interactors {
using namespace response_models;    


class UsernameResponseInteractor {
public:
	UsernameResponseModel execute(int playerId, const std::string& username) const;
};


} // namespace invasion::interactors
 
 
 
#endif // USERNAME_RESPONSE_INTERACTOR_H_
 