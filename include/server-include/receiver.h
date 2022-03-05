
#ifndef INVASION_SERVER_RECEIVER_H
#define INVASION_SERVER_RECEIVER_H

#include <thread>
#include "user.h"
#include "safe-queue.h"
#include <player.pb.h>

namespace inVasion::session {
    class ReceiverFromUser {

    public:
        ReceiverFromUser(std::shared_ptr<User> cur_client, SafeQueue<PlayerAction> *queueToEngine) {
            std::thread([client = std::move(cur_client), q = queueToEngine]() {
                while (client->channel) {

                    std::uint32_t size;  // get the message data length in bytes
                    client->channel.read(reinterpret_cast<char *> (&size), sizeof(size));
                    char arr[size];
                    client->channel.read(reinterpret_cast<char *> (&arr), size);

                    PlayerAction action;
                    action.ParseFromArray(arr, size);
                    q->produce(std::move(action));

                }
                std::cout << "Client disconnected" << std::endl;
            }).detach();
        }
    };
}


#endif //INVASION_SERVER_RECEIVER_H
