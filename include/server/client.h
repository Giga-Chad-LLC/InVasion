
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
        int m_clientIdInSession;

        friend class ClientRequestsReceiver;
        friend class ClientResponsesSender;
        friend class Server;

        friend void dispatchPacketsToClients(SafeQueue<NetworkPacketResponse> *responseQueue);
        friend void registerClientInSession(game_models::GameSession &session, std::shared_ptr<Client> client);

    public:
        int getClientId() const noexcept {
            return m_clientIdInSession;
        }

        explicit Client(tcp::socket &&socket, int id) : m_channel(std::move(socket)), m_clientIdInSession(id) {}
    };
}
#endif //INVASION_SERVER_USER_H
