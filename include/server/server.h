
#ifndef INVASION_SERVER_SERVER_H
#define INVASION_SERVER_SERVER_H

#include "player.pb.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include "server/safe-queue.h"
#include "server/Client/client.h"
#include "server/Receiver/receiver.h"
#include "server/Sender/sender.h"
#include "server/Engine/engine.h"
#include "server/NetworkPacket/network-packet.h"
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
        controllers::PhysicsTickController m_tickController = controllers::PhysicsTickController(30); // update the game each 30 milliseconds 
    public:
        explicit Server();
        void awaitNewConnections();
        std::shared_ptr<Client> getConnectedClientByPlayerId(uint32_t playerId) const;

        friend void manageRequestQueue(SafeQueue<NetworkPacketRequest> &requestQueue, SafeQueue<NetworkPacketResponse> &responseQueue, game_models::GameSession &gameSession);
        friend class ClientRequestsReceiver;
        // puts each response packet from main response queue to client specific response queues
        friend void dispatchPacketsToClients(SafeQueue<NetworkPacketResponse> *responseQueue);
    };

}
#endif //INVASION_SERVER_SERVER_H
