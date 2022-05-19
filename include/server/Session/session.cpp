#include "session.h"
#include <cassert>
#include <thread>
#include <iterator>
#include "server/Server/server.h"
#include "server/CountDownLatch/count-down-latch.h"
#include "server/NetworkPacket/network-packet.h"
#include "server/safe-queue.h"
// game-models
#include "player-info-response-model.pb.h"
#include "update-game-state-request-model.pb.h"


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
    std::cout << "Starting session: " << m_sessionId << std::endl;
    m_isActive.store(true);
    m_tickController.start([this]() mutable {
        auto request = std::make_shared <NetworkPacketRequest> (nullptr, RequestModel_t::UpdateGameStateRequestModel, 0U);
        m_requestQueue->produce(std::move(request));
    });

    m_gameEventsDispatcher->start(shared_from_this(), m_gameSession, m_requestQueue);

    m_sessionRemover.setTimeout(MATCH_DURATION_MS, [this]() {
        // send notification to players that the session is closing
        std::cout << "Session " << m_sessionId << " expired, send notifications to players" << std::endl;
        
        // users cannot connect to the session anymore
        m_isAvailable.store(false);
        std::shared_ptr <CountDownLatch> latch = nullptr;
        {
            std::scoped_lock sl{ mtx_connections, mtx_clientsThreadPool };
            latch = std::make_shared <CountDownLatch> (static_cast <uint32_t> (m_connections.size()));
            for (auto [ client, clientResponseQueue ] : m_connections) {
                // clientResponseQueue->produce(...);
            }
        }
        
        // wait for notification to be send to every player
        std::cout << "Start latch for session: " << m_sessionId << " (count: " << latch->getCount() << ")" << std::endl;
        latch->await();
        std::cout << "Stop latch for session: " << m_sessionId << " (count: " << latch->getCount() << ")" << std::endl;
    
        stop();
    });
}

void Session::stop() {
    if (!m_isActive.load()) {
        return;
    }

    m_isActive.store(false);
    m_isAvailable.store(false);

    std::cout << "Stopping session: " << m_sessionId << std::endl;

    std::cout << "Stopping tick controller" << std::endl;
    m_tickController.stop();
    std::cout << "Stopping events dispatcher" << std::endl;
    m_gameEventsDispatcher->stop();

    assert(m_clientsThreadPool.size() == m_connections.size());

    std::cout << "Stopping clients" << std::endl;
    for (auto [ client, clientResponseQueue ] : m_connections) {
        client->stop();
    }

    std::cout << "Stopping workers" << std::endl;
    for (auto [ ios, work ] : m_clientsThreadPool) {
        // we want to finish all sending/receiving data processes (on a client side and inside a `m_gameEventsDispatcher` we will stop adding any more network packets)
        // work.reset(); // does not work for some reason (maybe I don't get something about it)

        // for force stop we might user `ios->stop()` - the last packet will be discarded mid-air (not good, because client will not get full network packet and won't be able to parse it correctly)
        ios->stop();
    }
}

bool Session::isAvailable() const noexcept {
    return MAX_CLIENT_COUNT > m_connections.size() && m_isAvailable.load();
}


uint32_t Session::getSessionId() const noexcept {
    return m_sessionId;
}

void Session::removeClient(uint32_t clientId) {
    std::scoped_lock sl{ mtx_connections, mtx_clientsThreadPool };
    
    for (std::size_t i = 0U; i < m_connections.size(); i++) {
        auto client = m_connections[i].first;
        auto clientResponseQueue = m_connections[i].second;
        auto ios = m_clientsThreadPool[i].first;

        if (client->getClientId() == clientId) {
            std::cout << "Removing client: " << clientId << std::endl;
            m_gameSession->removePlayerById(clientId);
            client->stop(); // stop client's threads
            ios->stop(); // stop ios
            m_connections.erase(std::next(m_connections.begin(), i));
            m_clientsThreadPool.erase(std::next(m_clientsThreadPool.begin(), i));
            return;
        }
    }
}

void Session::makeHandshakeWithClient(
    std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacketResponse>>> clientResponseQueue,
    uint32_t playerId,
    game_models::PlayerTeamId teamId
) {
    // send to client his ID
    std::cout << "Send client his ID: " << playerId << std::endl;
    response_models::PlayerInfoResponseModel response;
    response.set_player_id(playerId);

    if (teamId == game_models::PlayerTeamId::FirstTeam) {
        response.set_team_id(response_models::PlayerInfoResponseModel::FirstTeam);
    } else {
        response.set_team_id(response_models::PlayerInfoResponseModel::SecondTeam);
    }

    auto packet = std::make_shared<NetworkPacketResponse> (
        NetworkPacket::serialize(response),
        ResponseModel_t::PlayerInfoResponseModel,
        response.ByteSizeLong()
    );

    clientResponseQueue->produce(std::move(packet));
}

void Session::addClient(
    std::shared_ptr <tcp::socket> socket,
    std::pair <
        std::shared_ptr <boost::asio::io_service>,
        std::shared_ptr <boost::asio::io_service::work>
    > executionService
) {
    std::cout << "Adding client to the session: " << m_sessionId << std::endl;

    // lock the mutexes
    std::scoped_lock sl{ mtx_connections, mtx_clientsThreadPool };
    
    auto clientResponseQueue = std::make_shared <SafeQueue<std::shared_ptr <NetworkPacketResponse>>> ();
    auto client = std::make_shared <Client> (socket, m_gameSession->createPlayerAndReturnId(), clientResponseQueue);
    m_connections.push_back({
        client,
        clientResponseQueue
    });
    
    m_clientsThreadPool.push_back(
        executionService
    );

    makeHandshakeWithClient(
        clientResponseQueue,
        client->getClientId(),
        m_gameSession->getPlayer(client->getClientId())->getTeamId()
    );
    

    std::thread thread([this, socket, executionService, client, clientResponseQueue]() {
        std::cout << "Processing the client in detached thread: " << socket->remote_endpoint() << std::endl;
        
        client->start(
            m_requestQueue,
            shared_from_this()
        );

        executionService.first->run();
        std::cout << "Client (" << client->getSocket()->remote_endpoint() << ") thread exits" << std::endl;
    });

    thread.detach();
}

void Session::putDataToSingleClient(
    uint32_t clientId,
    std::shared_ptr <NetworkPacketResponse> response
) {
    std::scoped_lock sl{ mtx_connections, mtx_clientsThreadPool };
    for (auto [client, clientResponseQueue] : m_connections) {
        if (client->getClientId() == clientId) {       
            clientResponseQueue->produce(std::move(response));
            return;
        }
    }
}

void Session::putDataToAllClients(
    std::shared_ptr <NetworkPacketResponse> response
) {
    std::scoped_lock sl{ mtx_connections, mtx_clientsThreadPool };
    for (auto [client, clientResponseQueue] : m_connections) {
        clientResponseQueue->produce(std::move(std::shared_ptr <NetworkPacketResponse> (response)));
    }
}
}