#ifndef INVASION_DATABASE_H
#define INVASION_DATABASE_H

#include <string>
#include <sqlite3.h>
#include "sqlite_orm.h"

struct User {
    int id;
    std::string nickname;
    std::string password;
};

using namespace sqlite_orm;
auto& getDatabase() {
    static auto storage_ = make_storage("db.sqlite",
                                make_table("users",
                                           make_column("id", &User::id, autoincrement(), primary_key()),
                                           make_column("nickname", &User::nickname),
                                           make_column("hashed password", &User::password)));
    return storage_;
}


bool tryAddUser(std::string nickname, std::string password){
    try{
        auto cntUsers = getDatabase().get_all<User>().size();
        if (cntUsers == 0){
            User user{-1, std::move(nickname), std::move(password)};
            getDatabase().insert(user);
            return true;
        }
        auto listAllUsers = getDatabase().get_all<User>(where(c(&User::nickname) != nickname));
        if (listAllUsers.size() == cntUsers){
            User user{-1, nickname, password};
            getDatabase().insert(user);
            return true;
        }
        return false;
    }
    catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }
}



#endif //INVASION_DATABASE_H
