
#ifndef INVASION_SERVER_USER_H
#define INVASION_SERVER_USER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "network-packet.h"
#include "game-models/GameSession/game-session.h"

namespace invasion::session {
    using boost::asio::ip::tcp;

    class Client {
    private:
        tcp::iostream m_channel;
        SafeQueue<NetworkPacketResponse> m_clientResponseQueue;

        friend class ClientRequestsReceiver;
        friend class ClientResponsesSender;
        friend class Server;

        friend void dispatchPacketsToClients(SafeQueue<NetworkPacketResponse> *responseQueue);
        friend void registerClientInSession(std::shared_ptr<Client> client, int playerId);

    public:
        explicit Client(tcp::socket &&socket) : m_channel(std::move(socket)) {}
    };
}
#endif //INVASION_SERVER_USER_H
