#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <thread>
#include <utility>
#include "../../include/server-include/Server.h"

using boost::asio::ip::tcp;

Server::Server() : acceptor(ioContext, tcp::endpoint(tcp::v4(), 8000)) {
    std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
}

void Server::makeHandler() {
    std::thread(handler, &queueReceive).detach();
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
        if (!handlerImplemented && baseUsers.size() == NUMBER_OF_TEAM) { // создание обработчика, если комманда собралась пока что handler - заглушка
            std::cout << "team are full\n";
            handlerImplemented = false;
            makeHandler();
        }
    }
}