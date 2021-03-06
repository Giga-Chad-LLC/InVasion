
#ifndef INVASIONSERVERS_HTTP_SERVER_H_
#define INVASIONSERVERS_HTTP_SERVER_H_

#include <iostream>
#include <regex>
#include <thread>
#include <string>
// libs
#include "libs/crow/crow_all.h"

namespace invasion::http_server {
    class HttpServer {
    private:
        std::thread m_thread;
        crow::SimpleApp m_app;
    public:
        void start(std::string host = "127.0.0.1", short port = 5555);
        void stop();
    };
}

#endif // INVASIONSERVERS_HTTP_SERVER_H_
