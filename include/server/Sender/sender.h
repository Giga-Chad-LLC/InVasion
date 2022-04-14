
#ifndef INVASION_SERVER_SENDER_H
#define INVASION_SERVER_SENDER_H

#include "server/Client/client.h"

namespace invasion::session {
    class ClientResponsesSender {
    public:
        ClientResponsesSender(std::shared_ptr<Client> client);
    };
}

#endif //INVASION_SERVER_SENDER_H
