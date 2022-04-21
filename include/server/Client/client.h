
#ifndef INVASION_SERVER_USER_H
#define INVASION_SERVER_USER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server/NetworkPacket/network-packet.h"
#include "server/safe-queue.h"
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player-team-id-enum.h"

namespace invasion::session {
    using boost::asio::ip::tcp;

    class Client {
    private:
        uint32_t m_clientIdInGameSession;
        tcp::iostream m_channel;
        SafeQueue<std::shared_ptr<NetworkPacketResponse>> m_clientResponseQueue;

        friend class ClientRequestsReceiver;
        friend class ClientResponsesSender;
        friend class Server;
        friend class RequestQueueManager;

        friend void dispatchPacketsToClients(SafeQueue<std::shared_ptr<NetworkPacketResponse>> *responseQueue);
        friend void registerClientInSession(std::shared_ptr<Client> client, uint32_t playerId, game_models::PlayerTeamId teamId);

    public:
        Client() = default;
        explicit Client(tcp::socket &&socket, uint32_t playerId);
    };
}
#endif //INVASION_SERVER_USER_H
