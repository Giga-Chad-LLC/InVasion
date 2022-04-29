#include "crow_all.h"
#include "../../include/database/database.h"
#include <iostream>

int main() {
    using namespace Invasion::database;
    InterfaceDB::deleteAllUsers();
    crow::SimpleApp app;
    CROW_ROUTE(app, "/registration")
            .methods("POST"_method)
                    ([](const crow::request &rowRequest) {
                        auto requestJson = crow::json::load(rowRequest.body);
                        crow::json::wvalue responseJson;
                        if (!requestJson) {
                            responseJson["status code"] = 404;
                            responseJson["message"] = "Bad request";
                        } else if (InterfaceDB::tryToRegistationUser(requestJson["nickname"].s(),
                                                                     requestJson["password"].s())) {
                            responseJson["status code"] = 200;
                            responseJson["message"] = "Success registration!";
                        } else {
                            responseJson["status code"] = 400;
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
                        if (!requestJson) {
                            responseJson["status code"] = 404;
                            responseJson["message"] = "Bad request";
                        } else if (InterfaceDB::login(requestJson["nickname"].s(), requestJson["password"].s())) {
                            responseJson["status code"] = 200;
                            responseJson["message"] = "Success entry!";
                        } else {
                            responseJson["status code"] = 400;
                            responseJson["message"] = "Wrong nickname or password!";
                        }
                        return responseJson;
                    });
    app.port(5555).multithreaded().run();
}