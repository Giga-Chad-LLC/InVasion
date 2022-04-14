
#ifndef INVASION_SERVER_USER_H
#define INVASION_SERVER_USER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server/NetworkPacket/network-packet.h"
#include "server/safe-queue.h"
#include "game-models/GameSession/game-session.h"

namespace invasion::session {
    using boost::asio::ip::tcp;

    class Client {
    private:
        uint32_t m_clientIdInGameSession;
        tcp::iostream m_channel;
        SafeQueue<NetworkPacketResponse> m_clientResponseQueue;

        friend class ClientRequestsReceiver;
        friend class ClientResponsesSender;
        friend class Server;

        friend void dispatchPacketsToClients(SafeQueue<NetworkPacketResponse> *responseQueue);
        friend void registerClientInSession(std::shared_ptr<Client> client, uint32_t playerId);
        friend inline void manageRequestQueue(SafeQueue<NetworkPacketRequest> &requestQueue, SafeQueue<NetworkPacketResponse> &responseQueue, game_models::GameSession &gameSession);

    public:
        Client() = default;
        explicit Client(tcp::socket &&socket, uint32_t playerId);
    };
}
#endif //INVASION_SERVER_USER_H
