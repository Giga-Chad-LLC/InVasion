
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


class Client{
private:
    tcp::iostream chanel;
    SafeQueue<PlayerAction> queue_send;
    friend void make_receiver(std::shared_ptr<Client> client,SafeQueue<PlayerAction> *q);
    friend void make_sender(std::shared_ptr<Client> client);
    friend void handler(SafeQueue<PlayerAction> *q);
public:
    Client(tcp::socket &&socket) : chanel(std::move(socket)){
        chanel << "Hello from C++ server!" << std::endl;
    }
};


void make_receiver(std::shared_ptr<Client> client, SafeQueue<PlayerAction> *q){

        while (client->chanel) {

            std::uint32_t size;  // get the message data length in bytes
            client->chanel.read(reinterpret_cast<char *> (&size), sizeof(size));
            char arr[size];
            client->chanel.read(reinterpret_cast<char *> (&arr), size);

            PlayerAction action;
            action.ParseFromArray(arr, size);
            q->Produce(std::move(action));


//            std::cout << "Action retrieve: " << action.key_pressed() << ", size in bytes: " << action.ByteSizeLong()
//                      << std::endl;
//            char action_buffer[action.ByteSizeLong()];
//            action.SerializeToArray(action_buffer, action.ByteSizeLong());
//
//            client->chanel.write(action_buffer, action.ByteSizeLong());
        }
        std::cout << "Client disconnected" << std::endl;

}

void make_sender(std::shared_ptr<Client> client){
    while(true){
            PlayerAction action;
            if (client->queue_send.Consume(action)){
                std::cout << "OK";
                char action_buffer[action.ByteSizeLong()];
                action.SerializeToArray(action_buffer, action.ByteSizeLong());

                client->chanel.write(action_buffer, action.ByteSizeLong());
            }
        }

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
std::vector<std::shared_ptr<Client>> base_clients;

void handler(SafeQueue<PlayerAction> *q);
class server {
private:
    boost::asio::io_context io_context;
    tcp::acceptor acceptor;

    bool handler_implemented = false;
public:
    SafeQueue<PlayerAction> queue_receive;
    server() : acceptor(io_context, tcp::endpoint(tcp::v4(), 8000)) {
        std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;

    }
    void make_handler(){
        if (!handler_implemented){
            std::thread(handler, &queue_receive).detach();
            handler_implemented = true;
        }
    }
    void wait_new_client() {
        bool flag = true;
        while (true) {
            tcp::socket s = acceptor.accept();
            std::cout << "Connected client: " << s.remote_endpoint() << " --> " << s.local_endpoint()
                      << std::endl;
            auto pointer_on_user = std::make_shared<Client>(std::move(s));
            base_clients.push_back(pointer_on_user);
            std::thread(make_receiver, pointer_on_user, &queue_receive).detach();
            std::thread(make_sender, pointer_on_user).detach();
            if (base_clients.size() == 1 && flag){
                std::cout << "team are full\n";
                flag = false;
                make_handler();
            }
        }
    }
};

void handler(SafeQueue<PlayerAction> *q){ // если у нас подключатся юзеры после метода make_handler, то он их просто не увидит
    while(true){
        PlayerAction cur;
        if (q->Consume(cur)){
            for (auto cur_client : base_clients){
                // пока никакой обработки
                PlayerAction tmp = cur;
                cur_client->queue_send.Produce(std::move(tmp));
            }
        }
    }
}
#endif //INVASION_SERVER_SERVER_H
