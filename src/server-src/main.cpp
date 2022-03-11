#include "../../include/server-include/server.h"

int main() {
    invasion::session::Server curServer;
    curServer.waitNewUser();
}
