
#ifndef INVASION_SERVER_SENDER_H
#define INVASION_SERVER_SENDER_H

#include "user.h"
#include "player.pb.h"
#include <thread>

class Sender {
public:
    Sender(std::shared_ptr<User> curClient) {
        std::thread([client = curClient]() {
            while (true) {
                PlayerAction action;
                if (client->queueForSend.consume(action)) {
                    char action_buffer[action.ByteSizeLong()];
                    action.SerializeToArray(action_buffer, action.ByteSizeLong());

                    client->chanel.write(action_buffer, action.ByteSizeLong());
                }
            }
        }).detach();
    }
};


#endif //INVASION_SERVER_SENDER_H
