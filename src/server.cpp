#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <thread>
#include <utility>
#include "server/server.h"
#include "server/NetworkPacket/network-packet.h"
#include "player-id-response-model.pb.h"
#include "update-game-state-request-model.pb.h"
#include "interactors/InitialStateResponseInteractor/initial-state-response-interactor.h"

using boost::asio::ip::tcp;
namespace invasion::session {
    void registerClientInSession(std::shared_ptr<Client> client, uint32_t playerId) {
        // send to client his ID
        std::cout << "Send client his ID: " << playerId << std::endl;
        response_models::PlayerIdResponseModel response;
        response.set_playerid(playerId);
        
        uint32_t size = response.ByteSizeLong();
        std::unique_ptr<char[]> buffer_ptr(new char[size]);
        response.SerializeToArray(buffer_ptr.get(), size);
        NetworkPacketResponse packet(std::move(buffer_ptr), ResponseModel_t::PlayerIdResponseModel, size);
        client->m_clientResponseQueue.produce(std::move(packet));
    }

    Server::Server() : acceptor(ioContext, tcp::endpoint(boost::asio::ip::address::from_string("192.168.1.201"),
                                                         8000)) { // boost::asio::ip::address::from_string("127.0.0.1"); 192.168.1.201
        manageRequestQueue(m_requestQueue, m_responseQueue, m_gameSession);
        std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
    }

    void Server::awaitNewConnections() {
        while (true) {
            tcp::socket socket = acceptor.accept();

            std::cout << "Connected client: " << socket.remote_endpoint() << " --> " << socket.local_endpoint()
                      << std::endl;
            uint32_t playerId = m_gameSession.createPlayerAndReturnId();
            auto client = std::make_shared<Client>(std::move(socket), playerId);

            m_connectedClients.push_back(client);
            [[maybe_unused]] auto receiverOnThisUser = ClientRequestsReceiver(client, &m_requestQueue); // создание двух потоков на каждого клиента
            [[maybe_unused]] auto senderOnThisUser = ClientResponsesSender(client);

            registerClientInSession(client, playerId);

            if (!m_isSessionActive && m_connectedClients.size() == m_requiredClientsCountInSession) { // создание обработчика, если комманда собралась пока что handler - заглушка
                std::cout << "Maximum players count reached. Starting the game..." << std::endl;
                m_isSessionActive = true;

                // start putting packets from main response queue to client's specific queues
                std::thread(dispatchPacketsToClients, &m_responseQueue).detach();
                
                // start tick controller
                m_tickController.start([this]() mutable {
                    NetworkPacketRequest request(nullptr, RequestModel_t::UpdateGameStateRequestModel, 0U);
                    m_requestQueue.produce(std::move(request));
                });
            }
        }
    }

    // finds client by its player id in a game session (returns `nullptr`, if not found)
    std::shared_ptr<Client> Server::getConnectedClientByPlayerId(uint32_t playerId) const {
        for (auto &client : m_connectedClients) {
            if (client->m_clientIdInGameSession == playerId) {
                return client;
            }
        }

        return nullptr;
    }
}