

#ifndef INVASIONSERVERS_DISPATCHERPACKETSTOCLIENTS_H
#define INVASIONSERVERS_DISPATCHERPACKETSTOCLIENTS_H

#include "server/safe-queue.h"
#include "server/Client/client.h"
#include "server/Receiver/receiver.h"
#include "server/Sender/sender.h"
#include "server/Engine/engine.h"
#include "server/NetworkPacket/network-packet.h"
#include "ConnectedClients.h"

namespace invasion::session {
    class DispatcherPackets {
        SafeQueue<std::shared_ptr<NetworkPacketResponse>> *responseQueue;
        std::thread thread_;
    public:
        DispatcherPackets(SafeQueue<std::shared_ptr<NetworkPacketResponse>> *responseQueue_) : responseQueue(
                responseQueue_) {}

        void start() {
            thread_ = std::move(std::thread([&]() {
                                    while (true) {
                                        auto response = std::shared_ptr<NetworkPacketResponse>();
                                        if (responseQueue->consumeSync(response)) {
                                            auto clients = ConnectedClients::getConnectedClients();
                                            for (auto &client: clients) {
                                                std::shared_ptr<NetworkPacketResponse> packet = response;
                                                client->getClientResponseQueue().produce(std::move(packet));
                                            }
                                        }
                                    }
                                })

            );
            thread_.detach();
        }

        void stop() {
            thread_.join();
        }
    };
}

#endif //INVASIONSERVERS_DISPATCHERPACKETSTOCLIENTS_H
