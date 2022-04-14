#include "server/server.h"

int main() {
    invasion::session::Server server;
    server.awaitNewConnections();
}
