#include "receiver.h"
#include <thread>
#include <player.pb.h>
#include "player-position-response-model.pb.h"


namespace invasion::session {
    ClientRequestsReceiver::ClientRequestsReceiver(std::shared_ptr<Client> client,
                                                   SafeQueue<std::shared_ptr<NetworkPacketRequest>> *requestQueue) {
        std::thread([client = std::move(client), q = requestQueue]() {
            while (client->getChannel()) {
                // get data from client
                auto request = ClientRequestsReceiver::getRequestFromClient(client);
                if (request.has_value()) {
                    q->produce(std::move(request.value()));
                }
            }
            std::cout << "Client disconnected" << std::endl;
        }).detach();
    }


    std::optional<std::shared_ptr<NetworkPacketRequest>>
    ClientRequestsReceiver::getRequestFromClient(std::shared_ptr<Client> client) {
        std::uint32_t size;  // get the message data length in bytes
        client->getChannel().read(reinterpret_cast<char *> (&size), sizeof(size));
        std::uint32_t messageType; // get the message type
        client->getChannel().read(reinterpret_cast<char *> (&messageType), sizeof(messageType));

        std::unique_ptr<char[]> buffer_ptr(new char[size]);
        auto packet = std::make_shared<NetworkPacketRequest>(std::move(buffer_ptr),
                                                             NetworkPacketRequest::getMessageTypeById(messageType),
                                                             size);
        client->getChannel().read(reinterpret_cast<char *> (packet->getStoredBytes()), size);

        if (!client->getChannel()) {
            return {};
        }

        return packet;
    }


}