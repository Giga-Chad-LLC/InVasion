
#ifndef INVASIONSERVERS_STATISTICBASE_H
#define INVASIONSERVERS_STATISTICBASE_H

#include <sqlite3.h>
//#include <optional>
#include "../../../3rd-party/common-header.h"
#include <functional>
#include <optional>
#include <cassert>
#include "database/StatisticsContainer/statistic-container.h"

namespace invasion::statistic_base {

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
            auto table = make_table("statistic",
                                    make_column("id", &UserStatistics::id, primary_key(), autoincrement()),
                                    make_column("nickname", &UserStatistics::nickname, unique()),
                                    make_column("total_kills", &UserStatistics::totalKills),
                                    make_column("total_deaths", &UserStatistics::totalDeaths),
                                    make_column("win_rate", &UserStatistics::winRate),
                                    make_column("number_of_matches", &UserStatistics::numberOfMatches),
                                    make_column("number_of_wins", &UserStatistics::numberWins));
            static auto storage_ = make_storage("db.sqlite", table);
            storage_.sync_schema();
            return storage_;
        }
    };


    class StatisticAccessor {
    public:
        static void changeStats(UserStatistics &oldStats, const StatisticContainer &newStats) {
            oldStats.totalKills += newStats.getKills();
            oldStats.totalDeaths += newStats.getDeaths();
            oldStats.numberOfMatches++;
            if (newStats.getWinThisMatch()) {
                oldStats.numberWins++;
            }
            oldStats.winRate = (double) oldStats.numberWins / oldStats.numberOfMatches;
        }

        static void insertNewLine(const StatisticContainer &userStatistic) {
            UserStatistics userStatistics;
            userStatistics.nickname = userStatistic.getNickname();
            auto &table = DatabaseManager::getTable();
            table.insert(userStatistics);
        }

        static void updateLine(const StatisticContainer &userStatisticsPerMatch) {
            auto &table = DatabaseManager::getTable();
            table.begin_transaction();

            auto arrayUserStatistic = table.get_all<UserStatistics>(
                    where(c(&UserStatistics::nickname) == userStatisticsPerMatch.getNickname()), limit(1));
            assert(arrayUserStatistic.size() == 1);
            auto& userStatistics = arrayUserStatistic[0];
            changeStats(userStatistics, userStatisticsPerMatch);

            table.update(userStatistics);
            table.commit();
        }

        static void addOrUpdateLine(const StatisticContainer &userStatisticsPerMatch) {
            if (checkAvailabilityUser(userStatisticsPerMatch.getNickname())) {
                updateLine(userStatisticsPerMatch);
            } else {
                insertNewLine(userStatisticsPerMatch);
            }
        }

        static UserStatistics getUserStatistic(const std::string &nickname) {
            auto &table = DatabaseManager::getTable();
            auto userStatistic = table.get_all<UserStatistics>(where(c(&UserStatistics::nickname) == nickname),
                                                               limit(1));
            assert(userStatistic.size() == 1);
            return userStatistic[0];
        }

        static bool checkAvailabilityUser(const std::string &nickname) {
            auto &table = DatabaseManager::getTable();
            auto usr = table.get_all<UserStatistics>(where(c(&UserStatistics::nickname) == nickname), limit(1));
            return !usr.empty();
        }


        // ========= debug-only =========


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
