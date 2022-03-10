#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <thread>
#include <utility>
#include "../../include/server-include/server.h"

using boost::asio::ip::tcp;
namespace inVasion::session {
    Server::Server() : acceptor(ioContext, tcp::endpoint(tcp::v4(), 8000)) {
        makeEngine(queueToEngine, queueFromEngine);
        std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
    }

    void Server::makeSenderUsers() {
        std::thread(dispatcherEachSender, &queueFromEngine).detach();
    }

    void Server::waitNewUser() {
        while (true) {
            tcp::socket socket = acceptor.accept();
            std::cout << "Connected client: " << socket.remote_endpoint() << " --> " << socket.local_endpoint()
                      << std::endl;
            auto pointerOnUser = std::make_shared<User>(std::move(socket));
            baseUsers.push_back(pointerOnUser);
            [[maybe_unused]] auto receiverOnThisUser = ReceiverFromUser(pointerOnUser,
                                                                        &queueToEngine); // создание двух потоков на каждого клиента
            [[maybe_unused]] auto senderOnThisUser = SenderUser(pointerOnUser);
            if (!ImplementedDispatherEachSender && baseUsers.size() ==
                                                   NUMBER_OF_TEAM) { // создание обработчика, если комманда собралась пока что handler - заглушка
                std::cout << "team are full\n";
                ImplementedDispatherEachSender = false;
                makeSenderUsers();
            }
        }
    }
}