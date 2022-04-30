#ifndef INVASION_SERVER_SERVER_H_
#define INVASION_SERVER_SERVER_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

// proto
#include "player.pb.h"
// server
#include "server/safe-queue.h"
#include "server/Client/client.h"
#include "server/Receiver/receiver.h"
#include "server/Sender/sender.h"
#include "server/Engine/engine.h"
#include "server/NetworkPacket/network-packet.h"
// game-models
#include "game-models/GameSession/game-session.h"
// controllers
#include "controllers/FixedTimeIntervalInvoker/fixed-time-interval-invoker.h"


using boost::asio::ip::tcp;
namespace invasion::session {
    class ConnectedClients{
    public:
        static std::vector<std::shared_ptr<Client>> &getConnectedClients() noexcept{
            static std::vector<std::shared_ptr<Client>> data;
            return data;
        }
    };

    inline void dispatchPacketsToClients(SafeQueue<std::shared_ptr<NetworkPacketResponse>> *responseQueue) {
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
    }

    class Server {
    private:
        boost::asio::io_context ioContext;
        tcp::acceptor acceptor;
        const size_t m_requiredClientsCountInSession = 1;
        bool m_isSessionActive = false;
        SafeQueue<std::shared_ptr<NetworkPacketRequest>> m_requestQueue{debug{}};
        SafeQueue<std::shared_ptr<NetworkPacketResponse>> m_responseQueue;
        game_models::GameSession m_gameSession;
        controllers::FixedTimeIntervalInvoker m_tickController = controllers::FixedTimeIntervalInvoker(
                30); // update the game each 30 milliseconds
    public:
        explicit Server();

        void awaitNewConnections();
        SafeQueue<std::shared_ptr<NetworkPacketRequest>>& getRequestQueue(){
            return m_requestQueue;
        }
        SafeQueue<std::shared_ptr<NetworkPacketResponse>> &getResponseQueue(){
            return m_responseQueue;
        }
        friend class ClientRequestsReceiver;

        friend class RequestQueueManager;

        // puts each response packet from main response queue to client specific response queues
        friend void dispatchPacketsToClients(SafeQueue<std::shared_ptr<NetworkPacketResponse>> *responseQueue);
    };

}

#endif // INVASION_SERVER_SERVER_H_
