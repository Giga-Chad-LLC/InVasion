#ifndef INVASION_SERVER_ENGINE_H
#define INVASION_SERVER_ENGINE_H

#include <thread>
#include <memory>
// server
#include "server/NetworkPacket/network-packet.h"
#include "server/safe-queue.h"
// game-models
#include "game-models/GameSession/game-session.h"



namespace invasion::session {
    void manageRequestQueue(SafeQueue<NetworkPacketRequest> &requestQueue, SafeQueue<NetworkPacketResponse> &responseQueue, game_models::GameSession &gameSession); 
}
#endif //INVASION_SERVER_ENGINE_H
