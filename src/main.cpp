#include <exception>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <thread>
#include <system_error>

#include "server/Server/server.h"
#include "controllers/FixedTimeoutCallbackInvoker/fixed-timeout-callback-invoker.h"


int main() {
    try {
        bool shouldStop = false;
        invasion::server::Server server;
        
        if (!shouldStop) {
            server.start("192.168.0.17", 8000); // blocks the current thread of execution!
        }
        else {
            std::thread serverThread([&server]() {
                server.start("127.0.0.1", 8000); // blocks the current thread of execution!
            });
            serverThread.detach();

            invasion::controllers::FixedTimeoutCallbackInvoker timeout;
            timeout.setTimeout(1000 * 20, [&serverThread, &server]() {
                server.stop();
            });
        }
    }
    catch (const boost::system::system_error& error) {
        std::cout << "Error while running server: Error code = " << error.code() << ", Message: " << error.what() << std::endl;
    }
    catch (const std::exception& error) {
        std::cout << "Error while running server: " << "Message: " << error.what() << std::endl;
    }
}