
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
#include "User.h"
#include "receiver.h"
#include "sender.h"

using boost::asio::ip::tcp;

inline std::vector<std::shared_ptr<User>> baseСlients;

inline void handler(SafeQueue<PlayerAction> *q) {
    while (true) {
        PlayerAction cur;
        if (q->consume(cur)) {
            for (auto cur_client: baseСlients) {
                // пока никакой обработки
                // просто имитируем ее
                PlayerAction tmp = cur;
                cur_client->queueSend.produce(std::move(tmp));
            }
        }
    }
}

class server {
private:
    boost::asio::io_context ioСontext;
    tcp::acceptor acceptor;
    const size_t NUMBER_OF_TEAM = 1;
    bool handlerImplemented = false;
public:
    SafeQueue<PlayerAction> queueReceive;

    explicit server();

    void makeHandler();

    void waitNewClient();
};


#endif //INVASION_SERVER_SERVER_H
