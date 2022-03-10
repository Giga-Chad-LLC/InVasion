
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

        friend class ReceiverFromUser;

        friend class SenderUser;

        friend void dispatcherEachSender(SafeQueue<NetworkPacketResponse> *queueOnReceive);

    public:
        explicit User(tcp::socket &&socket) : channel(std::move(socket)) {}
    };
}
#endif //INVASION_SERVER_USER_H
