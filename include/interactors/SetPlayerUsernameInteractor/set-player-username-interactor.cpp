#include <string>
#include <memory>

#include "set-player-username-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
// request-models
#include "client-credentials-request-model.pb.h"
 
 
namespace invasion::interactors {
using namespace game_models;
using namespace request_models;
 

void SetPlayerUsernameInteractor::execute(const ClientCredentialsRequestModel& req, GameSession& session) const {
	const int playerId = req.player_id();
	const std::string username = req.username();

	std::shared_ptr<Player> player = session.getPlayer(playerId);
	
	player->getGameSessionStats().setUsername(username);
}

 
} // namespace invasion::interactors
 