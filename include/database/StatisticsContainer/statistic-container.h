
#ifndef INVASIONSERVERS_USERSTATISTICSPERMATCH_H
#define INVASIONSERVERS_USERSTATISTICSPERMATCH_H

#include <string>

// libs
#include "libs/crow/crow_all.h"
#include "libs/json/json.hpp"

namespace invasion::statistic_base {
    struct StatisticContainer {
    private:
		using json = nlohmann::json;

        std::string username;
        int kills = 0;
        int deaths = 0;
        bool isVictory = false;
    public:
        StatisticContainer() = default;

		StatisticContainer(const json& request) {
			username = request["username"];
			kills = request["kills"];
			deaths = request["deaths"];
			isVictory = request["isVictory"];
		}

        StatisticContainer(const std::string &username_) : username(username_) {}

        template<class T>
        StatisticContainer(T request) : username(request["username"].s()), kills(request["kills"].i()),
                                        deaths(request["deaths"].i()),
                                        isVictory(request["isVictory"].b()) {}

        int getKills() const {
            return kills;
        }

        int getDeaths() const {
            return deaths;
        }

        std::string getUsername() const {
            return username;
        }

        bool getWinThisMatch() const {
            return isVictory;
        }

    };
}
#endif //INVASIONSERVERS_USERSTATISTICSPERMATCH_H
