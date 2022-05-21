#include "damaged-players-response-interactor.h"

#include <memory>
#include <vector>
// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
// response-models
#include "game-state-response-model.pb.h"
#include "damaged-player-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;

void DamagedPlayersResponseInteractor::execute(GameStateResponseModel& response, GameSession& session) const {
	std::vector<std::shared_ptr<Player>>& damagedPlayers = session.getDamagedPlayers();

	for(const auto& player_ptr : damagedPlayers) {
		const int playerId = player_ptr->getId();
		const int hitPoints = player_ptr->getLifeState().getHitPoints();
		const int damagedBy = player_ptr->getLifeState().damagedBy();

		DamagedPlayerResponseModel* playerModel = response.add_damaged_players();

		playerModel->set_player_id(playerId);
		playerModel->set_new_hitpoints(hitPoints);
		playerModel->set_damaged_by(damagedBy);
	}
}

} // namespace invasion::interactors
