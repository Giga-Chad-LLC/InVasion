#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <thread>
#include <utility>
#include "server/server.h"
#include "server/NetworkPacket/network-packet.h"
#include "player-info-response-model.pb.h"
#include "update-game-state-request-model.pb.h"
#include "interactors/InitialStateResponseInteractor/initial-state-response-interactor.h"

using boost::asio::ip::tcp;
namespace invasion::session {
    void registerClientInSession(std::shared_ptr<Client> client, uint32_t playerId, game_models::PlayerTeamId teamId) {
        // send to client his ID
        std::cout << "Send client his ID: " << playerId << std::endl;
        response_models::PlayerInfoResponseModel response;
        response.set_player_id(playerId);
        if (teamId == game_models::PlayerTeamId::FirstTeam) {
            response.set_team_id(response_models::PlayerInfoResponseModel::FirstTeam);
        } else {
            response.set_team_id(response_models::PlayerInfoResponseModel::SecondTeam);
        }

        uint32_t size = response.ByteSizeLong();
        std::unique_ptr<char[]> buffer_ptr(new char[size]);
        response.SerializeToArray(buffer_ptr.get(), size);
        auto packet = std::make_shared<NetworkPacketResponse>(std::move(buffer_ptr),
                                                              ResponseModel_t::PlayerInfoResponseModel, size);
        client->getClientResponseQueue().produce(std::move(packet));
    }

    Server::Server() : acceptor(ioContext, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"),
                                                         8000)) { // boost::asio::ip::address::from_string("127.0.0.1"); 192.168.1.201
        RequestQueueManager::manageRequestQueue(m_requestQueue, m_responseQueue, m_gameSession, ConnectedClients::getConnectedClients());
        std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
    }

    void Server::awaitNewConnections() {
        while (true) {
            tcp::socket socket = acceptor.accept();

            std::cout << "Connected client: " << socket.remote_endpoint() << " --> " << socket.local_endpoint()
                      << std::endl;
            uint32_t playerId = m_gameSession.createPlayerAndReturnId();
            auto teamId = m_gameSession.getPlayer(playerId)->getTeamId();
            auto client = std::make_shared<Client>(std::move(socket), playerId);

            ConnectedClients::getConnectedClients().push_back(client);
            [[maybe_unused]] auto receiverOnThisUser = ClientRequestsReceiver(client,
                                                                              &m_requestQueue); // создание двух потоков на каждого клиента
            receiverOnThisUser.start();
            [[maybe_unused]] auto senderOnThisUser = ClientResponsesSender(client);
            senderOnThisUser.start();

            registerClientInSession(client, playerId, teamId);

            if (!m_isSessionActive && ConnectedClients::getConnectedClients().size() ==
                                      m_requiredClientsCountInSession) { // создание обработчика, если комманда собралась пока что handler - заглушка
                std::cout << "Maximum players count reached. Starting the game..." << std::endl;
                m_isSessionActive = true;

                // start putting packets from main response queue to client's specific queues
                std::thread(dispatchPacketsToClients, &m_responseQueue).detach();

                // start tick controller
                m_tickController.start([this]() mutable {
                    auto request = std::make_shared<NetworkPacketRequest>(nullptr,
                                                                          RequestModel_t::UpdateGameStateRequestModel,
                                                                          0U);
                    m_requestQueue.produce(std::move(request));
                });
            }
        }
    }
}