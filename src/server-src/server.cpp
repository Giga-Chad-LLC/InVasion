#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <thread>
#include <utility>
#include "../../include/server-include/server.h"

using boost::asio::ip::tcp;
namespace inVasion::session {
    Server::Server() : acceptor(ioContext, tcp::endpoint(tcp::v4(), 8000)) {
        makeEngine(queueReceive, queueSend);
        std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
    }

    void Server::makeSenderUsers() {
        std::thread(sendEachUser, &queueReceive).detach();
    }

    void Server::waitNewUser() {
        while (true) {
            tcp::socket socket = acceptor.accept();
            std::cout << "Connected client: " << socket.remote_endpoint() << " --> " << socket.local_endpoint()
                      << std::endl;
            auto pointerOnUser = std::make_shared<User>(std::move(socket));
            baseUsers.push_back(pointerOnUser);
            [[maybe_unused]] auto receiverOnThisUser = Receiver(pointerOnUser,
                                                                &queueReceive); // создание двух потоков на каждого клиента
            [[maybe_unused]] auto senderOnThisUser = Sender(pointerOnUser);
            if (!ImplementedSenderEachUser && baseUsers.size() ==
                                              NUMBER_OF_TEAM) { // создание обработчика, если комманда собралась пока что handler - заглушка
                std::cout << "team are full\n";
                ImplementedSenderEachUser = false;
                makeSenderUsers();
            }
        }
    }
}