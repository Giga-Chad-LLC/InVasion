
#ifndef INVASION_SERVER_USER_H
#define INVASION_SERVER_USER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

class user {
private:
    tcp::iostream chanel;
    SafeQueue<PlayerAction> queue_send;

    friend class receiver;

    friend class sender;

    friend void handler(SafeQueue<PlayerAction> *q);

public:
    explicit user(tcp::socket &&socket) : chanel(std::move(socket)) {
        chanel << "Hello from C++ server!" << std::endl;
    }
};

#endif //INVASION_SERVER_USER_H
