#include "server/server.h"
#include "../include/http-server/http-server.h"
int main() {
    invasion::session::Server server;
    HttpServer httpServer;
    httpServer.start();
    server.awaitNewConnections();
}
