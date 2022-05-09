#ifndef CLIENT_H_
#define CLIENT_H_


#include <boost/asio.hpp>
#include <memory>
#include <iostream>
// server
#include "server/NetworkPacket/network-packet.h"
#include "server/safe-queue.h"
#include "server/Session/session_fwd.h"

namespace invasion::server {
using boost::asio::ip::tcp;

class Client {
public:
    Client(std::shared_ptr <tcp::socket> socket, uint32_t clientId);

    void stop();

    void startReceivingPackets(
        std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacket>>> requestQueue,
        std::shared_ptr <Session> session
    );

    void startSendingPackets(
        std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacket>>> clientResponseQueue,
        std::shared_ptr <Session> session
    );

    uint32_t getClientId();

    std::shared_ptr <tcp::socket> getSocket();
private:
    template <class Functor>
    void read(Functor callback) {}
    
    template <class Functor>
    void write(Functor callback) {}

    
    const std::size_t MAX_MESSAGE_LENGTH = 1024U;
    uint32_t m_clientId;
    std::shared_ptr <tcp::socket> m_socket;
    std::shared_ptr <char[]> m_readBuffer;
    std::shared_ptr <char[]> m_writeBuffer;
};
}

#endif