
#ifndef INVASION_SERVER_RECEIVER_H
#define INVASION_SERVER_RECEIVER_H

#include <thread>
#include "user.h"
#include "safe-queue.h"
#include <player.pb.h>
#include <memory>
#include <optional>
#include "network_packet.h"
#include "player-position-response-model.pb.h"

namespace invasion::session {
    class ReceiverFromUser {
    private:
        inline static std::optional<NetworkPacketRequest> readFromClient(std::shared_ptr<User> client) {
            std::uint32_t size;  // get the message data length in bytes
            client->channel.read(reinterpret_cast<char *> (&size), sizeof(size));
            std::uint32_t messageType; // get the message type
            client->channel.read(reinterpret_cast<char *> (&messageType), sizeof(messageType));

            std::unique_ptr<char> buffer_ptr(new char[size]);
            NetworkPacketRequest packet(std::move(buffer_ptr), NetworkPacketRequest::getMessageTypeById(messageType),
                                        size);
            client->channel.read(reinterpret_cast<char *> (packet.getStoredBytes()), size);
            
            if (!client->channel) {
                return {};
            }

            return packet;
        }

    public:
        ReceiverFromUser(std::shared_ptr<User> cur_client, SafeQueue<NetworkPacketRequest> *queueServerFromClients) {
            std::thread([client = std::move(cur_client), q = queueServerFromClients]() {
                while (client->channel) {
                    // get data from client
                    std::optional<NetworkPacketRequest> request = ReceiverFromUser::readFromClient(client);
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
