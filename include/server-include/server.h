
#ifndef INVASION_SERVER_SERVER_H
#define INVASION_SERVER_SERVER_H

#include "../../include/server-include/safe-queue.h"
#include "player.pb.h"
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
using boost::asio::ip::tcp;

class server {
private:
    SafeQueue<PlayerAction> queue_receive;
    SafeQueue<PlayerAction> queue_send;
    boost::asio::io_context io_context;
    tcp::acceptor acceptor;
public:
    server() : acceptor(io_context, tcp::endpoint(tcp::v4(), 8000)){
        std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
        wait_new_client();
    }

    void wait_new_client() {

        while (true) {
            tcp::socket s = acceptor.accept();

            std::thread([socket = std::move(s)]() mutable {
                std::cout << "Connected client: " << socket.remote_endpoint() << " --> " << socket.local_endpoint()
                          << std::endl;
                tcp::iostream client(std::move(socket));
                client << "Hello from C++ server!" << std::endl;

                while (client) {

                    std::uint32_t size;  // get the message data length in bytes
                    client.read(reinterpret_cast<char *> (&size), sizeof(size));
                    char arr[size];
                    client.read(reinterpret_cast<char *> (&arr), size);

                    PlayerAction action;
                    action.ParseFromArray(arr, size);
                    std::cout << "Action retrieve: " << action.key_pressed() << ", size in bytes: " << action.ByteSizeLong() << std::endl;
                    char action_buffer[action.ByteSizeLong()];
                    action.SerializeToArray(action_buffer, action.ByteSizeLong());

                    client.write(action_buffer, action.ByteSizeLong());
                }
                std::cout << "Client disconnected" << std::endl;
            }).detach();
        }
    }
};

#endif //INVASION_SERVER_SERVER_H
