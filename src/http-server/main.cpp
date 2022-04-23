#include "crow_all.h"
#include <iostream>

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/registration")([](){
        return "Hello world";
    });

    app.port(5555).multithreaded().run();
}