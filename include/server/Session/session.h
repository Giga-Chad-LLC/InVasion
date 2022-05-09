#ifndef SESSION_H_
#define SESSION_H_


#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <vector>
#include <memory>
#include <atomic>
#include <mutex>
// server
#include "server/Client/client.h"
#include "server/NetworkPacket/network-packet.h"
#include "server/safe-queue.h"
#include "server/GameEventsDispatcher/game-events-dispatcher.h"
// game-models
#include "game-models/GameSession/game-session.h"
// controllers
#include "controllers/FixedTimeIntervalInvoker/fixed-time-interval-invoker.h"

namespace invasion::server {
using boost::asio::ip::tcp;

class Session {
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
    void putDataToSingleClient(uint32_t clientId, std::shared_ptr <NetworkPacket>);
    void putDataToAllClients(std::shared_ptr <NetworkPacket>);
    bool isAvailable() const noexcept;
    uint32_t getSessionId() const noexcept;


private:
    const std::size_t MAX_CLIENT_COUNT = 8U;
    std::atomic_bool m_isActive = false;
    uint32_t m_sessionId;
    std::vector <
        std::pair <
            std::shared_ptr <boost::asio::io_service>,
            std::shared_ptr <boost::asio::io_service::work>
        >
    > m_clientsThreadPool;
    std::vector <std::pair <
        std::shared_ptr <Client>,
        std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacketResponse>>>
    >> m_connections;
    std::mutex mtx_connections;
    std::mutex mtx_clientsThreadPool;

    game_models::GameSession m_gameSession;
    // update the game each 30 milliseconds
    controllers::FixedTimeIntervalInvoker m_tickController = controllers::FixedTimeIntervalInvoker(30); 
    std::shared_ptr <GameEventsDispatcher> m_gameEventsDispatcher = std::make_shared <GameEventsDispatcher> ();
    std::shared_ptr <SafeQueue <std::shared_ptr <NetworkPacketRequest>>> m_requestQueue = std::make_shared <SafeQueue <std::shared_ptr <NetworkPacketRequest>>> ();
};
}

#endif