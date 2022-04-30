
#ifndef INVASIONSERVERS_CONNECTEDCLIENTS_H

#include <vector>
#include <memory>
#include "server/Client/client.h"

namespace invasion::session {
    class ConnectedClients {
    public:
        static std::vector<std::shared_ptr<Client>> &getConnectedClients() noexcept {
            static std::vector<std::shared_ptr<Client>> data;
            return data;
        }
    };
}
#define INVASIONSERVERS_CONNECTEDCLIENTS_H

#endif //INVASIONSERVERS_CONNECTEDCLIENTS_H
