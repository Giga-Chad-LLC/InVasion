#include <exception>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <thread>
#include <system_error>

// controllers
#include "controllers/FixedTimeoutCallbackInvoker/fixed-timeout-callback-invoker.h"
// http-server
#include "http-server/HttpServer/http-server.h"
// server (tcp)
#include "server/Server/server.h"


int main() {
    try {
        bool shouldStop = false;

        std::string host = "127.0.0.1"; // 192.168.1.71
        short tcp_port = 8000;
        short http_port = 5555;
        
        invasion::server::Server server;
        invasion::http_server::HttpServer httpServer;
        
        if (!shouldStop) {
            httpServer.start(host, http_port);
            server.start(host, tcp_port); // blocks the current thread of execution!
        }
        else {
            std::thread serverThread([&server, &httpServer, host, tcp_port, http_port]() {
                httpServer.start(host, http_port);
                server.start(host, tcp_port); // blocks the current thread of execution!
            });

            serverThread.detach();

            invasion::controllers::FixedTimeoutCallbackInvoker timeout;
            timeout.setTimeout(1000 * 20, [&serverThread, &server, &httpServer]() {
                httpServer.stop();
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