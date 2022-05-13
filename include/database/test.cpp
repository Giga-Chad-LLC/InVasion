#include <iostream>
#include "auth-service.h"

int main() {
    using namespace invasion::database_interface;
    AuthService::deleteAllUsers();
    int cnt = 0;
    while (cnt++ < 20) {
        std::string request;
        std::cin >> request;
        if (request == "login") {
            std::string nickname, pass;
            std::cin >> nickname >> pass;
            if (AuthService::login(nickname, pass)) {
                std::cout << "Successful login\n";
            } else {
                std::cout << "Wrong nickname or password\n";
            }
        } else if (request == "print") {
            std::cout << "allUsers:" << std::endl;
            AuthService::printUsers();
        } else if (request == "delete") {
            AuthService::deleteAllUsers();
        } else if (request == "reg") {
            std::string nickname, pass;
            std::cin >> nickname >> pass;
            AuthService::tryToRegisterUser(nickname, pass);
        }
    }
}