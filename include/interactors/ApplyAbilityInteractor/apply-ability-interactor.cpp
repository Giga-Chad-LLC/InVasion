#include <optional>
#include <iostream>

#include "apply-ability-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/AbilityEndowedPlayer/ability-endowed-player.h"
#include "game-models/StaticObject/static-object.h"
#include "game-models/StaticSupply/static-supply.h"
#include "game-models/StaticSupply/static-supply-enum.h"
#include "game-models/Vector2D/vector2d.h"
// request-models
#include "apply-ability-request-model.pb.h"
// response-models
#include "supply-response-model.pb.h"
// util-models
#include "supply-model.pb.h"
#include "supply-type.pb.h"
#include "player-team-id.pb.h"


namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;
using namespace request_models;


std::optional<SupplyResponseModel> ApplyAbilityInteractor::execute(const ApplyAbilityRequestModel& req, GameSession& session) const {
	const int playerId = req.player_id();
	std::shared_ptr<Player> player_ptr = session.getPlayer(playerId);
	
	// this piece of code is required only for testing purposes
	if(!player_ptr->hasAbility()) {
		std::cout << "ApplyAbilityInteractor cannot apply ability because player does not have ability. (Player id: "
				  << playerId << ")" << std::endl;
	}
	assert(player_ptr->hasAbility());
	
	// downcasing player
	assert(dynamic_cast<AbilityEndowedPlayer*>(player_ptr.get()));
	std::shared_ptr<AbilityEndowedPlayer> player(static_cast<AbilityEndowedPlayer*>(player_ptr.get()));

	// if cool down has not finished
	if(!player->isAbleToApplyAbility()) {
		return std::nullopt;
	}

	// applying ability
	const int supplyId = session.createIdForNewSupply();
	std::shared_ptr<StaticSupply> supply = player->applyAbility(supplyId);


	// pushing to supplies
	std::vector<std::shared_ptr<StaticSupply>>& supplies = session.getSupplies();
	supplies.push_back(supply);

	// pushing to obstalces
	std::vector<std::shared_ptr<StaticObject>>& obstacles = session.getObstacles();
	obstacles.emplace_back(supply.get());


	// creating response
	SupplyResponseModel response;

	response.set_player_id(playerId);

	// setting supply data
	util_models::SupplyModel* supplyModel = response.mutable_supply();

	supplyModel->set_supply_id(supplyId);
	supplyModel->set_player_id(playerId);

	// team id
	const PlayerTeamId teamId = player->getTeamId();
	if(teamId == PlayerTeamId::FirstTeam) {
		supplyModel->set_player_team_id(util_models::PlayerTeamId::FirstTeam);
	}
	else if(teamId == PlayerTeamId::SecondTeam) {
		supplyModel->set_player_team_id(util_models::PlayerTeamId::SecondTeam);
	}

	// supply type
	const StaticSupplyType supplyType = supply->getType();
	if(supplyType == StaticSupplyType::AidKit) {
		supplyModel->set_supply_type(util_models::SupplyType::AidKit);
	}
	else if(supplyType == StaticSupplyType::AmmoCrate) {
		supplyModel->set_supply_type(util_models::SupplyType::AmmoCrate);
	}

	// position
	const Vector2D position = player->getPosition();
	supplyModel->mutable_position()->set_x(position.getX());
	supplyModel->mutable_position()->set_y(position.getY());
	
	supplyModel->set_supply_capacity(supply->getLeftSupplyCapacity());
	supplyModel->set_is_active(supply->isActive());

	return response;
}


} // namespace invasion::interactors
