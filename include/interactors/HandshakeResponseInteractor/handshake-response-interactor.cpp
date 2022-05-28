#include <memory>
#include <vector>

#include "handshake-response-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/GameSession/game-session-stats.h"
#include "game-models/StaticSupply/static-supply.h"
#include "game-models/StaticSupply/static-supply-enum.h"
#include "game-models/Player/player.h"
#include "game-models/Player/player-team-id-enum.h"
// response-models
#include "handshake-response-model.pb.h"
// util-models
#include "player-team-id.pb.h"
#include "supply-model.pb.h"
#include "supply-type.pb.h"
#include "player-health-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;
	
HandshakeResponseModel HandshakeResponseInteractor::execute(std::size_t remainingSessionTime_ms, GameSession& session, const int playerId) const {
	std::shared_ptr<Player> player = session.getPlayer(playerId);

	HandshakeResponseModel response;
	
	response.set_player_id(playerId);
	response.set_remaining_session_time_ms(remainingSessionTime_ms);

	// setting team id	
	const PlayerTeamId teamId = player->getTeamId();
	util_models::PlayerTeamId responseTeamId;

	if(teamId == PlayerTeamId::FirstTeam) {
		responseTeamId = util_models::PlayerTeamId::FirstTeam;
	}
	else if(teamId == PlayerTeamId::SecondTeam) {
		responseTeamId = util_models::PlayerTeamId::SecondTeam;
	}
	response.set_team_id(responseTeamId);

	// setting teams' kills count
	const GameSessionStats &stats = session.getGameStatistics();
	const int firstTeamKillsCount = stats.getFirstTeamKillsCount();
	const int secondTeamKillsCount = stats.getSecondTeamKillsCount();

	response.set_first_team_kills_count(firstTeamKillsCount);
	response.set_second_team_kills_count(secondTeamKillsCount);

	// retrieving supplies
	const std::vector<std::shared_ptr<StaticSupply>>& supplies = session.getSupplies();

	for (std::shared_ptr<StaticSupply> supply : supplies) {
		util_models::SupplyModel* supplyModel = response.add_supplies();

		supplyModel->set_supply_id(supply->getId());
		supplyModel->set_player_id(playerId);
		supplyModel->set_player_team_id(responseTeamId);

		// supply type
		const StaticSupplyType supplyType = supply->getType();
		if(supplyType == StaticSupplyType::AidKit) {
			supplyModel->set_supply_type(util_models::SupplyType::AidKit);
		}
		else if(supplyType == StaticSupplyType::AmmoCrate) {
			supplyModel->set_supply_type(util_models::SupplyType::AmmoCrate);
		}

		// position
		supplyModel->mutable_position()->set_x(supply->getPosition().getX());
		supplyModel->mutable_position()->set_y(supply->getPosition().getY());

		supplyModel->set_supply_capacity(supply->getLeftSupplyCapacity());
		supplyModel->set_is_active(supply->isActive());
	}

	// retrieving players' health
	const std::vector<std::shared_ptr<Player>>& players = session.getPlayers();
	
	for (auto player_ptr : players) {
		const bool active = player_ptr->getLifeState().isInActiveState();
		
		if(player_ptr->getId() != playerId && active) {
			util_models::PlayerHealthModel *model = response.add_players_hitpoints();
			model->set_player_id(player_ptr->getId());
			model->set_current_hitpoints(player_ptr->getLifeState().getHitPoints());
			model->set_initial_hitpoints(player_ptr->getLifeState().getInitialHitPoints());
		}
	}

	return response;
}

} // namespace invasion::interactors
