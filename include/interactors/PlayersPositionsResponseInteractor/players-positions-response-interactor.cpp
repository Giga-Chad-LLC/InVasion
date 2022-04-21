#include <iostream>
#include <vector>

#include "players-positions-response-interactor.h"
// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
#include "game-models/Vector2D/vector2d.h"
// response models
#include "player-position-response-model.pb.h"
#include "game-state-response-model.pb.h"
// utils
#include "vector2d.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;

void PlayersPositionsResponseInteractor::execute(GameStateResponseModel& response, GameSession& session) const {
	
	const std::vector<Player>& players = session.getPlayers();
	
	for(const auto& player : players) {
		PlayerPositionResponseModel* playerModel = response.add_players();

		playerModel->set_player_id(player.getId());
		
		if(player.getTeamId() == Player::TeamId::FirstTeam) {
			playerModel->set_team_id(PlayerPositionResponseModel::FirstTeam);
		}
		else {
			playerModel->set_team_id(PlayerPositionResponseModel::SecondTeam);
		}

		const Vector2D velocity = player.getVelocity();
		playerModel->mutable_velocity()->set_x(velocity.getX());
		playerModel->mutable_velocity()->set_y(velocity.getY());

		const Vector2D position = player.getPosition();
		playerModel->mutable_position()->set_x(position.getX());
		playerModel->mutable_position()->set_y(position.getY());
	}
}

} // namespace invasion::interactors
