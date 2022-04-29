#include <iostream>
#include "database.h"

int main() {
    using namespace Invasion::database;
    InterfaceDB::deleteAllUsers();
    int cnt = 0;
    while (cnt++ < 20) {
        std::string request;
        std::cin >> request;
        if (request == "login") {
            std::string nickname, pass;
            std::cin >> nickname >> pass;
            if (InterfaceDB::login(nickname, pass)) {
                std::cout << "Successful login\n";
            } else {
                std::cout << "Wrong nickname or password\n";
            }
        } else if (request == "print") {
            std::cout << "allUsers:" << std::endl;
            InterfaceDB::printUsers();
        } else if (request == "delete") {
            InterfaceDB::deleteAllUsers();
        } else if (request == "reg") {
            std::string nickname, pass;
            std::cin >> nickname >> pass;
            InterfaceDB::tryToRegistationUser(nickname, pass);
        }
    }
}