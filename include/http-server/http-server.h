
#ifndef INVASIONSERVERS_HTTP_SERVER_H
#define INVASIONSERVERS_HTTP_SERVER_H

#include <iostream>
#include <regex>
#include <thread>

namespace invasion::http_server {
    class HttpServer {
    private:
        std::thread thread_;

    public:

        void start();

        void stop();
    };
}
#endif//INVASIONSERVERS_HTTP_SERVER_H
