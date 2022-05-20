#include <boost/bind.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "server.h"



namespace invasion::server {
using boost::asio::ip::tcp;

Server::Server() {
    m_work.reset(new boost::asio::io_service::work(m_ios));
    std::cout << "Server is created" << std::endl;
}

Server::~Server() {
    stop();
    std::cout << "Server is destroyed" << std::endl;
}

void Server::start(std::string host, short port) {
    // more than one invocation of start() is no-op
    if (m_isActive.load()) {
        return;
    }
    m_isActive.store(true);

    m_acceptor.reset(new Acceptor(m_ios, host, port));
    m_acceptor->start();
    m_acceptor->acceptNewClient([this](const boost::system::error_code& errorCode, Connection connection){
        onAccept(errorCode, connection);
    });

    // starts in a different thread
    m_sessionRemover.start([this]() {
        // remove inactive sessions
        std::unique_lock ul{ mtx_sessions };
 
        auto end = std::remove_if(
            m_sessions.begin(), m_sessions.end(),
            [](auto session) {
                return !session->isActive();
            }
        );

        std::cout << "(Daemon) Kill inactive sessions" << std::endl;
        m_sessions.erase(end, m_sessions.end());
    });

    m_ios.run();
}

void Server::stop() {
    // more than one invocation of stop() is no-op
    if (!m_isActive.load()) {
        return;
    }

    m_isActive.store(false);
    // stop acceptor
    m_acceptor->stop();
    // stop blocking the execution thread
    m_ios.stop();
    

    // stop the sessions 
    {
        std::unique_lock ul{ mtx_sessions };
        for (auto session : m_sessions) {
            session->stop();
        }
    }

    // stop session remover (it will stop on the next interval tick)
    std::cout << "Stopping session remover (daemon)" << std::endl;
    m_sessionRemover.stop();
    std::cout << "Server stopped" << std::endl;
}


void Server::onAccept(const boost::system::error_code& errorCode, Connection connection) {    
    if (errorCode.value() == 0) {
        // add client to the session
        std::cout << "Connected client: " << connection.socket->remote_endpoint() << " --> " << connection.socket->local_endpoint() << std::endl;

        auto availableSession = getAvailableSession();
        std::cout << "Available session: " << availableSession->getSessionId() << std::endl;
        availableSession->addClient(
            connection.socket,
            {
                connection.ios,
                connection.work
            }
        );
    }
    else {
        std::cout << "Server Error: Error code = " << errorCode.value() << ", Message: " << errorCode.message() << std::endl;
    }

    // Init next async accept operation if server has not been stopped yet.
    if (m_acceptor->isActive()) {
        std::cout << "Acceptor is active, accept new client" << std::endl;
        m_acceptor->acceptNewClient([this](const boost::system::error_code& errorCode, Connection connection){
            onAccept(errorCode, connection);
        });
    }
    else {
        std::cout << "Acceptor isn't active, close acceptor" << std::endl;
        // Stop accepting incoming connections.
        m_acceptor->close();
    }
}


std::shared_ptr<Session> Server::getAvailableSession() { 
    std::unique_lock ul{ mtx_sessions };
       
    for (auto session : m_sessions) {
        if (session->isAvailable()) {
            std::cout << "Found available session" << std::endl;
            return session;
        }
    }
    std::cout << "No available sessions" << std::endl;
    return addSession();
}

std::shared_ptr<Session> Server::addSession() {
    // mtx_sessions is assumed to be locked
    std::cout << "Creating new session" << std::endl;
    m_sessions.push_back(std::make_shared <Session> (m_nextSessionId++));
    m_sessions.back()->start();
    return m_sessions.back();
}

void Server::removeSession(uint32_t sessionId) {
    std::unique_lock ul{ mtx_sessions };

    for (auto it = m_sessions.begin(); it != m_sessions.end(); it++) {
        if ((*it)->getSessionId() == sessionId) {
            (*it)->stop();
            m_sessions.erase(it);
            break;
        }
    }
}
}