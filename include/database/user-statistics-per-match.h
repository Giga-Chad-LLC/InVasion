
#ifndef INVASIONSERVERS_USERSTATISTICSPERMATCH_H
#define INVASIONSERVERS_USERSTATISTICSPERMATCH_H

#include "../../3rd-party/crow_all.h"

namespace invasion::statistic_base {
    struct UserStatisticsPerMatch {
        std::string nickname;
        int kills = 0;
        int deaths = 0;
        bool winThisMatch = false;

        UserStatisticsPerMatch() = default;

        UserStatisticsPerMatch(const std::string &nickname_) : nickname(nickname_) {}

        template<class T>
        UserStatisticsPerMatch(T request) : nickname(request["nickname"].s()), kills(request["kills"].i()),
                                            deaths(request["deaths"].i()),
                                            winThisMatch(request["win"].b()) {}
    };
}
#endif //INVASIONSERVERS_USERSTATISTICSPERMATCH_H
