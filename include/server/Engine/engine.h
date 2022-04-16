#ifndef INVASION_SERVER_ENGINE_H
#define INVASION_SERVER_ENGINE_H

#include <thread>
#include <memory>
// server
#include "server/NetworkPacket/network-packet.h"
#include "server/Client/client.h"
#include "server/safe-queue.h"
// game-models
#include "game-models/GameSession/game-session.h"


class Server;

namespace invasion::session {
    class RequestQueueManager {
    private:
        static std::shared_ptr<Client> getConnectedClientByPlayerId(const std::vector <std::shared_ptr<Client>> &connectedClients, uint32_t playerId);
    public:
        static void manageRequestQueue(SafeQueue<std::shared_ptr<NetworkPacketRequest>> &requestQueue, SafeQueue<std::shared_ptr<NetworkPacketResponse>> &responseQueue, game_models::GameSession &gameSession, const std::vector <std::shared_ptr<Client>> &connectedClients);
    }; 
}
#endif //INVASION_SERVER_ENGINE_H
