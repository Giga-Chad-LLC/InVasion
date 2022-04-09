
#ifndef INVASION_SERVER_SERVER_H
#define INVASION_SERVER_SERVER_H

#include "../../include/server/safe-queue.h"
#include "player.pb.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include "client.h"
#include "receiver.h"
#include "sender.h"
#include "engine.h"
#include "network-packet.h"
#include "game-models/GameSession/game-session.h"
#include "controllers/PhysicsTickController/physics-tick-controller.h"


using boost::asio::ip::tcp;
namespace invasion::session {
    inline std::vector<std::shared_ptr<Client>> m_connectedClients;
    inline void dispatchPacketsToClients(SafeQueue<NetworkPacketResponse> *responseQueue) {
        while (true) {
            NetworkPacketResponse response;
            if (responseQueue->consumeSync(response)) {
                for (auto client: m_connectedClients) { // пока никакой обработки просто имитируем ее
                    NetworkPacketResponse packet = response;
                    client->m_clientResponseQueue.produce(std::move(packet));
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
        SafeQueue<NetworkPacketRequest> m_requestQueue;
        SafeQueue<NetworkPacketResponse> m_responseQueue;
        game_models::GameSession m_gameSession;
        controllers::PhysicsTickController m_tickController = controllers::PhysicsTickController(30); // 60 updates per second => every 1000/60 milliseconds update the game
    public:
        explicit Server();
        void awaitNewConnections();

        friend void manageRequestQueue(SafeQueue<NetworkPacketRequest> &requestQueue, SafeQueue<NetworkPacketResponse> &responseQueue);
        friend class ClientRequestsReceiver;
        // puts each response packet from main response queue to client specific response queues
        friend void dispatchPacketsToClients(SafeQueue<NetworkPacketResponse> *responseQueue);
    };

}
#endif //INVASION_SERVER_SERVER_H
