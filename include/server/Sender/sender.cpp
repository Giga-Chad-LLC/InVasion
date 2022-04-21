#include "sender.h"

#include <thread>
#include <cstring>
#include <utility>
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
    std::thread([client = std::move(client)]() {
        while (true) {
            std::shared_ptr<NetworkPacketResponse> response;
            if (client->m_clientResponseQueue.consumeSync(response)) {
                uint32_t messageLength = response->bytesSize() + sizeof(static_cast<std::uint32_t> (response->getMessageType())) + sizeof(response->bytesSize());
                std::shared_ptr<char[]> buffer = response->serializeToByteArray();
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