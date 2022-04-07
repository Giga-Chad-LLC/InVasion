
#ifndef INVASION_SERVER_SENDER_H
#define INVASION_SERVER_SENDER_H

#include "user.h"
#include "player.pb.h"
#include <thread>
#include <cstring>
#include <vector>
#include <memory>
#include <cassert>
#include "network_packet.h"

namespace invasion::session {
    class SenderUser {
    public:
        SenderUser(std::shared_ptr<User> curClient) {
            std::thread([client = curClient]() {
                while (true) {
                    NetworkPacketResponse response;
                    if (client->queueClientPrivate.consumeSync(response)) {
                        
                        std::shared_ptr<char> buffer = response.serializeToByteArray(); 
                        client->channel.write(buffer.get(),
                                            response.bytesSize() + sizeof(static_cast<std::uint32_t> (response.getMessageType())));
                    }
                }
            }).detach();
        }
    };
}

#endif //INVASION_SERVER_SENDER_H
