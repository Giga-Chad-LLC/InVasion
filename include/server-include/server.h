
#ifndef INVASION_SERVER_SERVER_H
#define INVASION_SERVER_SERVER_H

#include "../../include/server-include/safe-queue.h"
#include "player.pb.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include "user.h"
#include "receiver.h"
#include "sender.h"

using boost::asio::ip::tcp;

inline std::vector<std::shared_ptr<user>> base_clients;

inline void handler(SafeQueue<PlayerAction> *q) {
    while (true) {
        PlayerAction cur;
        if (q->Consume(cur)) {
            for (auto cur_client: base_clients) {
                // пока никакой обработки
                // просто имитируем ее
                PlayerAction tmp = cur;
                cur_client->queue_send.Produce(std::move(tmp));
            }
        }
    }
}

class server {
private:
    boost::asio::io_context io_context;
    tcp::acceptor acceptor;
    const size_t NUMBER_OF_TEAM = 1;
    bool handler_implemented = false;
public:
    SafeQueue<PlayerAction> queue_receive;

    explicit server();

    void make_handler();

    void wait_new_client();
};


#endif //INVASION_SERVER_SERVER_H
