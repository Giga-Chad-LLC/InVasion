
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
            std::thread([client = curClient, this]() {
                while (true) {

                    NetworkPacketResponse response;
                    if (client->queueForSend.consumeSync(response)) {
                        // switch (response.getMessageType()) {
                        //     case ResponseModel_t::PlayerActionResponseModel: {
                        //         std::uint32_t type = static_cast<std::uint32_t> (response.getMessageType());
                        //         std::uint32_t totalBytesLength = response.bytesSize() + sizeof(type);
                        //         std::shared_ptr<char> responseFromServer = response.serializeToByteArray();
                        //         client->channel.write(responseFromServer.get(), totalBytesLength);
                        //         break;
                        //     }
                        //     default: {
                        //         break;
                        //     }
                        // }
                        client->channel.write(response.serializeToByteArray().get(),
                                            response.bytesSize() + sizeof(static_cast<std::uint32_t> (response.getMessageType())));
                    }
                }
            }).detach();
        }
    };
}

#endif //INVASION_SERVER_SENDER_H
