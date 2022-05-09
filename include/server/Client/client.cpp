#include <memory>
#include "client.h"


namespace invasion::server {
using boost::asio::ip::tcp;

Client::Client(std::shared_ptr <tcp::socket> socket, uint32_t clientId): m_socket(socket), m_clientId(clientId) {}

void Client::stop() {
    std::cout << "Stop client: " << m_socket->remote_endpoint() << std::endl;
}
}