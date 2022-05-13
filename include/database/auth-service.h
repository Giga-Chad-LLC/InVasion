
#ifndef INVASION_AUTH_SERVICE_H
#define INVASION_AUTH_SERVICE_H

#include "database.h"

namespace invasion::database_interface {
    using namespace database_access;

    struct AuthService {
        static bool login(const std::string &nickname, const std::string &password) {
            auto hashedPassword = DatabaseAccessor::getUserPassword(nickname);
            if (hashedPassword.has_value()) {
                return bcrypt::validatePassword(password, hashedPassword.value());
            }
            return false;
        }

        static bool tryToRegisterUser(const std::string &nickname, const std::string &password) {
            if (DatabaseAccessor::checkUser(nickname)) {
                return false;
            }
            std::cout << "success registration\n";
            DatabaseAccessor::insertUser(User{-1, nickname, bcrypt::generateHash(password)});
            return true;
        }

        // these methods for debugging only
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

#endif //INVASION_AUTH_SERVICE_H
