
#ifndef INVASION_SERVER_RECEIVER_H
#define INVASION_SERVER_RECEIVER_H

#include <thread>
#include "user.h"
#include "safe-queue.h"
#include <player.pb.h>
#include <memory>
#include "network_packet.h"
#include "player-position-response-model.pb.h"

namespace invasion::session {
    class ReceiverFromUser {
    private:
        std::shared_ptr<User> clientPointer;

        NetworkPacketRequest readClient() {
            std::uint32_t size;  // get the message data length in bytes
            clientPointer->channel.read(reinterpret_cast<char *> (&size), sizeof(size));
            std::uint32_t messageType; // get the message type
            clientPointer->channel.read(reinterpret_cast<char *> (&messageType), sizeof(messageType));

            std::unique_ptr<char> buffer_ptr(new char[size]);
            NetworkPacketRequest packet(std::move(buffer_ptr), NetworkPacketRequest::getMessageTypeById(messageType),
                                        size);
            packet.setPlayerId(clientPointer->getIdClient()); //set idPlayer
            clientPointer->channel.read(reinterpret_cast<char *> (packet.getStoredBytes()), size);
            return packet;
        }

    public:
        ReceiverFromUser(std::shared_ptr<User> cur_client, SafeQueue<NetworkPacketRequest> *queueServerFromClients)
                : clientPointer(cur_client) {
            std::thread([client = std::move(cur_client), q = queueServerFromClients, this]() {
                while (client->channel) {
                    // get data from client
                    auto packet = readClient();
                    q->produce(std::move(packet));
                }
                std::cout << "Client disconnected" << std::endl;
            }).detach();
        }
    };
}


#endif //INVASION_SERVER_RECEIVER_H
