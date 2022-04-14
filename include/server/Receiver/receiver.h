
#ifndef INVASION_SERVER_RECEIVER_H
#define INVASION_SERVER_RECEIVER_H

#include "server/Client/client.h"
#include "server/NetworkPacket/network-packet.h"
#include "server/safe-queue.h"
#include <memory>
#include <optional>

namespace invasion::session {
    class ClientRequestsReceiver {
    private:
        // reads data from clients in format: [4 bytes: message length in bytes][4 bytes: uint32_t message type][length bytes: data itself in byte representation]
        static std::optional<NetworkPacketRequest> getRequestFromClient(std::shared_ptr<Client> client);
    public:
        ClientRequestsReceiver(std::shared_ptr<Client> client, SafeQueue<NetworkPacketRequest> *requestQueue);
    };
}


#endif //INVASION_SERVER_RECEIVER_H
