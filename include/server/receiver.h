
#ifndef INVASION_SERVER_RECEIVER_H
#define INVASION_SERVER_RECEIVER_H

#include <thread>
#include "client.h"
#include "safe-queue.h"
#include <player.pb.h>
#include <memory>
#include <optional>
#include "network-packet.h"
#include "player-position-response-model.pb.h"

namespace invasion::session {
    class ClientRequestsReceiver {
    private:
        // reads data from clients in format: [4 bytes: message length in bytes][4 bytes: uint32_t message type][length bytes: data itself in byte representation]
        inline static std::optional<NetworkPacketRequest> getRequestFromClient(std::shared_ptr<Client> client) {
            std::uint32_t size;  // get the message data length in bytes
            client->m_channel.read(reinterpret_cast<char *> (&size), sizeof(size));
            std::uint32_t messageType; // get the message type
            client->m_channel.read(reinterpret_cast<char *> (&messageType), sizeof(messageType));

            std::unique_ptr<char> buffer_ptr(new char[size]);
            NetworkPacketRequest packet(std::move(buffer_ptr), NetworkPacketRequest::getMessageTypeById(messageType),
                                        size);
            client->m_channel.read(reinterpret_cast<char *> (packet.getStoredBytes()), size);
            
            if (!client->m_channel) {
                return {};
            }

            return packet;
        }

    public:
        ClientRequestsReceiver(std::shared_ptr<Client> client, SafeQueue<NetworkPacketRequest> *requestQueue) {
            std::thread([client = std::move(client), q = requestQueue]() {
                while (client->m_channel) {
                    // get data from client
                    auto request = ClientRequestsReceiver::getRequestFromClient(client);
                    if (request.has_value()) {
                        q->produce(std::move(request.value()));
                    }
                }
                std::cout << "Client disconnected" << std::endl;
            }).detach();
        }
    };
}


#endif //INVASION_SERVER_RECEIVER_H
