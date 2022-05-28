
#ifndef INVASIONSERVERS_AUTENTICATOR_H
#define INVASIONSERVERS_AUTENTICATOR_H

#include "../../../3rd-party/common-header.h"
#include "cassert"

struct UserInfo {
    int id;
    std::string nickname;
    std::string token;
};


namespace invasion::token_authenticator {
	using namespace sqlite_orm;

    class DatabaseManager {
    public:
        static auto &getTable() {
            auto table = make_table("tokens",
                                    make_column("id", &UserInfo::id, primary_key(), autoincrement()),
                                    make_column("nickname", &UserInfo::nickname, unique()),
                                    make_column("token", &UserInfo::token));
            static auto storage_ = make_storage("db.sqlite", table);
            storage_.sync_schema();
            return storage_;
        }
    };

    class Authenticator {
    private:
        static void putTokenInTable(const std::string &nickname, const std::string &token) {
            auto &table = DatabaseManager::getTable();
            table.insert(UserInfo{1, nickname, token});
        }

        static std::string generateToken(const std::string &nickname) {
            std::string token = bcrypt::generateHash(nickname);
            return token;
        }

    public:

        static std::string createNewToken(const std::string &nickname) {
            std::string token = generateToken(nickname);
            putTokenInTable(nickname, token);
            return token;
        }

        static std::string refreshOldToken(const std::string &nickname) {
            auto &table = DatabaseManager::getTable();
            table.begin_transaction();
            auto user = table.get_all<UserInfo>(
                    where(c(&UserInfo::nickname) == nickname), limit(1))[0];

            std::string newToken = generateToken(nickname);
            user.token = newToken;
            table.update(user);
            table.commit();
            return newToken;
        }

        static bool checkTokenMatch(const std::string &nickname, const std::string &token){
            auto &table = DatabaseManager::getTable();
            auto userToken = table.select(&UserInfo::token,
                    where(c(&UserInfo::nickname) == nickname), limit(1))[0];
            return userToken == token;
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
#endif //INVASIONSERVERS_AUTENTICATOR_H
