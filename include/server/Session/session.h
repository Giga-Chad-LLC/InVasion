#ifndef SESSION_H_
#define SESSION_H_


#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <vector>
#include <chrono>
#include <memory>
#include <string>
#include <atomic>
#include <mutex>
#include <functional>
// server
#include "server/Client/client.h"
#include "server/CountDownLatch/count-down-latch.h"
#include "server/NetworkPacket/network-packet.h"
#include "server/safe-queue.h"
#include "server/GameEventsDispatcher/game-events-dispatcher.h"
// game-models
#include "game-models/GameSession/game-session.h"
// controllers
#include "controllers/FixedTimeIntervalInvoker/fixed-time-interval-invoker.h"
#include "controllers/FixedTimeoutCallbackInvoker/fixed-timeout-callback-invoker.h"

namespace invasion::server {
using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(uint32_t sessionId);
    ~Session();
    void start();
    void stop();
    void addClient(
        std::shared_ptr <tcp::socket> socket,
        std::pair <
            std::shared_ptr <boost::asio::io_service>,
            std::shared_ptr <boost::asio::io_service::work>
        > executionService
    );
    void removeClient(uint32_t clientId);
    void putDataToSingleClient(uint32_t clientId, std::shared_ptr <NetworkPacketResponse> response);
    void putDataToAllClients(std::shared_ptr <NetworkPacketResponse> response);
    bool isAvailable() const noexcept;
    bool isActive() const noexcept;
    void setClientCredencials(uint32_t clientId, const std::string& username, const std::string& token);
    uint32_t getSessionId() const noexcept;
    void makeHandshakeWithClient(
        std::shared_ptr <SafeQueue<
            std::pair<
                std::shared_ptr <NetworkPacketResponse>,
                std::shared_ptr <LatchCaller>
            >
        >> clientResponseQueue,
        uint32_t playerId
    );

private:
    void onGameOver();

    const std::size_t MAX_CLIENT_COUNT = 8U;
    std::chrono::milliseconds MATCH_START_TIMESTAMP_MS;
    const std::size_t MATCH_DURATION_MS = 1000 * 60 * 10;
    std::atomic_bool m_isActive = false;
    std::atomic_bool m_isAvailable = true;
    uint32_t m_sessionId;
    std::vector <
        std::pair <
            std::shared_ptr <boost::asio::io_service>,
            std::shared_ptr <boost::asio::io_service::work>
        >
    > m_clientsThreadPool;
    std::vector <std::pair <
        std::shared_ptr <Client>,
        std::shared_ptr <SafeQueue<
            std::pair<
                std::shared_ptr <NetworkPacketResponse>,
                std::shared_ptr <LatchCaller>
            >
        >>
    >> m_connections;
    std::mutex mtx_connections;
    std::mutex mtx_clientsThreadPool;

    std::shared_ptr <game_models::GameSession> m_gameSession = std::make_shared <game_models::GameSession> ();
    // update the game each 30 milliseconds
    controllers::FixedTimeIntervalInvoker m_tickController = controllers::FixedTimeIntervalInvoker(30); 
    controllers::FixedTimeoutCallbackInvoker m_gameTimer;
    std::shared_ptr <GameEventsDispatcher> m_gameEventsDispatcher = std::make_shared <GameEventsDispatcher> ();
    std::shared_ptr <SafeQueue <std::shared_ptr <NetworkPacketRequest>>> m_requestQueue = std::make_shared <SafeQueue <std::shared_ptr <NetworkPacketRequest>>> ();
    std::size_t getRemainingSessionTime_ms() const noexcept; 
};
}

#endif