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
	const int playerId = 0;
	[[maybe_unused]] const Player& player = session.getPlayer(playerId);

	response_models::PlayerPositionResponseModel response;
	
	// setting position 
	[[maybe_unused]] util_models::Vector2D* position = new util_models::Vector2D();
	position->set_x(player.getPosition().getX());
	position->set_y(player.getPosition().getY());

	// setting velocity
	[[maybe_unused]] util_models::Vector2D* velocity = new util_models::Vector2D();
	velocity->set_x(player.getVelocity().getX());
	velocity->set_y(player.getVelocity().getY());

	// setting response
	response.set_allocated_position(position); // response takes ownership of position. Need double check on that
	response.set_allocated_velocity(velocity); // response takes ownership of velocity. Need double check on that
	response.set_playerid(playerId);

	return response;
}

} // namespace invasion::interactors
