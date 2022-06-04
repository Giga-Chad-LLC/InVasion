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
        bool shouldPrompt = true;

        std::string host = "127.0.0.1"; // 192.168.1.71
        short tcp_port = 8000;
        short http_port = 8001;

        if (shouldPrompt) {
            std::string in_host;
            short in_tcp_port;
            short in_http_port;
            
            std::cout << "Enter 0 for default values" << std::endl;

            std::cout << "Enter host ip address (default '127.0.0.1'):" << std::endl;
            std::cin >> in_host;
            if (in_host != "0") {
                host = in_host;
            }


            std::cout << "Enter TCP server port (default: '8000'):" << std::endl;
            std::cin >> in_tcp_port;
            if (in_tcp_port != 0) {
                tcp_port = in_tcp_port;
            }

            
            std::cout << "Enter HTTP server port (default: '8001'):" << std::endl;
            std::cin >> in_http_port;
            if (in_http_port != 0) {
                http_port = in_http_port;
            }
        }
        
        invasion::server::Server server;
        invasion::http_server::HttpServer httpServer;

        std::cout << "Host: " << host << std::endl;
        std::cout << "TCP port: " << tcp_port << std::endl;
        std::cout << "HTTP port: " << http_port << std::endl;

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