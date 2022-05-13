
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
        mutable tcp::iostream m_channel;
        SafeQueue<std::shared_ptr<NetworkPacketResponse>> m_clientResponseQueue;
    public:

        Client() = default;

        explicit Client(tcp::socket &&socket, uint32_t playerId);

        uint32_t getClientIDInSession() const noexcept {
            return m_clientIdInGameSession;
        }

        tcp::iostream &getChannel() const {
            return m_channel;
        }

        auto &getClientResponseQueue() {
            return m_clientResponseQueue;
        }
    };
}
#endif //INVASION_SERVER_USER_H
