#include "../../3rd-party/crow_all.h"
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
                        if (!requestJson || requestJson["nickname"].s() == ""|| requestJson["password"].s()=="") {
                            responseJson["message"] = "Bad request";
                            return crow::response(404, responseJson);
                        } else if (AuthService::tryToRegisterUser(requestJson["nickname"].s(),
                                                                     requestJson["password"].s())) {
                            responseJson["message"] = "Success registration!";
                        } else {
                            responseJson["message"] = "This user already exists in the database!";
                            return crow::response(400, responseJson);
                        }
                        std::cout << requestJson["nickname"].s() << " " << requestJson["password"].s() << std::endl;
                        return crow::response(200, responseJson);
                    });
    CROW_ROUTE(app, "/login")
            .methods("GET"_method)
                    ([](const crow::request &rowRequest) {
                        auto requestJson = crow::json::load(rowRequest.body);
                        crow::json::wvalue responseJson;
                        if (!requestJson || requestJson["nickname"].s() == ""|| requestJson["password"].s()=="") {
                            responseJson["message"] = "Bad request";
                            return crow::response(404, responseJson);
                        } else if (AuthService::login(requestJson["nickname"].s(), requestJson["password"].s())) {
                            responseJson["message"] = "Success entry!";
                            return crow::response(200, responseJson);
                        } else {
                            responseJson["message"] = "Wrong nickname or password!";
                            return crow::response(400, responseJson);
                        }
                    });
    app.port(5555).multithreaded().run();
}