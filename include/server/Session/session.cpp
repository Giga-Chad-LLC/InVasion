#include <cassert>
#include <thread>
#include "server/Session/session.h"
#include "server/NetworkPacket/network-packet.h"
#include "server/safe-queue.h"


namespace invasion::server {
using boost::asio::ip::tcp;

Session::Session(uint32_t sessionId): m_sessionId(sessionId) {}

Session::~Session() {
    stop();
}

void Session::start() {
    if (m_isActive.load()) {
        return;
    }
    m_isActive.store(true);
    // m_tickController.start([this]() mutable {
    //     auto request = std::make_shared <NetworkPacket> (nullptr, RequestModel_t::UpdateGameStateRequestModel, 0U);
    //     m_requestQueue->produce(std::move(request));
    // });
    m_gameEventsDispatcher->start();
}

void Session::stop() {
    if (!m_isActive.load()) {
        return;
    }

    m_isActive.store(false);
    std::cout << "Stopping session: " << m_sessionId << std::endl;

    m_gameEventsDispatcher->stop();

    assert(m_clientsThreadPool.size() == m_connections.size());

    std::cout << "Stopping clients" << std::endl;
    for (auto [ client, queue ] : m_connections) {
        client->stop();
    }

    std::cout << "Stopping works" << std::endl;
    for (auto [ ios, work ] : m_clientsThreadPool) {
        // we want to finish all sending/receiving data processes (on a client side and inside a `m_gameEventsDispatcher` we will stop adding any more newtwork packets)
        work.reset();
        std::cout << "Stop work. Wait to finish clients operations." << std::endl;
        // for force stop we might user `ios->stop()` - the last packet will be discarded mid-air (not good, because client will not get full network packet and won't be able to parse it correctly)
    }

}

bool Session::isAvailable() {
    return MAX_CLIENT_COUNT > m_connections.size();
}


uint32_t Session::getSessionId() {
    return m_sessionId;
}

void Session::addClient(
        std::shared_ptr <tcp::socket> socket,
        std::pair <
            std::shared_ptr <boost::asio::io_service>,
            std::shared_ptr <boost::asio::io_service::work>
        > executionService
    ) {
        std::cout << "Adding client to the session: " << m_sessionId << std::endl;

        m_connections.push_back({
            std::make_shared <Client> (socket, m_gameSession.createPlayerAndReturnId()),
            std::make_shared <SafeQueue<std::shared_ptr <NetworkPacket>>> ()
        });
        
        m_clientsThreadPool.push_back(
            executionService
        );

        std::thread thread([socket, executionService]() {
            std::cout << "Processing the client in detached thread: " << socket->remote_endpoint() << std::endl;
            executionService.first->run();
        });
        thread.detach();
    }
}