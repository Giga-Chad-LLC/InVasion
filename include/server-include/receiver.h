
#ifndef INVASION_SERVER_RECEIVER_H
#define INVASION_SERVER_RECEIVER_H

#include <thread>
#include "user.h"
#include "safe-queue.h"
#include <player.pb.h>

class receiver {

public:
    receiver(std::shared_ptr<user> cur_client, SafeQueue<PlayerAction> *queue) {
        std::thread([client = std::move(cur_client), q = queue]() {
            while (client->chanel) {

                std::uint32_t size;  // get the message data length in bytes
                client->chanel.read(reinterpret_cast<char *> (&size), sizeof(size));
                char arr[size];
                client->chanel.read(reinterpret_cast<char *> (&arr), size);

                PlayerAction action;
                action.ParseFromArray(arr, size);
                q->Produce(std::move(action));
                // на данном этапе в очереди хранятся уже готовые PlayerAction, дальше
                // я буду переделывать, чтобы у нас поддерживались не только шаги
            }
            std::cout << "Client disconnected" << std::endl;
        }).detach();
    }
};


#endif //INVASION_SERVER_RECEIVER_H
