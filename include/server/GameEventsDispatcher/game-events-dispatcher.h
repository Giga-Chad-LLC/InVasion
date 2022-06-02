#ifndef GAME_EVENTS_DISPATCHER_H_
#define GAME_EVENTS_DISPATCHER_H_

#include <atomic>
#include <memory>
#include <thread>
// server
#include "server/Session/session-fwd.h"
#include "server/NetworkPacket/network-packet.h"
#include "server/safe-queue.h"
// game-models
#include "game-models/GameSession/game-session.h"


namespace invasion::server {
class GameEventsDispatcher {
public:
    ~GameEventsDispatcher();
    void start(
        std::weak_ptr <Session> session,
        std::weak_ptr <game_models::GameSession> gameSession,
        std::weak_ptr <SafeQueue <std::shared_ptr <NetworkPacketRequest>>> requestQueue
    );
    void stop();
private:
    void dispatchEvent(
        std::weak_ptr <Session> session,
        std::weak_ptr <game_models::GameSession> gameSession,
        std::shared_ptr <NetworkPacketRequest> request
    );
    std::atomic_bool m_isActive = false;
    std::thread m_thread;
};
}

#endif