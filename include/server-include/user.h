
#ifndef INVASION_SERVER_USER_H
#define INVASION_SERVER_USER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace inVasion::session {
    using boost::asio::ip::tcp;

    class User {
    private:
        tcp::iostream channel;
        SafeQueue<PlayerAction> queueToClientPrivate;

        friend class ReceiverFromUser;

        friend class SenderUser;

        friend void dispatcherEachSender(SafeQueue<PlayerAction> *queueClientsFromServer);

    public:
        explicit User(tcp::socket &&socket) : channel(std::move(socket)) {
            channel << "Hello from C++ server!" << std::endl;
        }
    };
}
#endif //INVASION_SERVER_USER_H
