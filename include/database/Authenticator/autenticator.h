
#ifndef INVASIONSERVERS_AUTENTICATOR_H_
#define INVASIONSERVERS_AUTENTICATOR_H_

#include <cassert>

// libs
#include "libs/bcrypt/bcrypt.h"
#include "libs/sqlite_orm/sqlite_orm.h"

struct UserInfo {
    int id;
    std::string username;
    std::string token;
};


namespace invasion::token_authenticator {
	using namespace sqlite_orm;

    class DatabaseManager {
    public:
        static auto &getTable() {
            auto table = make_table("tokens",
                                    make_column("id", &UserInfo::id, primary_key(), autoincrement()),
                                    make_column("username", &UserInfo::username, unique()),
                                    make_column("token", &UserInfo::token));
            static auto storage_ = make_storage("db.sqlite", table);
            storage_.sync_schema();
            return storage_;
        }
    };

    class Authenticator {
    private:
        static void putTokenInTable(const std::string &username, const std::string &token) {
            auto &table = DatabaseManager::getTable();
            table.insert(UserInfo{1, username, token});
        }

        static std::string generateToken(const std::string &username) {
            std::string token = bcrypt::generateHash(username);
            return token;
        }

    public:

        static std::string createNewToken(const std::string &username) {
            std::string token = generateToken(username);
            putTokenInTable(username, token);
            return token;
        }

        static std::string refreshOldToken(const std::string &username) {
            auto &table = DatabaseManager::getTable();
            table.begin_transaction();
            auto user = table.get_all<UserInfo>(
                    where(c(&UserInfo::username) == username), limit(1))[0];

            std::string newToken = generateToken(username);
            user.token = newToken;
            table.update(user);
            table.commit();
            return newToken;
        }

        static bool checkTokenMatch(const std::string &username, const std::string &token){
            auto &table = DatabaseManager::getTable();
            auto tokenColumn = table.select(&UserInfo::token,
                    where(c(&UserInfo::username) == username), limit(1)); // [0]

            if (tokenColumn.empty()) {
                return false;
            }

            return tokenColumn.at(0) == token;
        }

        // =========== debug-only ==============

        static void printAll() {
            auto arrayUsers = DatabaseManager::getTable().get_all<UserInfo>();
            for (auto &user: arrayUsers) {
                std::cout << DatabaseManager::getTable().dump(user) << std::endl;
            }
        }

        static void deleteAll() {
            DatabaseManager::getTable().remove_all<UserInfo>();
            DatabaseManager::getTable().sync_schema();
        }
    };
}

#endif //INVASIONSERVERS_AUTENTICATOR_H_
