#include <algorithm>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include "player.pb.h"
#include "../../include/server-include/server.h"

using boost::asio::ip::tcp;

server::server() : acceptor(io_context, tcp::endpoint(tcp::v4(), 8000)) {
    std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
}

void server::make_handler() {
    std::thread(handler, &queue_receive).detach();
}

void server::wait_new_client() {
    while (true) {
        tcp::socket s = acceptor.accept();
        std::cout << "Connected client: " << s.remote_endpoint() << " --> " << s.local_endpoint()
                  << std::endl;
        auto pointer_on_user = std::make_shared<user>(std::move(s));
        base_clients.push_back(pointer_on_user);
        auto new_receiver = receiver(pointer_on_user, &queue_receive); // создание двух потоков на каждого клиента
        auto new_sender = sender(pointer_on_user);
        if (base_clients.size() == NUMBER_OF_TEAM &&
            !handler_implemented) { // создание обработчика, если комманда собралась пока что handler - заглушка
            std::cout << "team are full\n";
            handler_implemented = false;
            make_handler();
        }
    }
}