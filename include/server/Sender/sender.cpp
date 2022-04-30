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
    void ClientResponsesSender::start() {
        thread_ = std::move(std::thread([client = ptrClient]() {
            while (true) {
                std::shared_ptr<NetworkPacketResponse> response;
                if (client->getClientResponseQueue().consumeSync(response)) {
                    uint32_t messageLength =
                            response->bytesSize() + sizeof(static_cast<std::uint32_t> (response->getMessageType())) +
                            sizeof(response->bytesSize());
                    std::shared_ptr<char[]> buffer = response->serializeToByteArray();
                    client->getChannel().write(buffer.get(), messageLength);

                }
            }
        }));
        thread_.detach();
    }

    void ClientResponsesSender::stop() {
        thread_.join();
    }

}