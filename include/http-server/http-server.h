
#ifndef INVASIONSERVERS_HTTP_SERVER_H
#define INVASIONSERVERS_HTTP_SERVER_H

#include <iostream>
#include <regex>
#include <thread>


class HttpServer {
private:
    std::thread thread_;

public:
    HttpServer() {}

    void start();

    void stop();
};
#endif//INVASIONSERVERS_HTTP_SERVER_H
