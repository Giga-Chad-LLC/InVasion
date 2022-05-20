#include <memory>
#include <sstream>
#include <iostream>
#include "client.h"
#include "server/Session/session.h"
#include "server/NetworkPacket/network-packet.h"


namespace invasion::server {
using boost::asio::ip::tcp;

Client::Client(
    std::shared_ptr <tcp::socket> socket,
    int clientId,
    std::weak_ptr <SafeQueue<
        std::pair<
            std::shared_ptr <NetworkPacketResponse>,
            std::shared_ptr <LatchCaller>
        >
    >> clientResponseQueue
): m_socket(socket), m_clientId(clientId), m_clientResponseQueue(clientResponseQueue) {
    std::cout << "Created new client" << std::endl;   
}

Client::~Client() {
    stop();
    delete[] m_readBuffer;
    std::cout << "Client " << m_clientId << " destroyed" << std::endl;
}

void Client::start(
    std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacketRequest>>> requestQueue,
    std::shared_ptr <Session> session    
) {
    if (m_isActive.load()) {
        return;
    }

    m_isActive.store(true);
    std::cout << "Starting a client" << std::endl;
    receiveNextPacket(requestQueue, session);
    m_writeThread = std::move(std::thread([this, session]() mutable {
        std::cout << "Client (" << m_socket->remote_endpoint() << ") write thread starts" << std::endl;
        sendNextPacket(session);
        std::cout << "Client (" << m_socket->remote_endpoint() << ") write thread exits" << std::endl;
    }));
}

void Client::stop() {
    if (!m_isActive.load()) {
        return;
    }
    
    m_isActive.store(false);
    // notify client to check his `m_isActive`
    if (!m_clientResponseQueue.expired()) {
        m_clientResponseQueue.lock()->finish();
    }
    else {
        std::cout << "Client error: response queue is nullptr before client (id: " << m_clientId << ") destroy" << std::endl; 
    }
    m_canStartNextWriteAction = true;
    cv_writeNextPacket.notify_one();

    m_writeThread.join();
    std::cout << "Stop client (" << m_socket->remote_endpoint() << "): " << m_clientId << std::endl;
}

int Client::getClientId() const noexcept {
    return m_clientId;
}

std::shared_ptr <tcp::socket> Client::getSocket() const noexcept {
    return m_socket;
}

void Client::receiveNextPacket(
    std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacketRequest>>> requestQueue,
    std::shared_ptr <Session> session
) {
    // use callbacks for sequential reads
    read(PACKET_SIZE_LENGTH + PACKET_TYPE_LENGTH, [this, requestQueue, session](
        const boost::system::error_code& errorCode,
        std::size_t bytes_transferred
    ) {
        if (errorCode.value() != 0) {
            onError(errorCode, session);
            return;
        }

        std::uint32_t messageLength;  // get the message data length in bytes
        std::memcpy(reinterpret_cast<char*> (&messageLength), m_readBuffer, PACKET_SIZE_LENGTH);

        std::uint32_t messageType; // get the message type
        std::memcpy(reinterpret_cast<char*> (&messageType), m_readBuffer + PACKET_SIZE_LENGTH, PACKET_TYPE_LENGTH);

        read(messageLength, [this, requestQueue, session, messageLength, messageType](
            const boost::system::error_code& errorCode,
            std::size_t bytes_transferred
        ) {
            if (errorCode.value() != 0) {
                onError(errorCode, session);
                return;
            }

            std::unique_ptr<char[]> buffer_ptr(new char[messageLength]);
            std::memcpy(buffer_ptr.get(), m_readBuffer, messageLength);

            auto packet = std::make_shared<NetworkPacketRequest>(
                std::move(buffer_ptr),
                NetworkPacketRequest::getMessageTypeById(messageType),
                messageLength
            );
            
            requestQueue->produce(std::move(packet));
            if (m_isActive.load()) {
                receiveNextPacket(requestQueue, session);
            }
            else {
                std::cout << "Stop client (" << m_socket->remote_endpoint() << ") reading calls" << std::endl;
            }
        });
    });
}

void Client::sendNextPacket(
    std::shared_ptr <Session> session
) {
    // this method should be started in a separate thread
    std::pair <
        std::shared_ptr <NetworkPacketResponse>,
        std::shared_ptr <LatchCaller>
    > data;

    while (m_isActive.load() && !m_clientResponseQueue.expired() && m_clientResponseQueue.lock()->consumeSync(data)) {
        auto [ response, latchCaller ] = data;

        uint32_t messageLength = response->totalSize();
        m_writeBuffer = response->serializeToByteArray();
        
        std::unique_lock ul{mtx_writeNextPacket};
        cv_writeNextPacket.wait(ul, [this]() { return m_canStartNextWriteAction; });
        m_canStartNextWriteAction = false;

        write(
            messageLength,
            [this, latchCaller = std::move(latchCaller), session](
                const boost::system::error_code& errorCode,
                std::size_t bytes_transferred
            ) {
                if (latchCaller != nullptr) {
                    (*latchCaller)(); // since we disconnect user if we fail to send the data, we must call the latcher counter even if the package sending fails 
                }

                if (errorCode.value() != 0) {
                    onError(errorCode, session);
                    return;
                }

                m_canStartNextWriteAction = true;
                cv_writeNextPacket.notify_one();
            }
        );
    }
}


void Client::onError(
    const boost::system::error_code& errorCode,
    std::shared_ptr <Session> session
) {
    std::cout << "Error while receiving from client: Error code = " << errorCode.value() << ", Message: " << errorCode.message() << std::endl;
    // close the connection with the client
    session->removeClient(m_clientId);
}
}