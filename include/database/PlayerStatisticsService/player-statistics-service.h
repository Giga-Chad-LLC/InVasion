#ifndef PLAYER_STATISTICS_SERVICE_H_
#define PLAYER_STATISTICS_SERVICE_H_

// libs
#include "libs/json/json.hpp"


namespace invasion::services {
using json = nlohmann::json;


class PlayerStatisticsService {
public:
	void update(json request) const;

	json retrieve(const std::string username) const;
};

} // namespace invasion::services



#endif // PLAYER_STATISTICS_SERVICE_H_
