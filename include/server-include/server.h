
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
#include "network_packet.h"
#include "game-models/GameSession/game-session.h"
using boost::asio::ip::tcp;
namespace invasion::session {
    inline std::vector<std::shared_ptr<User>> baseUsers;

    inline void dispatcherEachSender(SafeQueue<NetworkPacketResponse> *queueClientsFromServer) {
        while (true) {
            NetworkPacketResponse cur;
            if (queueClientsFromServer->consume(cur)) {
                for (auto cur_client: baseUsers) { // пока никакой обработки просто имитируем ее
                    NetworkPacketResponse tmp = cur;
                    cur_client->queueForSend.produce(std::move(tmp));
                }
            }
        }
    }

    class Server {
    private:
        boost::asio::io_context ioContext;
        tcp::acceptor acceptor;
        const size_t NUMBER_OF_TEAM = 2;
        bool ImplementedDispatherEachSender = false;
        SafeQueue<NetworkPacketRequest> queueServerFromClients;
        SafeQueue<NetworkPacketResponse> queueClientsFromServer;
        game_models::GameSession curGameSession;
    public:

        explicit Server();

        void makeSenderUsers();

        void waitNewUser();

        friend void makeEngine(SafeQueue<NetworkPacketRequest> &queueReceive, SafeQueue<NetworkPacketResponse> &queueSend);

        friend class ReceiverFromUser;

        friend void dispatcherEachSender(SafeQueue<NetworkPacketResponse> *queueClientsFromServer);
    };

}
#endif //INVASION_SERVER_SERVER_H
