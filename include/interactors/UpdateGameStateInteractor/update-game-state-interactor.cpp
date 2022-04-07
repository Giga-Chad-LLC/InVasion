#include <iostream>

#include "update-game-state-interactor.h"
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"

#include "player-position-response-model.pb.h"
#include "vector2d.pb.h"

namespace invasion::interactors {
using namespace invasion::game_models;

response_models::PlayerPositionResponseModel UpdateGameStateInteractor::execute(GameSession& session) const {
	
	session.updateGameState();
	
	// hardcoding exactly one player
	const int playerId = 0;
	[[maybe_unused]] const Player& player = session.getPlayer(playerId);

	response_models::PlayerPositionResponseModel response;
	
	response.set_playerid(playerId);
	// setting position
	response.mutable_position()->set_x(player.getPosition().getX());
	response.mutable_position()->set_y(player.getPosition().getY());
	// setting velocity
	response.mutable_velocity()->set_x(player.getVelocity().getX());
	response.mutable_velocity()->set_y(player.getVelocity().getY());

	return response;
}

} // namespace invasion::interactors
