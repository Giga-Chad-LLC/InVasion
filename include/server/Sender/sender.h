
#ifndef INVASION_SERVER_SENDER_H
#define INVASION_SERVER_SENDER_H

#include "server/Client/client.h"
#include <functional>


namespace invasion::session {

    class ClientResponsesSender {
    private:
        std::shared_ptr<Client> ptrClient;
        std::thread thread_;
    public:
        ClientResponsesSender(const std::shared_ptr<Client> client_) : ptrClient(client_) {
        }

        void start();

        void stop();
    };
}

#endif //INVASION_SERVER_SENDER_H
