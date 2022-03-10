
#ifndef INVASION_SERVER_SENDER_H
#define INVASION_SERVER_SENDER_H

#include "user.h"
#include "player.pb.h"
#include <thread>

namespace inVasion::session {
    class SenderUser {
    public:
        SenderUser(std::shared_ptr<User> curClient) {
            std::thread([client = curClient]() {
                while (true) {
                    ResponseObject action;
                    if (client->queueToClientPrivate.consumeSync(action)) {
                        std::cerr << "ok";
                        client->channel.write(&action.arrBytes, action.sizeMessage);
                    }
                }
            }).detach();
        }
    };
}

#endif //INVASION_SERVER_SENDER_H
