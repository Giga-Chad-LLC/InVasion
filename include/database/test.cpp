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
            std::string username, pass;
            std::cin >> username >> pass;
            if (AuthService::login(username, pass)) {
                std::cout << "Successful login\n";
            } else {
                std::cout << "Wrong username or password\n";
            }
        } else if (request == "print") {
            std::cout << "allUsers:" << std::endl;
            AuthService::printUsers();
        } else if (request == "delete") {
            AuthService::deleteAllUsers();
        } else if (request == "reg") {
            std::string username, pass;
            std::cin >> username >> pass;
            AuthService::tryToRegisterUser(username, pass);
        }
    }
}