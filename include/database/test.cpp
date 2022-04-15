#include <iostream>
#include "database.h"

int main() {
    auto db = Database{};
    db.deleteAllUsers();
    auto allUsers = db.getArrayUsers();
    db.printUsers();
    int cnt = 0;
    while (cnt++ < 20) {
        std::string request;
        std::cin >> request;
        if (request == "login") {
            std::string nickname, pass;
            std::cin >> nickname >> pass;
            if (db.LoginUser(nickname, pass)) {
                std::cout << "password is right!" << std::endl;
            } else {
                std::cout << "Wrong password or login" << std::endl;
            }
        } else if (request == "print") {
            std::cout << "allUsers:" << std::endl;
            db.printUsers();
        } else if (request == "delete") {
            db.deleteAllUsers();
        } else if (request == "registration") {
            std::string nickname, pass;
            std::cin >> nickname >> pass;
            User user{-1, nickname, pass};
            if (db.tryAddUser(user.nickname, user.password)) {
                std::cout << "User created!" << std::endl;
            } else {
                std::cout << "ERROR!\n";
            }
        }
    }
}