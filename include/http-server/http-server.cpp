#include "http-server.h"
#include "../../3rd-party/crow_all.h"
#include "database/auth-service.h"
#include <iostream>
#include <regex>
#include "../include/database/statistic-base.h"

void HttpServer::start() {
    using namespace invasion::statistic_base;
    StatisticAccessor::clear();

    thread_ = std::move(std::thread{[]() {
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
                            } else if (!std::regex_match(password.c_str(), invalidSymbols)
                                       || !std::regex_match(nickname.c_str(), invalidSymbols)) {
                                responseJson["message"] = "Invalid Symbols";
                                return crow::response(500, responseJson);
                            } else if (AuthService::tryToRegisterUser(nickname,
                                                                      password)) {
                                StatisticAccessor::addOrUpdateLine(UserStatisticsPerMatch{nickname});
                                responseJson["message"] = "Success registration!";
                                return crow::response(200, responseJson);
                            } else {
                                responseJson["message"] = "This user already exists in the database!";
                                return crow::response(400, responseJson);
                            }

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
                            } else if (!std::regex_match(password.c_str(), invalidSymbols) ||
                                       !std::regex_match(nickname.c_str(), invalidSymbols)) {
                                responseJson["message"] = "Invalid Symbols";
                                return crow::response(500, responseJson);
                            } else if (AuthService::login(nickname, password)) {
                                responseJson["message"] = "Success entry!";
                                return crow::response(200, responseJson);
                            } else {
                                responseJson["message"] = "Wrong nickname or password!";
                                return crow::response(400, responseJson);
                            }
                        });
        CROW_ROUTE(app, "/get_statistic")
                .methods("POST"_method)
                        ([](const crow::request &rowRequest) {
                            auto requestJson = crow::json::load(rowRequest.body);
                            std::string nickname = requestJson["nickname"].s();
                            crow::json::wvalue responseJson;
                            if (!requestJson || nickname.empty()) {
                                responseJson["message"] = "Bad request";
                                return crow::response(404, responseJson);
                            }
                            using namespace invasion::statistic_base;
                            UserStatistics stat = StatisticAccessor::getUserStatistic(nickname);
                            responseJson["total_kills"] = stat.totalKills;
                            responseJson["total_deaths"] = stat.totalDeaths;
                            responseJson["win_rate"] = stat.winRate;
                            responseJson["number_of_matches"] = stat.numberOfMatches;
                            responseJson["number_of_wins"] = stat.numberWins;
                            return crow::response(200, responseJson);
                        });
        app.port(5555).multithreaded().run();
    }});
    thread_.detach();
}

void HttpServer::stop() {
    thread_.join();
}
