#ifndef INVASION_DATABASE_H
#define INVASION_DATABASE_H

#include <sqlite3.h>
#include <functional>
#include <optional>

// libs
#include "libs/sqlite_orm/sqlite_orm.h"


struct User {
    int id;
    std::string username;
    std::string password;
};


namespace invasion::database_access{
	using namespace sqlite_orm;
	
    class DatabaseManager {
    public:
        static auto &getTable() {
            auto table = make_table("users",
                                    make_column("id", &User::id, primary_key(), autoincrement()),
                                    make_column("username", &User::username, unique()),
                                    make_column("hashed password", &User::password));
            static auto storage_ = make_storage("db.sqlite", table);
            storage_.sync_schema();
            return storage_;
        }
    };

    class DatabaseAccessor {

    public:
        static std::optional<std::string> getUserPassword(const std::string &username) {
            auto table = DatabaseManager::getTable();
            auto selectedUsersArray = table.select(&User::password, where(c(&User::username) == username), limit(1));
            if (selectedUsersArray.empty()) {
                return {};
            }
            return selectedUsersArray[0];
        }

        static bool checkUser(const std::string &username) {
            auto table = DatabaseManager::getTable();
            return getUserPassword(username).has_value();
        }

        static std::optional<User> getUser(const std::string &username) {
            auto table = DatabaseManager::getTable();
            auto selectedUsersArray = table.get_all<User>(where(c(&User::username) == username), limit(1));
            if (!selectedUsersArray.empty()) {
                return {};
            }
            return selectedUsersArray[0];
        }


        static void insertUser(const User &user) {
            auto table = DatabaseManager::getTable();
            table.insert(user);
        }

        static bool deleteUserByUsername(const std::string &username) {
            auto table = DatabaseManager::getTable();
            auto userID = getUser(username);
            if (userID.has_value()) {
                table.remove<User>(userID.value().id);
                return true;
            }
            return false;
        }


        // ======= debug-only ========


        static void printAll() {
            auto arrayUsers = DatabaseManager::getTable().get_all<User>();
            for (auto &user: arrayUsers) {
                std::cout << DatabaseManager::getTable().dump(user) << std::endl;
            }
        }

        static void deleteAll() {
            DatabaseManager::getTable().remove_all<User>();
            DatabaseManager::getTable().sync_schema();
        }

    };
}


#endif //INVASION_DATABASE_H
