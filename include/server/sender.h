
#ifndef INVASION_SERVER_SENDER_H
#define INVASION_SERVER_SENDER_H

#include "client.h"
#include "player.pb.h"
#include <thread>
#include <cstring>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include "network-packet.h"

namespace invasion::session {
    class ClientResponsesSender {
    public:
        ClientResponsesSender(std::shared_ptr<Client> client) {
            std::thread([client = client]() {
                while (true) {
                    NetworkPacketResponse response;
                    if (client->m_clientResponseQueue.consumeSync(response)) {
                        std::shared_ptr<char> buffer = response.serializeToByteArray(); 
                        client->m_channel.write(buffer.get(), response.bytesSize() + sizeof(static_cast<std::uint32_t> (response.getMessageType())));
                    }
                }
            }).detach();
        }
    };
}

#endif //INVASION_SERVER_SENDER_H
