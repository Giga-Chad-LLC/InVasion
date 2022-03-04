#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <thread>
#include <utility>
#include "../../include/server-include/server.h"

using boost::asio::ip::tcp;

server::server(): acceptor(this->ioContext, tcp::endpoint(tcp::v4(), 8000)) {
    std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
}

void server::makeHandler() {
    std::thread(handler, &queueReceive).detach();
}

void server::waitNewClient() {
    while (true) {
        tcp::socket socket = acceptor.accept();
        std::cout << "Connected client: " << socket.remote_endpoint() << " --> " << socket.local_endpoint()
                  << std::endl;
        auto pointerOnUser = std::make_shared<User>(std::move(socket));
        baseClients.push_back(pointerOnUser);
        auto receiverOnThisUser = Receiver(pointerOnUser, &queueReceive); // создание двух потоков на каждого клиента
        auto senderOnThisUser = Sender(pointerOnUser);
        if (baseClients.size() == NUMBER_OF_TEAM &&
            !handlerImplemented) { // создание обработчика, если комманда собралась пока что handler - заглушка
            std::cout << "team are full\n";
            handlerImplemented = false;
            makeHandler();
        }
    }
}