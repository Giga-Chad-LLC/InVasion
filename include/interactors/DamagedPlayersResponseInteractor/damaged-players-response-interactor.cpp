#include "damaged-players-response-interactor.h"

#include <optional>
#include <memory>
#include <vector>
// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
// response-models
#include "damaged-players-response-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;

std::optional<DamagedPlayersResponseModel> DamagedPlayersResponseInteractor::execute(GameSession& session) const {
	std::vector<std::shared_ptr<Player>>& damagedPlayers = session.getDamagedPlayers();

	if(damagedPlayers.empty()) {
		return std::nullopt;
	}

	DamagedPlayersResponseModel response;

	for(const auto& player_ptr : damagedPlayers) {
		const int playerId = player_ptr->getId();
		const double hitPoints = player_ptr->getLifeState().getHitPoints();
		const int damagedBy = player_ptr->getLifeState().damagedBy();

		DamagedPlayerResponseModel* playerModel = response.add_damaged_players();

		playerModel->set_player_id(playerId);
		playerModel->set_new_hitpoints(hitPoints);
		playerModel->set_damaged_by(damagedBy);
	}

	return std::make_optional<DamagedPlayersResponseModel>(response);
}

} // namespace invasion::interactors
