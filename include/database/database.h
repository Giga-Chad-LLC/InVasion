#ifndef INVASION_DATABASE_H
#define INVASION_DATABASE_H

#include <sqlite3.h>
//#include <optional>
#include "../../3rd-party/common-header.h"
#include <functional>
#include <optional>

struct User {
    int id;
    std::string nickname;
    std::string password;
};


namespace invasion::database_access{
	using namespace sqlite_orm;
	
    class DatabaseManager {
    public:
        static auto &getTable() {
            auto table = make_table("users",
                                    make_column("id", &User::id, primary_key(), autoincrement()),
                                    make_column("nickname", &User::nickname, unique()),
                                    make_column("hashed password", &User::password));
            static auto storage_ = make_storage("db.sqlite", table);
            storage_.sync_schema();
            return storage_;
        }
    };

    class DatabaseAccessor {

    public:
        static std::optional<std::string> getUserPassword(const std::string &nickname) {
            auto table = DatabaseManager::getTable();
            auto selectedUsersArray = table.select(&User::password, where(c(&User::nickname) == nickname), limit(1));
            if (selectedUsersArray.empty()) {
                return {};
            }
            return selectedUsersArray[0];
        }

        static bool checkUser(const std::string &nickname) {
            auto table = DatabaseManager::getTable();
            return getUserPassword(nickname).has_value();
        }

        static std::optional<User> getUser(const std::string &nickname) {
            auto table = DatabaseManager::getTable();
            auto selectedUsersArray = table.get_all<User>(where(c(&User::nickname) == nickname), limit(1));
            if (!selectedUsersArray.empty()) {
                return {};
            }
            return selectedUsersArray[0];
        }


        static void insertUser(const User &user) {
            auto table = DatabaseManager::getTable();
            table.insert(user);
        }

        static bool deleteUserByNickname(const std::string &nickname) {
            auto table = DatabaseManager::getTable();
            auto userID = getUser(nickname);
            if (userID.has_value()) {
                table.remove<User>(userID.value().id);
                return true;
            }
            return false;
        }
    };
}



#endif //INVASION_DATABASE_H
