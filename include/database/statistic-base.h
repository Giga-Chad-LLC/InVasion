
#ifndef INVASIONSERVERS_STATISTICBASE_H
#define INVASIONSERVERS_STATISTICBASE_H

#include <sqlite3.h>
//#include <optional>
#include "../../3rd-party/common-header.h"
#include <functional>
#include <optional>
#include <cassert>


namespace invasion::statistic_base {

    struct UserStatisticsPerMatch {
        std::string nickname;
        int kills = 0;
        int deaths = 0;
        bool winThisMatch = false;
    };

    struct UserStatistics {
        int id;
        std::string nickname;
        int totalKills = 0;
        int totalDeaths = 0;
        double winRate = 0;
        int numberOfMatches = 0;
        int numberWins = 0;
    };


    using namespace sqlite_orm;

    class DatabaseManager {
    public:
        static auto &getTable() {
            auto table = make_table("users",
                                    make_column("id", &UserStatistics::id, primary_key(), autoincrement()),
                                    make_column("nickname", &UserStatistics::nickname, unique()),
                                    make_column("total_kills", &UserStatistics::totalKills),
                                    make_column("total_deaths", &UserStatistics::totalDeaths),
                                    make_column("win_rate", &UserStatistics::winRate),
                                    make_column("number_of_matches", &UserStatistics::numberOfMatches),
                                    make_column("number_of_wins", &UserStatistics::numberWins));
            static auto storage_ = make_storage("statistic.sqlite", table);
            storage_.sync_schema();
            return storage_;
        }
    };


    class StatisticAccessor {

    private:

        static void changeStats(UserStatistics &oldStats, const UserStatisticsPerMatch &newStats) {
            oldStats.totalKills += newStats.kills;
            oldStats.totalDeaths += newStats.deaths;
            oldStats.numberOfMatches++;
            if (newStats.winThisMatch) {
                oldStats.numberWins++;
            }
            oldStats.winRate = (double) oldStats.numberWins / oldStats.numberOfMatches;
        }

    public:

        static bool checkAvailabilityUser(const std::string &nickname) {
            auto table = DatabaseManager::getTable();
            auto usr = table.get_all<UserStatistics>(where(c(&UserStatistics::nickname) == nickname), limit(1));
            return !usr.empty();
        }

        static void insertNewLine(const UserStatisticsPerMatch &userStatistic) {
            UserStatistics userStatistics;
            userStatistics.nickname = userStatistic.nickname;
//            changeStats(userStatistics, userStatistic);
            auto table = DatabaseManager::getTable();
            table.insert(userStatistics);
        }

        static void addOrUpdateLine(const UserStatisticsPerMatch &userStatisticsPerMatch) {
            if (checkAvailabilityUser(userStatisticsPerMatch.nickname)) {
                updateLine(userStatisticsPerMatch);
            } else {
                insertNewLine(userStatisticsPerMatch);
            }
        }

        static void updateLine(const UserStatisticsPerMatch &userStatisticsPerMatch) {
            auto table = DatabaseManager::getTable();
            table.begin_transaction();

            auto arrayUserStatistic = table.get_all<UserStatistics>(
                    where(c(&UserStatistics::nickname) == userStatisticsPerMatch.nickname), limit(1));
            assert(arrayUserStatistic.size() == 1);
            auto userStatistics = arrayUserStatistic[0];
            changeStats(userStatistics, userStatisticsPerMatch);

            table.update(userStatistics);
            table.commit();
        }

        static UserStatistics getUserStatistic(const std::string &nickname) {
            auto table = DatabaseManager::getTable();
            auto userStatistic = table.get_all<UserStatistics>(where(c(&UserStatistics::nickname) == nickname),
                                                               limit(1));
            assert(userStatistic.size() == 1);
            return userStatistic[0];
        }

        static void printUsers() {
            auto arrayUsers = DatabaseManager::getTable().get_all<UserStatistics>();
            for (auto &user: arrayUsers) {
                std::cout << DatabaseManager::getTable().dump(user) << std::endl;
            }
        }

        static void clear() {
            DatabaseManager::getTable().remove_all<UserStatistics>();
            DatabaseManager::getTable().sync_schema();
        }
    };

}
#endif //INVASIONSERVERS_STATISTICBASE_H
