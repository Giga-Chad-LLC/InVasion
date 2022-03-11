
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
                    if (client->queueForSend.consumeSync(response)) {
                        switch (response.getMessageType()) {
                            case ResponseModel_t::PlayerActionResponseModel: {
                                std::uint32_t type = static_cast<std::uint32_t> (response.getMessageType());
                                char* action_buffer = new char[response.bytesSize() + sizeof(type)];
                                std::unique_ptr <char> action_buffer_ptr(action_buffer); 

                                std::memcpy(action_buffer_ptr.get(), reinterpret_cast<char*> (&type), sizeof(type));
                                std::memcpy(action_buffer_ptr.get() + static_cast<int> (sizeof(type)), response.getStoredBytes(), response.bytesSize());

                                client->channel.write(action_buffer_ptr.get(), response.bytesSize() + sizeof(type));
                                break;
                            }
                            default: {
                                break;
                            }
                        }
                    }
                }
            }).detach();
        }
    };
}

#endif //INVASION_SERVER_SENDER_H
