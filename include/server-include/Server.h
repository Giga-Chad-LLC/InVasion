
#ifndef INVASION_SERVER_SERVER_H
#define INVASION_SERVER_SERVER_H

#include "../../include/server-include/SafeQueue.h"
#include "player.pb.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include "User.h"
#include "Receiver.h"
#include "Sender.h"

using boost::asio::ip::tcp;

inline std::vector<std::shared_ptr<User>> baseUsers;

inline void handler(SafeQueue<PlayerAction> *queueOnReceive) {
    while (true) {
        PlayerAction cur;
        if (queueOnReceive->consume(cur)) {
            for (auto cur_client: baseUsers) { // пока никакой обработки просто имитируем ее
                PlayerAction tmp = cur;
                cur_client->queueForSend.produce(std::move(tmp));
            }
        }
    }
}

class Server {
private:
    boost::asio::io_context ioContext;
    tcp::acceptor acceptor;
    const size_t NUMBER_OF_TEAM = 1;
    bool handlerImplemented = false;
public:
    SafeQueue<PlayerAction> queueReceive;

    explicit Server();

    void makeHandler();

    void waitNewUser();
};


#endif //INVASION_SERVER_SERVER_H
