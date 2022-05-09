#include <memory>
#include "client.h"
#include "server/Session/session.h"


namespace invasion::server {
using boost::asio::ip::tcp;

Client::Client(std::shared_ptr <tcp::socket> socket, uint32_t clientId): m_socket(socket), m_clientId(clientId), m_isActive(true) {
    std::cout << "Crated new client" << std::endl;
}

void Client::stop() {
    std::cout << "Stop client (" << m_socket->remote_endpoint() << "): " << m_clientId << std::endl;
}

uint32_t Client::getClientId() const noexcept {
    return m_clientId;
}

std::shared_ptr <tcp::socket> Client::getSocket() const noexcept {
    return m_socket;
}

void Client::startReceivingPackets(
    std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacketRequest>>> requestQueue,
    std::shared_ptr <Session> session
) {
    
}

void Client::startSendingPackets(
    std::shared_ptr <SafeQueue<std::shared_ptr <NetworkPacketResponse>>> clientResponseQueue,
    std::shared_ptr <Session> session
) {
    
}
}