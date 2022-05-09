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
        invasion::server::Server server;
        std::thread serverThread([&server]() {
            server.start("127.0.0.1", 8000); // blocks the current thread of execution!
        });
        serverThread.detach();

        invasion::controllers::FixedTimeoutCallbackInvoker timeout;
        timeout.setTimeout(1000 * 10, [&serverThread, &server]() {
            server.stop();
            // serverThread.join();
        });

        // std::this_thread::sleep_for(std::chrono::seconds(6));
    }
    catch (boost::system::system_error& error) {
        std::cout << "Error while starting server: Error code = " << error.code() << ", Message: " << error.what() << std::endl;
    }
}