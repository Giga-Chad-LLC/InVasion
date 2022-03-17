
#ifndef INVASION_SERVER_USER_H
#define INVASION_SERVER_USER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "network_packet.h"
#include "game-models/GameSession/game-session.h"

namespace invasion::session {
    using boost::asio::ip::tcp;

    class User {
    private:
        tcp::iostream channel;
        SafeQueue<NetworkPacketResponse> queueClientPrivate;
        int idClientInSession;

        friend class ReceiverFromUser;

        friend class SenderUser;

        friend class Server;

        friend void dispatcherEachSender(SafeQueue<NetworkPacketResponse> *queueClientsFromServer);

        friend void initializeUserForFrontend(game_models::GameSession &session, std::shared_ptr<User> user);

    public:
        int getIdClient() const noexcept {
            return idClientInSession;
        }

        explicit User(tcp::socket &&socket, int id) : channel(std::move(socket)), idClientInSession(id) {}
    };
}
#endif //INVASION_SERVER_USER_H
