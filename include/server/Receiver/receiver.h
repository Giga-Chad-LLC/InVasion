
#ifndef INVASION_SERVER_RECEIVER_H
#define INVASION_SERVER_RECEIVER_H

#include "server/Client/client.h"
#include "server/NetworkPacket/network-packet.h"
#include "server/safe-queue.h"
#include <memory>
#include <optional>
#include <utility>

namespace invasion::session {
    class ClientRequestsReceiver {
    private:
        // reads data from clients in format: [4 bytes: message length in bytes][4 bytes: uint32_t message type][length bytes: data itself in byte representation]
        static std::optional<std::shared_ptr<NetworkPacketRequest>>
        getRequestFromClient(std::shared_ptr<Client> client);

        SafeQueue<std::shared_ptr<NetworkPacketRequest>> *RefRequestQueue;
        std::shared_ptr<Client> ptrClient;
        std::thread thread_;
    public:
        ClientRequestsReceiver(std::shared_ptr<Client> &client,
                               SafeQueue<std::shared_ptr<NetworkPacketRequest>> *requestQueue) : ptrClient(client),
                                                                                                 RefRequestQueue(
                                                                                                         requestQueue) {}

        void start();

        void stop();
    };
}


#endif //INVASION_SERVER_RECEIVER_H
