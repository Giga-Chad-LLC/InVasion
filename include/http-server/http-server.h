
#ifndef INVASIONSERVERS_HTTP_SERVER_H
#define INVASIONSERVERS_HTTP_SERVER_H
#include "../../3rd-party/crow_all.h"
#include "../../include/database/auth-service.h"
#include <iostream>
#include <regex>

void aboba() {
    std::thread{[]() {
        static const std::regex invalidSymbols(R"([a-zA-Z0-9]*)");
    using namespace invasion::database_access;
    using namespace invasion::database_interface;
    AuthService::deleteAllUsers();
    crow::SimpleApp app;
    CROW_ROUTE(app, "/registration")
            .methods("POST"_method)
                    ([](const crow::request &rowRequest) {
                        auto requestJson = crow::json::load(rowRequest.body);
                      std::string password = requestJson["password"].s();
                      std::string nickname = requestJson["nickname"].s();
                      crow::json::wvalue responseJson;
                        if (!requestJson || nickname.empty() || password.empty()) {
                            responseJson["message"] = "Bad request";
                            return crow::response(404, responseJson);
                        }
                        else if (!std::regex_match(password.c_str(), invalidSymbols)
                                 || !std::regex_match(nickname.c_str(), invalidSymbols)){
                            responseJson["message"] = "Invalid Symbols";
                            return crow::response(500, responseJson);
                        }
                    else if (AuthService::tryToRegisterUser(nickname,
                                                                  password)) {
                            responseJson["message"] = "Success registration!";
                        } else {
                            responseJson["message"] = "This user already exists in the database!";
                            return crow::response(400, responseJson);
                        }
                        return crow::response(200, responseJson);
                    });
    CROW_ROUTE(app, "/login")
            .methods("GET"_method)
                    ([](const crow::request &rowRequest) {
                        auto requestJson = crow::json::load(rowRequest.body);
                        crow::json::wvalue responseJson;
                        std::string password = requestJson["password"].s();
                        std::string nickname = requestJson["nickname"].s();
                        if (!requestJson || nickname.empty() || password.empty()) {
                            responseJson["message"] = "Bad request";
                            return crow::response(404, responseJson);
                        } else if (!std::regex_match(password.c_str(), invalidSymbols) || !std::regex_match(nickname.c_str(), invalidSymbols)){
                            responseJson["message"] = "Invalid Symbols";
                            return crow::response(500, responseJson);
                        }else if (AuthService::login(nickname, password)) {
                            responseJson["message"] = "Success entry!";
                            return crow::response(200, responseJson);
                        } else {
                            responseJson["message"] = "Wrong nickname or password!";
                            return crow::response(400, responseJson);
                        }
                    });
    app.port(5555).multithreaded().run(); }}.detach();
}
#endif//INVASIONSERVERS_HTTP_SERVER_H
