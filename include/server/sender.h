
#ifndef INVASION_SERVER_SENDER_H
#define INVASION_SERVER_SENDER_H

#include "client.h"
#include "player.pb.h"
#include "player-id-response-model.pb.h"
#include "players-positions-response-model.pb.h"
#include "player-position-response-model.pb.h"
#include <thread>
#include <cstring>
#include <vector>
#include <memory>
#include <iostream>
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
                        uint32_t messageLength = response.bytesSize() + sizeof(static_cast<std::uint32_t> (response.getMessageType())) + sizeof(response.bytesSize());
                        std::shared_ptr<char[]> buffer = response.serializeToByteArray();
                        client->m_channel.write(buffer.get(), messageLength);

                        // Print bytes (for debug only)
                        // for (int i = 0; i < messageLength; i++) {
                        //     std::cout << (int)buffer.get()[i] << ' ';
                        // }
                        // std::cout << std::endl;
                    }
                }
            }).detach();
        }
    };
}

#endif //INVASION_SERVER_SENDER_H
