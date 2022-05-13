#include "server/server.h"
#include "../include/http-server/http-server.h"
int main() {
    invasion::session::Server server;
    aboba();
    server.awaitNewConnections();
}
