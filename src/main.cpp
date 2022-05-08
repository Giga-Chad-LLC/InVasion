#include "server/Server/server.h"
#include <exception>
#include <iostream>
#include <system_error>


int main() {
    try {
        std::cout << "Creating server" << std::endl;
        invasion::server::Server server("127.0.0.1", 8000);
        server.start();
        system("pause");
    }
    catch (const std::exception& error) {
        std::cout << "Error while starting server. Message: " << error.what() << std::endl;
    }

}
