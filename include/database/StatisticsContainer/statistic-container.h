
#ifndef INVASIONSERVERS_USERSTATISTICSPERMATCH_H
#define INVASIONSERVERS_USERSTATISTICSPERMATCH_H

// #include "../../../3rd-party/crow_all.h"
// libs
#include "libs/crow/crow_all.h"
#include <string>

namespace invasion::statistic_base {
    struct StatisticContainer {
    private:
        std::string nickname;
        int kills = 0;
        int deaths = 0;
        bool winThisMatch = false;
    public:
        StatisticContainer() = default;

        StatisticContainer(const std::string &nickname_) : nickname(nickname_) {}

        template<class T>
        StatisticContainer(T request) : nickname(request["nickname"].s()), kills(request["kills"].i()),
                                        deaths(request["deaths"].i()),
                                        winThisMatch(request["win"].b()) {}

        int getKills() const {
            return kills;
        }

        int getDeaths() const {
            return deaths;
        }

        std::string getNickname() const {
            return nickname;
        }

        bool getWinThisMatch() const {
            return winThisMatch;
        }

    };
}
#endif //INVASIONSERVERS_USERSTATISTICSPERMATCH_H
