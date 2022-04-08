#include <iostream>
#include <vector>

#include "update-game-state-interactor.h"
// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
#include "game-models/Vector2D/vector2d.h"
// response models
#include "player-position-response-model.pb.h"
#include "players-positions-response-model.pb.h"
// utils
#include "vector2d.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;


response_models::PlayersPositionsResponseModel UpdateGameStateInteractor::execute(GameSession& session) const {
	// updating game
	session.updateGameState();

	response_models::PlayersPositionsResponseModel response;
	
	const std::vector<Player>& players = session.getPlayers();
	
	for(const auto& player : players) {
		response_models::PlayerPositionResponseModel* playerModel = response.add_players();

		playerModel->set_playerid(player.getId());
		
		if(player.getTeamId() == Player::TeamId::FirstTeam) {
			playerModel->set_team_id(response_models::PlayerPositionResponseModel::FirstTeam);
		}
		else {
			playerModel->set_team_id(response_models::PlayerPositionResponseModel::SecondTeam);
		}

		const Vector2D velocity = player.getVelocity();
		playerModel->mutable_velocity()->set_x(velocity.getX());
		playerModel->mutable_velocity()->set_y(velocity.getY());

		const Vector2D position = player.getPosition();
		playerModel->mutable_position()->set_x(position.getX());
		playerModel->mutable_position()->set_y(position.getY());
	}

	/*
	
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

	*/

	return response;
}

} // namespace invasion::interactors
