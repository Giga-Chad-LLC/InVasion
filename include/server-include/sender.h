
#ifndef INVASION_SERVER_SENDER_H
#define INVASION_SERVER_SENDER_H

#include "user.h"
#include "player.pb.h"
#include <thread>

class sender {
public:
    sender(std::shared_ptr<user> cur_client) {
        std::thread([client = cur_client]() {
            while (true) {
                PlayerAction action;
                if (client->queue_send.Consume(action)) {
                    char action_buffer[action.ByteSizeLong()];
                    action.SerializeToArray(action_buffer, action.ByteSizeLong());

                    client->chanel.write(action_buffer, action.ByteSizeLong());
                }
            }
        }).detach();
    }
};


#endif //INVASION_SERVER_SENDER_H
