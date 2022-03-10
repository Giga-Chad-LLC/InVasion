
#ifndef INVASION_SERVER_RECEIVER_H
#define INVASION_SERVER_RECEIVER_H

#include <thread>
#include "user.h"
#include "safe-queue.h"
#include <player.pb.h>
#include "player-position-response-model.pb.h"

namespace inVasion::session {
    class ReceiverFromUser {

    public:
        ReceiverFromUser(std::shared_ptr<User> cur_client, SafeQueue<PlayerAction> *queueOnReceive) {
            std::thread([client = std::move(cur_client), q = queueOnReceive]() {
                while (client->channel) {

                    std::uint32_t size;  // get the message data length in bytes
                    client->channel.read(reinterpret_cast<char *> (&size), sizeof(size));
                    std::uint32_t type;
                    client->channel.read(reinterpret_cast<char *> (&type), sizeof(type));
                    char arr[size];
                    client->channel.read(reinterpret_cast<char *> (&arr), size);

                    if (type == 2) {
                        PlayerAction action;
                        action.ParseFromArray(arr, size);
                        q->produce(std::move(action));    
                    }
                    else {
                        std::cout << "Unknown type: " << type << std::endl;
                    }
                }
                std::cout << "Client disconnected" << std::endl;
            }).detach();
        }
    };
}


#endif //INVASION_SERVER_RECEIVER_H
