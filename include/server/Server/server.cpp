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
    m_acceptor.reset(new Acceptor(m_ios, host, port));
    m_acceptor->start();
    m_acceptor->acceptNewClient([this](const boost::system::error_code& errorCode, Connection connection){
        this->onAccept(errorCode, connection);
    });
}

void Server::stop() {
    m_acceptor->stop();
    m_ios.stop();
    std::cout << "Server stopped" << std::endl;
}


void Server::onAccept(const boost::system::error_code& errorCode, Connection connection) {
    std::cout << this->test << std::endl;
    
    if (errorCode.value() == 0) {
        // add client to the session
        std::cout << "Connected client: " << connection.socket->remote_endpoint() << " --> " << connection.socket->local_endpoint() << std::endl;
    }
    else {
        std::cout << "Server Error: Error code = " << errorCode.value() << ", Message: " << errorCode.message() << std::endl;
    }

    // Init next async accept operation if server has not been stopped yet.
    if (m_acceptor->isActive()) {
        m_acceptor->acceptNewClient([this](const boost::system::error_code& errorCode, Connection connection){
            this->onAccept(errorCode, connection);
        });
    }
    else {
        // Stop accepting incoming connections.
        m_acceptor->close();
    }
}




std::shared_ptr<Session> Server::getAvailableSession() {
    return nullptr;
}

std::shared_ptr<Session> Server::addSession() {
    return nullptr;
}

void Server::removeSession(uint32_t sessionId) {}
}