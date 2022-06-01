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
#include "player-data-model.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;
	
HandshakeResponseModel HandshakeResponseInteractor::execute(std::size_t remainingSessionTime_ms, GameSession& session, const int playerId) const {
	std::shared_ptr<Player> player = session.getPlayer(playerId);

	HandshakeResponseModel response;
	
	// setting player's id & team id
	{
		response.set_player_id(playerId);

		util_models::PlayerTeamId responseTeamId = this->mapPlayerTeamIds(player->getTeamId());
		response.set_team_id(responseTeamId);
	}


	// setting session data
	{
		response.set_remaining_session_time_ms(remainingSessionTime_ms);

		// setting teams' kills count
		const GameSessionStats &stats = session.getGameStatistics();
		const int firstTeamKillsCount = stats.getFirstTeamKillsCount();
		const int secondTeamKillsCount = stats.getSecondTeamKillsCount();

		response.set_first_team_kills_count(firstTeamKillsCount);
		response.set_second_team_kills_count(secondTeamKillsCount);
	}


	// setting supplies
	{
		const std::vector<std::shared_ptr<StaticSupply>>& supplies = session.getSupplies();

		for (std::shared_ptr<StaticSupply> supply : supplies) {
			util_models::SupplyModel* supplyModel = response.add_supplies();

			supplyModel->set_supply_id(supply->getId());
			supplyModel->set_player_id(supply->getPlayerId());

			util_models::PlayerTeamId responseTeamId = this->mapPlayerTeamIds(supply->getPlayerTeamId());
			supplyModel->set_player_team_id(responseTeamId);

			// supply type
			util_models::SupplyType supplyType = this->mapSupplyTypes(supply->getType());
			supplyModel->set_supply_type(supplyType);

			// position
			supplyModel->mutable_position()->set_x(supply->getPosition().getX());
			supplyModel->mutable_position()->set_y(supply->getPosition().getY());

			supplyModel->set_supply_capacity(supply->getLeftSupplyCapacity());
			supplyModel->set_is_active(supply->isActive());
		}
	}


	// setting players' data
	{
		const std::vector<std::shared_ptr<Player>>& players = session.getPlayers();

		for (auto player_ptr : players) {
			const bool active = player_ptr->getLifeState().isInActiveState();
			
			if(player_ptr->getId() != playerId && active) {
				util_models::PlayerDataModel *model = response.add_players_data();
				
				model->set_player_id(player_ptr->getId());

				util_models::PlayerTeamId teamId = this->mapPlayerTeamIds(player_ptr->getTeamId());
				model->set_team_id(teamId);

				model->set_username(player_ptr->getGameSessionStats().getUsername());

				model->set_current_hitpoints(player_ptr->getLifeState().getHitPoints());
				model->set_initial_hitpoints(player_ptr->getLifeState().getInitialHitPoints());

				util_models::PlayerSpecialization specialization = this->mapPlayerSpecializations(player_ptr->getSpecialization());
				model->set_specialization(specialization);

				model->set_kills(player_ptr->getGameSessionStats().getKills());
				model->set_deaths(player_ptr->getGameSessionStats().getDeaths());
			}
		}
	}

	return response;
}



util_models::PlayerTeamId HandshakeResponseInteractor::mapPlayerTeamIds(PlayerTeamId teamId) const {
	if(teamId == PlayerTeamId::FirstTeam) {
		return util_models::PlayerTeamId::FirstTeam;
	}
	else if(teamId == PlayerTeamId::SecondTeam) {
		return util_models::PlayerTeamId::SecondTeam;
	}
	throw std::runtime_error("Provided team id does not exist!");
}



util_models::SupplyType HandshakeResponseInteractor::mapSupplyTypes(StaticSupplyType supplyType) const {
	if(supplyType == StaticSupplyType::AidKit) {
		return util_models::SupplyType::AidKit;
	}
	else if(supplyType == StaticSupplyType::AmmoCrate) {
		return util_models::SupplyType::AmmoCrate;
	}
	throw std::runtime_error("Provided supply type does not exist!");
}


util_models::PlayerSpecialization HandshakeResponseInteractor::mapPlayerSpecializations(PlayerSpecialization specialization) const {
	switch (specialization) {
		case PlayerSpecialization::Stormtrooper: {
			return util_models::PlayerSpecialization::Stormtrooper;
		}
		case PlayerSpecialization::Sentinel: {
			return util_models::PlayerSpecialization::Sentinel;
		}
		case PlayerSpecialization::Support: {
			util_models::PlayerSpecialization::Support;
		}
		case PlayerSpecialization::Medic: {
			return util_models::PlayerSpecialization::Medic;
		}
		default: {
			throw std::runtime_error("HandshakeResponseInteractor cannot map provided specialization");
		}
	}
}


} // namespace invasion::interactors
