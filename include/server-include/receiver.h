
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

    public:
        ReceiverFromUser(std::shared_ptr<User> cur_client, SafeQueue<NetworkPacketRequest> *queueServerFromClients) {
            std::thread([client = std::move(cur_client), q = queueServerFromClients]() {
                while (client->channel) {
                    // get data from client
                    std::uint32_t size;  // get the message data length in bytes
                    client->channel.read(reinterpret_cast<char *> (&size), sizeof(size));
                    std::uint32_t messageType; // get the message type
                    client->channel.read(reinterpret_cast<char *> (&messageType), sizeof(messageType));
                    
                    std::unique_ptr <char> buffer_ptr(new char[size]);
                    NetworkPacketRequest packet(std::move(buffer_ptr), NetworkPacketRequest::getMessageTypeById(messageType), size);
                    packet.setPlayerId(client->getIdClient()); //set idPlayer
                    client->channel.read(reinterpret_cast<char*> (packet.getStoredBytes()), size);

                    q->produce(std::move(packet));
                }
                std::cout << "Client disconnected" << std::endl;
            }).detach();
        }
    };
}


#endif //INVASION_SERVER_RECEIVER_H
