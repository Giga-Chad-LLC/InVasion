
#ifndef INVASION_SERVER_SENDER_H
#define INVASION_SERVER_SENDER_H

#include "user.h"
#include "player.pb.h"
#include <thread>
#include <cstring>

namespace inVasion::session {
    class SenderUser {
    public:
        SenderUser(std::shared_ptr<User> curClient) {
            std::thread([client = curClient]() {
                while (true) {
                    PlayerAction action;
                    if (client->queueForSend.consume(action)) {
                        std::uint32_t type = 1002;
                        char action_buffer[action.ByteSizeLong() + sizeof(type)];
                        std::memcpy(&action_buffer, reinterpret_cast<char*> (&type), sizeof(type));

                        action.SerializeToArray(action_buffer + sizeof(type), action.ByteSizeLong());
                        // std::cout << "Send action: " << action.key_pressed() << std::endl;
                        client->channel.write(action_buffer, action.ByteSizeLong() + sizeof(type));
                    }
                }
            }).detach();
        }
    };
}

#endif //INVASION_SERVER_SENDER_H
