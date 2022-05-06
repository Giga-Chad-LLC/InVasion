#include "crow_all.h"
#include "../../include/database/auth-service.h"
#include <iostream>

int main() {
    using namespace invasion::database_access;
    using namespace invasion::database_interface;
    AuthService::deleteAllUsers();
    crow::SimpleApp app;
    CROW_ROUTE(app, "/registration")
            .methods("POST"_method)
                    ([](const crow::request &rowRequest) {
                        auto requestJson = crow::json::load(rowRequest.body);
                        crow::json::wvalue responseJson;
                        if (!requestJson) {
                            responseJson["message"] = "Bad request";
                        } else if (AuthService::tryToRegisterUser(requestJson["nickname"].s(),
                                                                     requestJson["password"].s())) {
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
                        if (!requestJson) {
                            responseJson["message"] = "Bad request";
                        } else if (AuthService::login(requestJson["nickname"].s(), requestJson["password"].s())) {
                            responseJson["message"] = "Success entry!";
                        } else {
                            responseJson["message"] = "Wrong nickname or password!";
                        }
                        return responseJson;
                    });
    app.port(5555).multithreaded().run();
}