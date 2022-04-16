#include "sender.h"

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
#include <chrono>
#include <boost/asio.hpp>
#include "server/NetworkPacket/network-packet.h"


namespace invasion::session {
ClientResponsesSender::ClientResponsesSender(std::shared_ptr<Client> client) {
    std::thread([client = client]() {
        int i = 1, j = 1;

        while (true) {
            NetworkPacketResponse response;
            if (client->m_clientResponseQueue.consumeSync(response)) {
                
                uint32_t messageLength = response.bytesSize() + sizeof(static_cast<std::uint32_t> (response.getMessageType())) + sizeof(response.bytesSize());
                std::shared_ptr<char[]> buffer = response.serializeToByteArray();
                client->m_channel.write(buffer.get(), messageLength);

	            // std::this_thread::sleep_for(std::chrono::milliseconds(30));
                // Print bytes (for debug only)
                // std::cout << static_cast<int> (response.getMessageType()) << "\n";
                // for (int i = 0; i < messageLength; i++) {
                //     std::cout << (int)buffer.get()[i] << ' ';
                // }
                // std::cout << std::endl;
            }
        }
    }).detach();
}
}