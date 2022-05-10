#include <memory>
#include <sstream>
#include "client.h"
#include "server/Session/session.h"
#include "server/NetworkPacket/network-packet.h"


namespace invasion::server {
using boost::asio::ip::tcp;

Client::Client(std::shared_ptr <tcp::socket> socket, uint32_t clientId): m_socket(socket), m_clientId(clientId) {
    std::cout << "Created new client" << std::endl;   
}

Client::~Client() {
    stop();
    delete[] m_readBuffer;
}

void Client::start(
    std::shared_ptr <Session> session,
    std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacketRequest>>> requestQueue,
    std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacketResponse>>> clientResponseQueue    
) {
    if (m_isActive.load()) {
        return;
    }

    m_isActive.store(true);
    std::cout << "Starting a client" << std::endl;
    receiveNextPacket(requestQueue, session);
}

void Client::stop() {
    if (!m_isActive.load()) {
        return;
    }

    m_isActive.store(false);
    std::cout << "Stop client (" << m_socket->remote_endpoint() << "): " << m_clientId << std::endl;
}

uint32_t Client::getClientId() const noexcept {
    return m_clientId;
}

std::shared_ptr <tcp::socket> Client::getSocket() const noexcept {
    return m_socket;
}

void Client::receiveNextPacket(
    std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacketRequest>>> requestQueue,
    std::shared_ptr <Session> session
) {
    std::cout << "Start receiving next packet" << std::endl;
    // use callbacks for sequential reads
    read(PACKET_SIZE_LENGTH + PACKET_TYPE_LENGTH, [this, requestQueue, session](
        const boost::system::error_code& errorCode,
        std::size_t bytes_transferred
        ) {
        std::cout << "We read " << bytes_transferred << std::endl;

        if (errorCode.value() != 0) {
            std::cout << "Error while receiving from client: Error code = " << errorCode.value() << ", Message: " << errorCode.message() << std::endl;
            // close the connection with the client
            session->removeClient(m_clientId);
            return;
        }

        std::cout << "Bytes: ";
        for (int i = 0; i < bytes_transferred; i++) {
            std::cout << (int)m_readBuffer[i] << ' ';
        }
        std::cout << std::endl;
        // std::istringstream in(m_readBuffer);
        std::uint32_t messageLength;  // get the message data length in bytes
        std::memcpy(reinterpret_cast<char*> (&messageLength), m_readBuffer, PACKET_SIZE_LENGTH);
        // std::cout << "Bytes: " << m_readBuffer << std::endl;
        // in.read(reinterpret_cast<char*> (&messageLength), PACKET_SIZE_LENGTH);

        std::uint32_t messageType; // get the message type
        std::memcpy(reinterpret_cast<char*> (&messageType), m_readBuffer + PACKET_SIZE_LENGTH, PACKET_TYPE_LENGTH);
        // in.read(reinterpret_cast<char*> (&messageType), PACKET_TYPE_LENGTH);

        std::cout << "Message length: " << messageLength << std::endl;
        std::cout << "Message type: " << messageType << std::endl;

        read(messageLength, [this, requestQueue, session, messageLength, messageType](
            const boost::system::error_code& errorCode,
            std::size_t bytes_transferred
        ) {
            std::cout << "We read additional " << bytes_transferred << std::endl;
            if (errorCode.value() != 0) {
                std::cout << "Error while receiving from client: Error code = " << errorCode.value() << ", Message: " << errorCode.message() << std::endl;
                // close the connection with the client
                session->removeClient(m_clientId);
                return;
            }

            
            std::cout << "Additional Bytes: ";
            for (int i = 0; i < bytes_transferred; i++) {
                std::cout << (int)m_readBuffer[i] << ' ';
            }
            std::cout << std::endl;
            std::unique_ptr<char[]> buffer_ptr(new char[messageLength]);
            std::memcpy(buffer_ptr.get(), m_readBuffer, messageLength);

            auto packet = std::make_shared<NetworkPacketRequest>(
                std::move(buffer_ptr),
                NetworkPacketRequest::getMessageTypeById(messageType),
                messageLength
            );

            requestQueue->produce(std::move(packet));
            receiveNextPacket(requestQueue, session);
        });
    });
}

void Client::sendNextPacket(
    std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacketResponse>>> clientResponseQueue,
    std::shared_ptr <Session> session
) {
    // should be started in a separate thread
    // std::shared_ptr <NetworkPacketResponse> response;
    // if (m_isActive.load() && clientResponseQueue->consumeSync(response)) {
    //     uint32_t messageLength = response->bytesSize() + sizeof(static_cast<std::uint32_t> (response->getMessageType())) + sizeof(response->bytesSize());
    //     m_writeBuffer = response->serializeToByteArray();

    //     write(messageLength, [this, clientResponseQueue, session](const boost::system::error_code& errorCode, std::size_t bytes_transferred) {
    //         if (errorCode.value() != 0) {
    //             std::cout << "Error while writing to client socket (" << m_socket->remote_endpoint() << "), Error code: " << errorCode.value() << ", Message: " << errorCode.message() << std::endl;
                
    //             // session->removeClient(m_clientId); // remove client from session
    //             return;
    //         }

    //         sendNextPacket(clientResponseQueue, session);
    //     });
    // }
}
}