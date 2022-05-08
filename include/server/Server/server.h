#ifndef SERVER_H_
#define SERVER_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/core/noncopyable.hpp>
#include <memory>
#include <atomic>
#include <string>

#include "server/Session/session.h"
#include "controllers/FixedTimeIntervalInvoker/fixed-time-interval-invoker.h"


namespace invasion::server {
using boost::asio::ip::tcp;

class Server : private boost::noncopyable {
public:
    explicit Server(std::string host, short port);
    ~Server();
    // starts the server acceptor
    void start();
    // stops the server and releases all resources
    void stop();
private:
    struct Connection {
        explicit Connection(
            std::shared_ptr<boost::asio::io_service> ios, std::shared_ptr<boost::asio::io_service::work> work, std::shared_ptr<tcp::socket> socket
        ): ios(ios), work(work), socket(socket) {}

        std::shared_ptr<boost::asio::io_service> ios;
        std::shared_ptr<boost::asio::io_service::work> work;
        std::shared_ptr<tcp::socket> socket;
    };

    // waits for new client and puts it to the available session
    void acceptNewClient();
    void onAccept(const boost::system::error_code& error, Connection connection);
    // returns the best-fit session
    std::shared_ptr<Session> getAvailableSession();
    // creates and adds new session
    std::shared_ptr<Session> addSession();
    // removes session by id
    void removeSession(uint32_t sessionId);

    tcp::acceptor m_acceptor;
    boost::asio::io_service m_ios;
    std::atomic <bool> m_isActive = true;
    invasion::controllers::FixedTimeIntervalInvoker m_sessionRemover = controllers::FixedTimeIntervalInvoker(1000 * 60);
    std::vector <std::shared_ptr <Session>> m_sessions;
};
}

#endif