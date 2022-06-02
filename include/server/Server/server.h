#ifndef SERVER_H_
#define SERVER_H_

#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/system/error_code.hpp>
#include <memory>
#include <atomic>
#include <string>
#include <mutex>
#include <iostream>

#include "server/Session/session.h"
#include "controllers/FixedTimeIntervalInvoker/fixed-time-interval-invoker.h"


namespace invasion::server {
using boost::asio::ip::tcp;



class Server : private boost::noncopyable {
private:
    struct Connection {
        std::shared_ptr<boost::asio::io_service> ios;
        std::shared_ptr<boost::asio::io_service::work> work;
        std::shared_ptr<tcp::socket> socket;
    };

    class Acceptor {
    public:

        Acceptor(boost::asio::io_service& ios, std::string host, short port):
            m_acceptor(ios, tcp::endpoint(boost::asio::ip::address_v4::from_string(host), port)) {}
        
        // starts listening for the incoming connections
        void start() {
            m_acceptor.listen();
            std::cout << "Listening at " << m_acceptor.local_endpoint() << std::endl;
        }

        void stop() {
            m_isActive.store(false);
        }

        void close() {
            m_acceptor.close();
        }

        bool isActive() {
            return m_isActive.load();
        }

        // waits for new client and calls the callback when it is connected
        template <class Functor>
        void acceptNewClient(Functor callback) {
            std::cout << "Accepting new client" << std::endl;
            // create execution thread for the newly connected client
            std::shared_ptr<boost::asio::io_service> ioService(new boost::asio::io_service);
            std::shared_ptr<boost::asio::io_service::work> ioWork(new boost::asio:: io_service::work(*ioService));
            std::shared_ptr<tcp::socket> socket(new tcp::socket(*ioService));

            Connection connection{ioService, ioWork, socket};

            m_acceptor.async_accept(*connection.socket, [callback, connection](const boost::system::error_code& errorCode) {
                std::cout << "Handle a connected user" << std::endl;
                callback(errorCode, connection);
            });
        }
    private:
        tcp::acceptor m_acceptor;
        std::atomic_bool m_isActive = true;
    };

public:
    Server();
    ~Server();
    // starts the server acceptor (blocks the current thread of execution)
    void start(std::string host, short port);
    // stops the server and releases all resources
    void stop();
    // removes session by id
    void removeSession(uint32_t sessionId);
private:
    // session remove time interval 
    const std::size_t SESSION_REMOVER_TIME_INTERVAL_MS = 1000 * 60 * 1;
    // next session id
    uint32_t m_nextSessionId = 0U;
    // callback is invoked when the client connected successfully 
    void onAccept(const boost::system::error_code& errorCode, Connection connection);
    // returns the best-fit session
    std::shared_ptr<Session> getAvailableSession();
    // creates and adds new session
    std::shared_ptr<Session> addSession();

    std::unique_ptr <Acceptor> m_acceptor;
    boost::asio::io_service m_ios;
    std::shared_ptr <boost::asio::io_service::work> m_work;
    invasion::controllers::FixedTimeIntervalInvoker m_sessionRemover = controllers::FixedTimeIntervalInvoker(SESSION_REMOVER_TIME_INTERVAL_MS);
    std::vector <std::shared_ptr <Session>> m_sessions;
    std::mutex mtx_sessions;
    
    std::atomic_bool m_isActive = false;
};
}

#endif