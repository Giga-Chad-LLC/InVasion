
#ifndef INVASION_SERVER_USER_H
#define INVASION_SERVER_USER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

class User {
private:
    tcp::iostream chanel;
    SafeQueue<PlayerAction> queueForSend;

    friend class Receiver;

    friend class Sender;

    friend void handler(SafeQueue<PlayerAction> *queueOnReceive);

public:
    explicit User(tcp::socket &&socket) : chanel(std::move(socket)) {
        chanel << "Hello from C++ server!" << std::endl;
    }
};

#endif //INVASION_SERVER_USER_H
