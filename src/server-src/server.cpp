#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <thread>
#include <utility>
#include "../../include/server-include/server.h"
#include "../../include/server-include/network_packet.h"
#include "player-id-response-model.pb.h"

using boost::asio::ip::tcp;
namespace invasion::session {
    Server::Server() : acceptor(ioContext, tcp::endpoint(boost::asio::ip::address::from_string("192.168.1.201"), 8000)) { // boost::asio::ip::address::from_string("127.0.0.1"); 192.168.1.201
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
            auto pointerOnUser = std::make_shared<User>(std::move(socket));
            baseUsers.push_back(pointerOnUser);
            [[maybe_unused]] auto receiverOnThisUser = ReceiverFromUser(pointerOnUser,
                                                                        &queueServerFromClients); // создание двух потоков на каждого клиента
            [[maybe_unused]] auto senderOnThisUser = SenderUser(pointerOnUser);
            
            {
                // send to user his generated ID
                response_models::PlayerIdResponseModel playerIdResponse;
                playerIdResponse.set_playerid(pointerOnUser->getIdClient());
                std::unique_ptr<char> buffer_ptr(new char[playerIdResponse.ByteSizeLong()]);
                playerIdResponse.SerializeToArray(buffer_ptr.get(), playerIdResponse.ByteSizeLong());
                pointerOnUser->queueForSend.produce(std::move(NetworkPacketResponse(std::move(buffer_ptr), ResponseModel_t::PlayerIdResponseModel, playerIdResponse.ByteSizeLong())));
            }

            if (!ImplementedDispatherEachSender && baseUsers.size() ==
                                                   NUMBER_OF_TEAM) { // создание обработчика, если комманда собралась пока что handler - заглушка
                std::cout << "team are full" << std::endl;
                ImplementedDispatherEachSender = true;
                makeSenderUsers();
            }
        }
    }
}