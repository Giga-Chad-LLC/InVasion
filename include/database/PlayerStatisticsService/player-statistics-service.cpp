#include <iostream>

#include "player-statistics-service.h"

// libs
#include "libs/json/json.hpp"
// database
#include "database/StatisticsContainer/statistic-container.h"
#include "database/StatisticAccessor/statistic-accessor.h"
#include "database/Authenticator/autenticator.h"

namespace invasion::services {
using json = nlohmann::json;


void PlayerStatisticsService::update(const json& request) const {
	std::string token = request["token"];

	statistic_base::StatisticContainer data(request);
	
	bool isTokenValid = token_authenticator::Authenticator::checkTokenMatch(data.getNickname(), token);

	if(isTokenValid) {
		statistic_base::StatisticAccessor::addOrUpdateLine(data);
		std::cout << "Player stats update done" << std::endl;
	}
	else {
		std::cout << "Error while updating player stats: invalid token" << std::endl;
	}
}



json PlayerStatisticsService::retrieve(const std::string username) const {
	statistic_base::UserStatistics stats = statistic_base::StatisticAccessor::getUserStatistic(username); 
	
	json response;
	
	response["nickname"] = stats.nickname;
	response["kills"] = stats.totalKills;
	response["deaths"] = stats.totalDeaths;
	response["matches"] = stats.numberOfMatches;
	response["victories"] = stats.numberWins;

	return response;
}


} // invasion::services
