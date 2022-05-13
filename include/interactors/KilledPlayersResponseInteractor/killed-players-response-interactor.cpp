#include <vector>
#include <memory>

#include "killed-players-response-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
// response-models
#include "killed-player-response-model.pb.h"
#include "game-state-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;

	
void KilledPlayersResponseInteractor::execute(GameStateResponseModel& response, GameSession& session) const {
	std::vector<std::shared_ptr<Player>>& killedPlayers = session.getKilledPlayers();

	for (const auto& player_ptr : killedPlayers) {
		const int playerId = player_ptr->getId();
		const int killedBy = player_ptr->getLifeState().killedBy();

		KilledPlayerResponseModel* playerModel = response.add_killed_players();

		playerModel->set_player_id(playerId);
		playerModel->set_killed_by(killedBy);
	}
}

} // namespace invasion::interactors
