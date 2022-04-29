#ifndef INVASION_DATABASE_H
#define INVASION_DATABASE_H

#include <sqlite3.h>
//#include <optional>
#include "common-header.h"
#include <functional>
#include <optional>

struct User {
    int id;
    std::string nickname;
    std::string password;
};

using namespace sqlite_orm;

namespace {
    auto &getTable() {
        static auto storage_ = make_storage("db.sqlite",
                                            make_table("users",
                                                       make_column("id", &User::id, primary_key(), autoincrement()),
                                                       make_column("nickname", &User::nickname, unique()),
                                                       make_column("hashed password", &User::password)));
        return storage_;
    }


    struct methodsDB {
        static std::optional<std::string> getUserPassword(const std::string &nickname) {
            auto table = getTable();
            table.sync_schema();
            auto selectedUsersArray = table.select(&User::password, where(c(&User::nickname) == nickname), limit(1));
            if (selectedUsersArray.empty()) {
                return {};
            }
            return selectedUsersArray[0];
        }

        static bool checkUser(const std::string &nickname) {
            auto table = getTable();
            table.sync_schema();
            return getUserPassword(nickname).has_value();
        }

        static std::optional<User> getUser(const std::string &nickname) {
            auto table = getTable();
            table.sync_schema();
            auto selectedUsersArray = table.get_all<User>(where(c(&User::nickname) == nickname), limit(1));
            if (!selectedUsersArray.empty()) {
                return {};
            }
            return selectedUsersArray[0];
        }

        static std::optional<int> getUserId(const std::string &nickname) {

        }

        static void insertUser(const User &user) {
            auto table = getTable();
            table.sync_schema();
            try {
                table.insert(user);
            }
            catch (std::exception &e) {
                std::cout << e.what() << '\n';
            }
        }

        static bool deleteUserByNickname(const std::string &nickname) {
            auto table = getTable();
            table.sync_schema();
            auto userID = getUserId(nickname);
            if (userID.has_value()) {
                table.remove<User>(userID.value());
                return true;
            }
            return false;
        }
    };
}


namespace Invasion::database {
    struct InterfaceDB {
        static bool login(const std::string &nickname, const std::string &password) {
            auto hashedPassword = methodsDB::getUserPassword(nickname);
            if (hashedPassword.has_value()) {
                return bcrypt::validatePassword(password, hashedPassword.value());
            }
            return false;
        }

        static bool tryToRegistationUser(const std::string &nickname, const std::string &password) {
            if (methodsDB::checkUser(nickname)) {
                return false;
            }
            std::cout << "success registration\n";
            methodsDB::insertUser(User{-1, nickname, bcrypt::generateHash(password)});
            return true;
        }

        // these methods for debugging only
        static void printUsers() {
            auto arrayUsers = getTable().get_all<User>();
            for (auto &user: arrayUsers) {
                std::cout << getTable().dump(user) << std::endl;
            }
        }

        static void deleteAllUsers() {
            getTable().sync_schema();
            getTable().remove_all<User>();
        }
    };

}
#endif //INVASION_DATABASE_H
