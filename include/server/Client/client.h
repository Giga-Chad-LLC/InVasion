#ifndef CLIENT_H_
#define CLIENT_H_


#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <string>
// server
#include "server/NetworkPacket/network-packet.h"
#include "server/safe-queue.h"
#include "server/Session/session-fwd.h"

namespace invasion::server {
using boost::asio::ip::tcp;

class Client {
public:
    Client(
        std::shared_ptr <tcp::socket> socket,
        int clientId,
        std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacketResponse>>> clientResponseQueue
    );
    ~Client();

    void start(
        std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacketRequest>>> requestQueue,
        std::shared_ptr <Session> session
    );
    void stop();

    int getClientId() const noexcept;

    std::shared_ptr <tcp::socket> getSocket() const noexcept;

private:
    template <class Functor>
    void read(std::size_t totalMessageLength, Functor callback) {
        boost::asio::async_read(
            *m_socket.get(),
            boost::asio::buffer(m_readBuffer, totalMessageLength),
            [this, callback] (const boost::system::error_code& errorCode, std::size_t bytes_transferred) {
                callback(errorCode, bytes_transferred);
            }
        );
    }

    template <class Functor>
    void write(std::size_t totalMessageLength, Functor callback) {
        boost::asio::async_write(
            *m_socket.get(),
            boost::asio::buffer(m_writeBuffer.get(), totalMessageLength),
            [this, callback] (const boost::system::error_code& errorCode, std::size_t bytes_transferred) {
                callback(errorCode, bytes_transferred);
            }
        );
    }
    
    void receiveNextPacket(
        std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacketRequest>>> requestQueue,
        std::shared_ptr <Session> session
    );

    void sendNextPacket(
        std::shared_ptr <Session> session
    );

    void onError(
        const boost::system::error_code& errorCode,
        std::shared_ptr <Session> session
    );

    const std::size_t PACKET_SIZE_LENGTH = sizeof(std::uint32_t);
    const std::size_t PACKET_TYPE_LENGTH = sizeof(std::uint32_t);
    const std::size_t MAX_MESSAGE_LENGTH = 1024U;


    std::shared_ptr<SafeQueue<std::shared_ptr<NetworkPacketResponse>>> m_clientResponseQueue;
    bool m_canStartNextWriteAction = true;
    std::condition_variable cv_writeNextPacket;
    std::mutex mtx_writeNextPacket;
    std::thread m_writeThread;
    int m_clientId;
    std::atomic_bool m_isActive = false;
    std::shared_ptr <tcp::socket> m_socket;
    char* m_readBuffer = new char[MAX_MESSAGE_LENGTH]; // needed to use pure pointer, cuz boost (or me) is damn stupid...
    std::shared_ptr <char[]> m_writeBuffer;
};
}

#endif