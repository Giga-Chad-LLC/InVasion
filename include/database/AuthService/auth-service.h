
#ifndef INVASION_AUTH_SERVICE_H_
#define INVASION_AUTH_SERVICE_H_

#include <iostream>

// libs
#include "libs/bcrypt/bcrypt.h"
// database
#include "database/DatabaseAccessor/database-accessor.h"

namespace invasion::database_interface {
    using namespace database_access;

    struct AuthService {
        static bool login(const std::string &username, const std::string &password) {
            auto hashedPassword = DatabaseAccessor::getUserPassword(username);
            if (hashedPassword.has_value()) {
                return bcrypt::validatePassword(password, hashedPassword.value());
            }
            return false;
        }

        static bool tryToRegisterUser(const std::string &username, const std::string &password) {
            if (DatabaseAccessor::checkUser(username)) {
                return false;
            }
            std::cout << "successful registration\n";
            DatabaseAccessor::insertUser(User{-1, username, bcrypt::generateHash(password)});
            return true;
        }

        // ====== debug-only =======

        static void printUsers() {
            auto arrayUsers = DatabaseManager::getTable().get_all<User>();
            for (auto &user: arrayUsers) {
                std::cout << DatabaseManager::getTable().dump(user) << std::endl;
            }
        }

        static void deleteAllUsers() {
            DatabaseManager::getTable().remove_all<User>();
            DatabaseManager::getTable().sync_schema();
        }
    };

}

#endif //INVASION_AUTH_SERVICE_H_
