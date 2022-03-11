
#ifndef INVASION_SERVER_USER_H
#define INVASION_SERVER_USER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "network_packet.h"

namespace invasion::session {
    using boost::asio::ip::tcp;

    class User {
    private:
        tcp::iostream channel;
        SafeQueue<NetworkPacketResponse> queueForSend;
        int idClientInSession;
        friend class ReceiverFromUser;

        friend class SenderUser;

        friend void dispatcherEachSender(SafeQueue<NetworkPacketResponse> *queueClientsFromServer);

    public:
        int getIdClient() const noexcept{
            return idClientInSession;
        }
        explicit User(tcp::socket &&socket, int id) : channel(std::move(socket)), idClientInSession(id) {}
    };
}
#endif //INVASION_SERVER_USER_H
