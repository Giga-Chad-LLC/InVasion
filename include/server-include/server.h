
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

//void handler(SafeQueue<PlayerAction> *q){
//    while(true){
//        while(q->Size() > 0){
//
//        }
//    }
//}
class Client{
private:
    tcp::iostream chanel;
    SafeQueue<PlayerAction> queue_send;
    friend void make_receiver(std::shared_ptr<Client> client);
public:
    Client(tcp::socket &&socket) : chanel(std::move(socket)){
        chanel << "Hello from C++ server!" << std::endl;
    }
};


void make_receiver(std::shared_ptr<Client> client){

        while (client->chanel) {

            std::uint32_t size;  // get the message data length in bytes
            client->chanel.read(reinterpret_cast<char *> (&size), sizeof(size));
            char arr[size];
            client->chanel.read(reinterpret_cast<char *> (&arr), size);

            PlayerAction action;
            action.ParseFromArray(arr, size);
//        q->Produce(std::move(action));

            std::cout << "Action retrieve: " << action.key_pressed() << ", size in bytes: " << action.ByteSizeLong()
                      << std::endl;
            char action_buffer[action.ByteSizeLong()];
            action.SerializeToArray(action_buffer, action.ByteSizeLong());

            client->chanel.write(action_buffer, action.ByteSizeLong());
        }
        std::cout << "Client disconnected" << std::endl;

}

void make_sender(SafeQueue<PlayerAction> *q, tcp::iostream *client){

}


//void return_client(tcp::socket &&socket, SafeQueue<PlayerAction> *queue_receive) {
//
//    SafeQueue<PlayerAction> queue_send;
//
//    std::thread a (make_receiver, queue_receive, &client);
//    a.detach();
//    while(true){
//
//    }
////    std::thread(make_sender, &queue_send, &client).detach();
//
//
//
//}

class server {
private:
    boost::asio::io_context io_context;
    tcp::acceptor acceptor;
    std::vector<std::shared_ptr<Client>> base_clients;
public:
    SafeQueue<PlayerAction> queue_receive;
    server() : acceptor(io_context, tcp::endpoint(tcp::v4(), 8000)) {
        std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;
    }

    void wait_new_client() {

        while (true) {
            tcp::socket s = acceptor.accept();
            std::cout << "Connected client: " << s.remote_endpoint() << " --> " << s.local_endpoint()
                      << std::endl;
            auto pointer_on_user = std::make_shared<Client>(std::move(s));
            base_clients.push_back(pointer_on_user);
            std::thread(make_receiver, pointer_on_user).detach();
        }
    }
};

#endif //INVASION_SERVER_SERVER_H
