#include <cassert>
#include <thread>
#include <iterator>

#include "session.h"
// database
#include "database/PlayerStatisticsService/player-statistics-service.h"
// libs
#include "libs/json/json.hpp"
// server
#include "server/Server/server.h"
#include "server/CountDownLatch/count-down-latch.h"
#include "server/NetworkPacket/network-packet.h"
#include "server/safe-queue.h"
// request-models
#include "update-game-state-request-model.pb.h"
// response-models
#include "game-over-response-model.pb.h"
#include "handshake-response-model.pb.h"
#include "client-connected-response-model.pb.h"
#include "client-disconnected-response-model.pb.h"
// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player-specialization-enum.h"
// interactors
#include "interactors/HandshakeResponseInteractor/handshake-response-interactor.h"
#include "interactors/FormJSONPlayerStatsResponseInteractor/form-json-player-stats-response-interactor.h"
#include "interactors/ClientConnectedResponseInteractor/client-connected-response-interactor.h"
#include "interactors/ClientDisconnectedResponseInteractor/client-disconnected-response-interactor.h"
#include "algorithm"



namespace invasion::server {
using boost::asio::ip::tcp;

Session::Session(uint32_t sessionId): m_sessionId(sessionId) {}

Session::~Session() {
    stop();
    std::cout << "Session " << m_sessionId << " destroyed" << std::endl;
}

void Session::start() {
    if (m_isActive.load()) {
        return;
    }
    std::cout << "Starting session: " << m_sessionId << std::endl;
    m_isActive.store(true);
    m_tickController.start([this]() mutable {
        auto request = std::make_shared <NetworkPacketRequest> (nullptr, RequestModel_t::UpdateGameStateRequestModel, 0U);
        if (m_requestQueue) {
            m_requestQueue->produce(std::move(request));
        }
        else {
            std::cout << "Session error: request queue is nullptr when tried to put data" << std::endl;
        }
    });

    // set time of the match start
    MATCH_START_TIMESTAMP_MS = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );

    m_gameEventsDispatcher->start(shared_from_this(), m_gameSession, m_requestQueue);

    m_gameTimer.setTimeout(MATCH_DURATION_MS, [this]() {
        this->onGameOver();
    });
}

void Session::onGameOver() {
    // send notification to players that the session is closing
    std::cout << "Session " << m_sessionId << " expired, send notifications to players" << std::endl;
    
    // users cannot connect to the session anymore
    m_isAvailable.store(false);
    std::shared_ptr <CountDownLatch> latch = nullptr;
    {
        std::scoped_lock sl{ mtx_connections, mtx_clientsThreadPool };
        latch = std::make_shared <CountDownLatch> (static_cast <uint32_t> (m_connections.size()));
        for (auto [ client, clientResponseQueue ] : m_connections) {
            response_models::GameOverResponseModel gameOverModel;
            auto response = std::make_shared <NetworkPacketResponse> (
                NetworkPacket::serialize(gameOverModel),
                ResponseModel_t::GameOverResponseModel,
                gameOverModel.ByteSizeLong()
            );

            clientResponseQueue->produce({
                std::move(response),
                std::make_shared <LatchCaller> (latch)
            });
        }

        interactors::FormJSONPlayerStatsResponseInteractor interactor;
        services::PlayerStatisticsService service;

        for (auto [ client, clientResponseQueue ] : m_connections) {
            std::cout << "Make stats request for client: " << client->getClientId() << std::endl;
            nlohmann::json request = interactor.execute(
                client->getClientId(),
                client->getClientAccessToken(),
                *m_gameSession
            );
            std::cout << "Retrieved stats request: " << request["username"] << ", " << request["token"] << ", " << request["kills"] << ", " << request["deaths"] << std::endl;

            std::cout << "Before service update" << std::endl;
            service.update(request);
            std::cout << "After service update" << std::endl;
        }
    }
    
    // wait for notification to be send to every player
    std::cout << "Start latch for session: " << m_sessionId << " (count: " << latch->getCount() << ")" << std::endl;
    latch->await();
    std::cout << "Stop latch for session: " << m_sessionId << " (count: " << latch->getCount() << ")" << std::endl;

    stop();
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
    
    std::scoped_lock sl{ mtx_connections, mtx_clientsThreadPool };
    assert(m_clientsThreadPool.size() == m_connections.size());

    std::cout << "Stopping clients and their workers (total connected clients: " << m_connections.size() << ")" << std::endl;
    
    while (!m_connections.empty()) {
        auto client = m_connections.back().first;
        auto clientResponseQueue = m_connections.back().second;
        auto ios = m_clientsThreadPool.back().first;

        std::cout << "Removing client: " << client->getClientId() << std::endl;
        
        if (m_gameSession->playerExists(client->getClientId())) {
            m_gameSession->removePlayerById(client->getClientId());
        }

        client->stop(); // stop client's threads
        ios->stop(); // stop ios
        m_connections.pop_back();
        m_clientsThreadPool.pop_back();
        return;
    }
}

bool Session::isAvailable() const noexcept {
    return MAX_CLIENT_COUNT > m_connections.size() && m_isAvailable.load();
}

bool Session::isActive() const noexcept {
    return m_isActive.load();
}


void Session::setClientCredencials(uint32_t clientId, const std::string& username, const std::string& token) {
    std::scoped_lock sl{ mtx_clientsThreadPool, mtx_connections };

    for (auto [ client, clientResponseQueue ] : m_connections) {
        if (client->getClientId() == clientId) {
            client->storeCredencials(username, token);
            break;
        }
    }
}

uint32_t Session::getSessionId() const noexcept {
    return m_sessionId;
}

void Session::removeClient(uint32_t clientId) {
    // remove disconnected client
    {
        std::scoped_lock sl{ mtx_connections, mtx_clientsThreadPool };
        for (std::size_t i = 0U; i < m_connections.size(); i++) {
            auto client = m_connections[i].first;
            auto clientResponseQueue = m_connections[i].second;
            auto ios = m_clientsThreadPool[i].first;

            if (client->getClientId() == clientId) {
                std::cout << "Removing client: " << clientId << std::endl;
                
                std::cout << "All clients: ";
                const auto& players = m_gameSession->getPlayers();
                for (auto p : players) {
                    std::cout << p->getId() << ", ";
                }
                std::cout << std::endl;

                if (m_gameSession->playerExists(client->getClientId())) {
                    m_gameSession->removePlayerById(clientId);
                }
                else {
                    std::cout << "Client with id: " << clientId << " was not found and not removed" << std::endl;
                }
                std::cout << "Total clients in game session: " << players.size() << std::endl;

                ios->stop(); // stop ios
                client->stop(); // stop client's threads

                m_connections.erase(std::next(m_connections.begin(), i));
                m_clientsThreadPool.erase(std::next(m_clientsThreadPool.begin(), i));
                
                break;
            }
        }
    }

    // notify other clients, that client have left the session
    {
        interactors::ClientDisconnectedResponseInteractor interactor;
        auto response = interactor.execute(clientId, *m_gameSession);
        auto packet = std::make_shared <NetworkPacketResponse> (
            NetworkPacket::serialize(response),
            ResponseModel_t::ClientDisconnectedResponseModel,
            response.ByteSizeLong()
        );
        putDataToAllClients(packet);
    }
}

void Session::makeHandshakeWithClient(
    std::shared_ptr <SafeQueue<
            std::pair<
                std::shared_ptr <NetworkPacketResponse>,
                std::shared_ptr <LatchCaller>
            >
        >> clientResponseQueue,
    uint32_t playerId
) {
    // send to client his ID
    // std::cout << "Send client his info: id " << playerId << ", team " << static_cast<uint32_t> (teamId) << std::endl;
    interactors::HandshakeResponseInteractor interactor;
    auto response = interactor.execute(getRemainingSessionTime_ms(), *m_gameSession, playerId);

    auto packet = std::make_shared<NetworkPacketResponse> (
        NetworkPacket::serialize(response),
        ResponseModel_t::HandshakeResponseModel,
        response.ByteSizeLong()
    );

    clientResponseQueue->produce({ std::move(packet), nullptr });
}

void Session::addClient(
    std::shared_ptr <tcp::socket> socket,
    std::pair <
        std::shared_ptr <boost::asio::io_service>,
        std::shared_ptr <boost::asio::io_service::work>
    > executionService
) {
    std::cout << "Adding client to the session: " << m_sessionId << std::endl;
    
    auto clientResponseQueue = std::make_shared <SafeQueue<
        std::pair<
            std::shared_ptr <NetworkPacketResponse>,
            std::shared_ptr <LatchCaller>
        >
    >> ();

    auto client = std::make_shared <Client> (
        socket,
        m_gameSession->createPlayerAndReturnId(game_models::PlayerSpecialization::UNDEFINED), 
        clientResponseQueue
    );

    // send packets to the existing clients that new connection arrived
    {
        interactors::ClientConnectedResponseInteractor interactor;
        auto response = interactor.execute(client->getClientId(), *m_gameSession);

        auto packet = std::make_shared<NetworkPacketResponse> (
            NetworkPacket::serialize(response),
            ResponseModel_t::ClientConnectedResponseModel,
            response.ByteSizeLong()
        );

        putDataToAllClients(packet);
    }

    // lock the mutexes
    std::scoped_lock sl{ mtx_connections, mtx_clientsThreadPool };
    m_connections.push_back({
        client,
        clientResponseQueue
    });
    
    m_clientsThreadPool.push_back(
        executionService
    );

    makeHandshakeWithClient(clientResponseQueue, client->getClientId());

    std::thread thread([this, socket, executionService, client, clientResponseQueue]() {
        std::cout << "Processing the client in detached thread: " << socket->remote_endpoint() << std::endl;
        
        client->start(
            m_requestQueue,
            shared_from_this()
        );

        try {
            executionService.first->run();
        }
        catch (const std::exception& error) {
            std::cerr << "Session error: " << error.what() << std::endl;
            removeClient(client->getClientId());
        }

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
        if (client->getClientId() == clientId && m_gameSession->playerExists(client->getClientId())) {       
            clientResponseQueue->produce({ std::move(response), nullptr });
            return;
        }
    }
}

void Session::putDataToAllClients(
    std::shared_ptr <NetworkPacketResponse> response
) {
    std::scoped_lock sl{ mtx_connections, mtx_clientsThreadPool };
    for (auto [client, clientResponseQueue] : m_connections) {
        if (m_gameSession->playerExists(client->getClientId())) {
            clientResponseQueue->produce({
                std::move(std::shared_ptr <NetworkPacketResponse> (response)),
                nullptr
            });
        }
    }
}

std::size_t Session::getRemainingSessionTime_ms() const noexcept {
    std::chrono::milliseconds now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );

    std::size_t remainingTime_ms = std::max(
        static_cast <int64_t> (0),
        static_cast <int64_t> (MATCH_DURATION_MS - (now_ms - MATCH_START_TIMESTAMP_MS).count())
    );

    return remainingTime_ms;
}
}