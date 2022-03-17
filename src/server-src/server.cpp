#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <thread>
#include <utility>
#include "server-include/server.h"
#include "server-include/network_packet.h"
#include "player-id-response-model.pb.h"
#include "update-game-state-request-model.pb.h"
#include "interactors/InitialStateResponseInteractor/initial-state-response-interactor.h"

using boost::asio::ip::tcp;
namespace invasion::session {

    void initializeUserForFrontend(game_models::GameSession &session, std::shared_ptr<User> user) {
        // send to user his ID
        auto responseFromInteractor = invasion::interactors::InitialStateResponseInteractor::execute(session,
                                                                                                     user->getIdClient());
        auto sizeMessage = responseFromInteractor.ByteSizeLong();
        std::unique_ptr<char> buffer_ptr(new char[sizeMessage]);
        responseFromInteractor.SerializeToArray(buffer_ptr.get(), sizeMessage);
        auto responseInBytes = NetworkPacketResponse(std::move(buffer_ptr), ResponseModel_t::PlayerIdResponseModel,
                                                     sizeMessage);
        user->queueClientPrivate.produce(std::move(responseInBytes));
    }


    Server::Server() : acceptor(ioContext, tcp::endpoint(boost::asio::ip::address::from_string("192.168.1.201"),
                                                         8000)) { // boost::asio::ip::address::from_string("127.0.0.1"); 192.168.1.201
        HandlerQueues(queueServerFromClients, queueClientsFromServer, curGameSession);
        std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
    }

    void Server::makeSenderUsers() {
        std::thread(dispatcherEachSender, &queueClientsFromServer).detach();
    }

    void Server::waitNewUser() {
        while (true) {
            tcp::socket socket = acceptor.accept();

            std::cout << "Connected client: " << socket.remote_endpoint() << " --> " << socket.local_endpoint()
                      << std::endl;
            auto pointerOnUser = std::make_shared<User>(std::move(socket), curGameSession.addPlayer());
            baseUsers.push_back(pointerOnUser);
            [[maybe_unused]] auto receiverOnThisUser = ReceiverFromUser(pointerOnUser,
                                                                        &queueServerFromClients); // создание двух потоков на каждого клиента
            [[maybe_unused]] auto senderOnThisUser = SenderUser(pointerOnUser);

            initializeUserForFrontend(curGameSession, pointerOnUser);

            if (!ImplementedDispatherEachSender && baseUsers.size() ==
                                                   NUMBER_OF_TEAM) { // создание обработчика, если комманда собралась пока что handler - заглушка
                std::cout << "Maximum players count reached. Starting the game..." << std::endl;
                ImplementedDispatherEachSender = true;
                makeSenderUsers();
                // start tick controller
                tickController.start([this]() mutable {
                    NetworkPacketRequest request(nullptr, RequestModel_t::UpdateGameStateRequestModel, 0U);
                    queueServerFromClients.produce(std::move(request));
                });
            }
        }
    }
}