
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
#include "engine.h"

using boost::asio::ip::tcp;
namespace inVasion::session {
    inline std::vector<std::shared_ptr<User>> baseUsers;

    inline void sendEachUser(SafeQueue<PlayerAction> *queueOnReceive) {
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
        bool ImplementedSenderEachUser = false;
        SafeQueue<PlayerAction> queueReceive;
        SafeQueue<PlayerAction> queueSend;
    public:

        explicit Server();

        void makeSenderUsers();

        void waitNewUser();

        friend void makeEngine(SafeQueue<PlayerAction> &queueReceive, SafeQueue<PlayerAction> &queueSend);

        friend class Receiver;

        friend void sendEachUser(SafeQueue<PlayerAction> *queueOnReceive);
    };

}
#endif //INVASION_SERVER_SERVER_H
