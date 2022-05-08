#include "server/Server/server.h"
#include <exception>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <system_error>


int main() {
    try {
        invasion::server::Server server;
        server.start("127.0.0.1", 8000);
        system("pause");
        // std::this_thread::sleep_for(std::chrono::seconds(60));
    }
    catch (boost::system::system_error& error) {
        std::cout << "Error while starting server: Error code = " << error.code() << ", Message: " << error.what() << std::endl;
    }

}
